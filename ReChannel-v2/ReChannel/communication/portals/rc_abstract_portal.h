// vim:set et sts=4 ts=4 tw=75 sw=4 ai ci cin cino=g0,t0:
/*
 * Copyright (C) 2007, Technical Computer Science Group,
 *                     University of Bonn
 *
 * This file is part of the ReChannel library.
 *
 * The ReChannel library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors: Andreas Raabe and Armin Felke. Implementation by Armin Felke.
 *          {raabe, felke}@cs.uni-bonn.de
 */
/**
 * @file    rc_abstract_portal.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Definition of class rc_abstract_portal.
 *
 *
 * $Date: 2008/01/01 13:46:06 $
 * $Revision: 1.12 $
 */

#ifndef RC_ABSTRACT_PORTAL_H_
#define RC_ABSTRACT_PORTAL_H_

#include "ReChannel/communication/rc_abstract_switch.h"
#include "ReChannel/communication/rc_abstract_interface_wrapper.h"
#include "ReChannel/communication/rc_event_forwarder.h"
#include "ReChannel/communication/accessors/rc_accessor.h"
#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/core/rc_non_reconfigurable.h"
#include "ReChannel/util/rc_port_traits.h"

namespace ReChannel {

/**
 * @brief Base class of a portal implementation.
 */
class rc_portal_base
    : public sc_module,
      virtual public rc_switch
{
private:
    typedef rc_switch::state_type state_type;

protected:
    typedef rc_interface_wrapper_base::accessor_base accessor_base_type;
    typedef rc_switch::filter_chain                  filter_chain;

protected:
    rc_portal_base(
        const sc_module_name& module_name,
        state_type& m_state, unsigned int& m_transaction_count);

    /** @brief initialize (must be called directly after construction) */
    void _rc_init(rc_interface_wrapper_base& interface_wrapper);

public:

    /** @brief returns the currently active dynamic module */
    virtual rc_reconfigurable* get_current_reconfigurable() const
        { return p_dyn_module; }

    /** @brief returns the currently active dynamic interface */
    virtual sc_interface* get_dynamic_interface() const
        { return p_dyn_accessor; }

protected:

    void register_port(rc_port_handle port_);

    /** @brief this method is called when the portal is opened up */
    virtual void rc_on_open() { }

    /** @brief this method is called when the portal is closed */
    virtual void rc_on_close() { }

    /** @brief this method is called when the portal is set to undefined state */
    virtual void rc_on_undef() { }

    /** @brief this method is called when the portal refreshes its event notif. */
    virtual void rc_on_refresh_notify() = 0;

    /** @brief opens up the portal irrespectively of a specific rc_reconfigurable */
    virtual void open();

    /** @brief opens up the portal and activates the given dynamic module */
    virtual void open(
        rc_reconfigurable& module,
        const filter_chain& filters = filter_chain());

    /** @brief closes the portal and deactivates the current dynamic interface */
    virtual void close();

    /** @brief deactivates the current dynamic module and set to undefined state */
    virtual void set_undefined();

    /** @brief refreshes the event notifications of this portal */
    virtual void refresh_notify()
        { this->rc_on_refresh_notify(); }

    /** @brief registers the given dynamic module/interface pair */
    virtual void register_reconfigurable(
        rc_reconfigurable& module, sc_interface& dyn_interface);

    /** @brief unregisters the given reconfigurable module */
    virtual void unregister_reconfigurable(rc_reconfigurable& module);

    /** @brief callback for interface accesses (begin access) */
    inline rc_process_behavior_change begin_access_callback();

    /** @brief callback for interface accesses (end access) */
    inline void end_access_callback();

    virtual ~rc_portal_base() {}

private:

    void add_event_forwarder_target(
        sc_interface& target_if, rc_event_filter* filter=NULL);

    void clear_event_forwarder_targets();

private:

/* special callback method */

    virtual bool _rc_check_accessor_if_type(
        accessor_base_type* accessor) const = 0;

private:

    /** @brief a reference to the m_state member */
    state_type&   p_state;

    /** @brief a reference to the m_transaction_count member */
    unsigned int& p_transaction_count;

    /** @brief a pointer to the interface wrapper */
    rc_interface_wrapper_base* p_interface_wrapper;

    /** @brief currently active dynamic accessor */
    accessor_base_type* p_dyn_accessor;

    /** @brief the last currently active dynamic filter in the chain */
    accessor_base_type* p_last_dyn_filter;

    /** @brief currently active dynamic filter chain */
    filter_chain p_dyn_filter_chain;

    /** @brief currently active dynamic module */
    rc_reconfigurable* p_dyn_module;
};

/**
 * @brief Base class of a portal implementation.
 */
template<class IF>
class rc_portal_b
    : public rc_portal_base,
      public rc_abstract_switch<IF>
{
    RC_STATIC_ASSERT_VALID_INTERFACE(IF);

private:
    typedef rc_portal_b<IF>        this_type;
    typedef rc_portal_base         base_type;
    typedef rc_abstract_switch<IF> switch_parent_type;

    typedef typename base_type::accessor_base_type      accessor_base_type;
    typedef typename rc_interface_wrapper<IF>::accessor accessor_b_type;

    typedef rc_switch::state_type state_type;

    typedef boost::function<const sc_event& (IF*)> event_getter_functor;
    typedef std::vector<std::pair<event_getter_functor, std::string> >
        ef_constr_vector;

protected:
    using switch_parent_type::m_state;
    using switch_parent_type::m_transaction_count;

public:
    typedef IF              if_type;
    typedef rc_accessor<IF> accessor_type;

private:
    class interface_wrapper;

protected:
    /** @brief constructor */
    explicit rc_portal_b(const sc_module_name& module_name);

    /** @brief initialize (must be called directly after construction) */
    void _rc_init(rc_port_handle static_port);

public:

    /** @brief returns the kind of this portal */
    virtual std::string get_switch_kind() const
        { return "portal"; }

    /** @brief returns the name of this portal */
    virtual std::string get_switch_name() const
        { return this->name(); }

    /** @brief returns the static interface */
    virtual sc_interface* get_static_interface() const;

protected:

    /** @brief creates an unconnected accessor */
    virtual accessor_type* create_accessor() const
        { return new accessor_type(); }

    /** @brief this method is called when the portal refreshes its event notif. */
    virtual void rc_on_refresh_notify() { }

    /** @brief registers the given dynamic module/interface pair */
    virtual void register_reconfigurable(
        rc_reconfigurable& module, sc_interface& dyn_interface);

    /** @brief unregisters the given reconfigurable module */
    virtual void unregister_reconfigurable(rc_reconfigurable& module);

    /** @brief things which have to be done at end of elaboration */
    virtual void end_of_elaboration();

    /** @brief bind static_port to rc_fallback_interface<IF> */
    virtual void unused() = 0;

    virtual bool is_unused() const = 0;

    /** @brief creates an event forwarder for the specified event getter method */
    template<class IF_>
    inline
    void add_event_forwarder(
        const sc_event& (IF_::*event_getter)() const,
        const std::string& event_name=NULL);

    /** @brief triggers the notification of the specified dynamic event */
    inline void notify_event(const std::string& event_name)
        { p_interface_wrapper.notify_event(event_name); }

    /** @brief returns a reference to the portal's interface wrapper */
    inline rc_interface_wrapper<IF>& get_interface_wrapper()
        { return p_interface_wrapper; }

private:

/* for internal use only */

    virtual bool _rc_check_accessor_if_type(
        accessor_base_type* accessor) const;

private:
    /** @brief the interface wrapper for the static interface */
    interface_wrapper p_interface_wrapper;

    /** @brief a handle to the static port */
    rc_port_handle p_static_port;

    /** @brief event forwarder construction vector */
    ef_constr_vector p_ef_constr_vector;
};

/**
 * @brief The interface wrapper within portals.
 */
template<class IF>
class rc_portal_b<IF>::interface_wrapper
    : public rc_abstract_interface_wrapper<IF>
{
    friend class rc_portal_b<IF>;

private:
    typedef rc_portal_b<IF> portal_type;
    typedef rc_abstract_interface_wrapper<IF> base_type;

    typedef const sc_event& (IF::*event_getter_ptr)() const;

public:
    typedef typename base_type::accessor_type accessor_type;

public:
    explicit interface_wrapper(portal_type& owner_portal)
        : base_type(), p_portal(owner_portal)
        { }

    /** @brief returns the name of this portal */
    virtual std::string get_interface_wrapper_name() const
        { return p_portal.name(); }

    virtual void register_port(
        sc_port_base& port_, const char* if_typename)
        { /* ignore */ }

    /** @brief creates an event forwarder for the specified event getter method */
    inline void add_event_forwarder(
        IF& trigger_if, event_getter_functor functor,
        const std::string& event_name)
    { base_type::add_event_forwarder(trigger_if, functor, event_name); }

protected:
    /** @brief creates an accessor */
    virtual accessor_type* create_accessor() const
        { return p_portal.create_accessor(); }

    /** @brief callback for interface accesses (begin access) */
    virtual rc_process_behavior_change begin_access_callback()
        { return p_portal.begin_access_callback(); }

    /** @brief callback for interface accesses (end access) */
    virtual void end_access_callback()
        { p_portal.end_access_callback(); }

private:
    /** @brief a reference to the portal owning this object */
    portal_type& p_portal;
};

/**
 * @brief An abstract portal class.
 */
template<class PORT>
class rc_abstract_portal
    : public rc_portal_b<typename rc_port_traits<PORT>::if_type>
{
    RC_STATIC_ASSERT_VALID_PORT(PORT);

private:
    typedef rc_port_traits<PORT> port_traits;

public:
    typedef PORT                          port_type;
    typedef typename port_traits::if_type if_type;
    typedef rc_accessor<if_type>          accessor_type;

private:
    typedef rc_abstract_portal<port_type> this_type;
    typedef rc_portal_b<if_type>          base_type;

protected:
    /** @brief constructors */
    rc_abstract_portal();
    explicit rc_abstract_portal(const sc_module_name& module_name);

public:

    /** @brief bind the portal to the given static port */
    void bind_static(port_type& port_)
        { static_port.bind(port_); }

    /** @brief bind the portal to a given static channel */
    void bind_static(if_type& channel_)
        { static_port.bind(channel_); }

    /** @brief create a new accessor and bind it to the given dynamic port */
    void bind_dynamic(port_type& port_);

    /** @brief allows dynamic port binding (alias for bind_dynamic()) */
    void dynamic_port(port_type& port_)
        { this->bind_dynamic(port_); }

    /** @brief bind static_port to rc_fallback_interface<IF> */
    virtual void unused();

    virtual bool is_unused() const
        { return p_is_unused; }

    /** @brief returns the static port */
    inline port_type& get_static_port()
        { return static_port; }

protected:

    virtual void bind_static_object(const rc_object_handle& obj_to_bind);

    virtual void bind_dynamic_object(const rc_object_handle& obj_to_bind);

public:
    /** @brief the port for the static interface */
    port_type static_port;

private:

    /** @brief flag to indicate if portal is declared as unused */
    bool p_is_unused;
};

/* inline code */

inline rc_process_behavior_change rc_portal_base::begin_access_callback()
{
    ++p_transaction_count;
    if (p_dyn_module != NULL) {
        p_dyn_module->rc_begin_transaction();
    }
    rc_process_handle hproc = rc_get_current_process_handle();
    return hproc.behavior_change(false);
}

inline void rc_portal_base::end_access_callback()
{
    if (p_transaction_count > 0) {
        --p_transaction_count;
    }
    if (p_dyn_module != NULL) {
        p_dyn_module->rc_end_transaction();
    }
}

/* template code */

template<class IF>
rc_portal_b<IF>::rc_portal_b(
    const sc_module_name& module_name)
    : base_type(module_name, m_state, m_transaction_count),
      p_interface_wrapper(*this)
{
    base_type::_rc_init(p_interface_wrapper);
}

template<class IF>
void rc_portal_b<IF>::_rc_init(rc_port_handle static_port)
{
    // check correct usage
    assert(!p_static_port.valid());

    // initialize (if necessary)
    if (!p_static_port.valid()) {
        p_static_port = static_port;
    }
}

template<class IF>
sc_interface* rc_portal_b<IF>::get_static_interface() const
{
    // check valid initialization
    assert(p_static_port.valid());

    return p_static_port->get_interface();
}

template<class IF>
template<class IF_>
inline
void rc_portal_b<IF>::add_event_forwarder(
    const sc_event& (IF_::*event_getter)() const,
    const std::string& event_name)
{
    // check valid internal initialization
    assert(p_static_port.valid());

    if (!this->is_unused()) {
        // (note: get_interface() will most likely return NULL)
        IF* trigger_if = dynamic_cast<IF*>(p_static_port.get_interface());
        if (trigger_if == NULL) {
            p_ef_constr_vector.push_back(
                typename ef_constr_vector::value_type(event_getter, event_name));
        } else {
            p_interface_wrapper.add_event_forwarder(
                *trigger_if, event_getter, event_name);
        }
    }
}

template<class IF>
void rc_portal_b<IF>::register_reconfigurable(
        rc_reconfigurable& module, sc_interface& dyn_if)
{
    base_type::register_reconfigurable(module, dyn_if);
    switch_parent_type::register_reconfigurable(module, dyn_if);
}

template<class IF>
void rc_portal_b<IF>::unregister_reconfigurable(rc_reconfigurable& module)
{
    base_type::unregister_reconfigurable(module);
    switch_parent_type::unregister_reconfigurable(module);
}

template<class IF>
void rc_portal_b<IF>::end_of_elaboration()
{
    base_type::end_of_elaboration();
    sc_interface* static_if = this->get_static_interface();
    if (static_if == NULL
    || p_interface_wrapper.set_wrapped_interface(*static_if) == false)
    {
        RC_REPORT_ERROR(RC_ID_INTERNAL_ERROR_,
            "static port is incompatible with the declared"
            " interface (in portal '" << this->name() << "')");
    }

    if (!this->is_unused()) {
        IF* trigger_if = dynamic_cast<IF*>(static_if);
        if (trigger_if != NULL) {
            for (typename ef_constr_vector::iterator it =
                    p_ef_constr_vector.begin();
                it != p_ef_constr_vector.end();
                ++it)
            {
                typename ef_constr_vector::value_type& ef_constr = (*it);
                p_interface_wrapper.add_event_forwarder(
                    *trigger_if, ef_constr.first,
                    ef_constr.second.c_str());
            }
        }
        p_ef_constr_vector.clear();
    }
}

template<class IF>
bool rc_portal_b<IF>::_rc_check_accessor_if_type(
    accessor_base_type* accessor) const
{
    return (dynamic_cast<accessor_b_type*>(accessor) != NULL);
}

template<class PORT>
rc_abstract_portal<PORT>::rc_abstract_portal()
    : base_type(sc_gen_unique_name("portal")),
      static_port("static_port"), p_is_unused(false)
{
    base_type::_rc_init(static_port);
}

template<class PORT>
rc_abstract_portal<PORT>::rc_abstract_portal(
    const sc_module_name& module_name)
    : base_type(module_name),
      static_port("static_port"), p_is_unused(false)
{
    base_type::_rc_init(static_port);
}

template<class PORT>
void rc_abstract_portal<PORT>::bind_dynamic(port_type& port_)
{
    if (port_.get_interface() == NULL) {
        accessor_type* accessor_ = this->create_accessor();
        if (accessor_ == NULL) {
            RC_REPORT_ERROR(RC_ID_NULL_POINTER_,
                "method create_accessor() did not create an accessor"
                " (in portal '" << this->name() << "')");
        }
        port_.bind(*accessor_);
    }
    register_port(port_);
}

template<class PORT>
void rc_abstract_portal<PORT>::unused()
{
    if_type* undef_if = rc_fallback_interface_factory<if_type>::create();
    if (undef_if != NULL) {
        static_port.bind(*undef_if);
        p_is_unused = true;
    } else {
        RC_REPORT_ERROR(RC_ID_NOT_IMPLEMENTED_,
            "no matching rc_fallback_interface found"
            " (in portal '" << this->name() << "')");
    }
}

template<class PORT>
void rc_abstract_portal<PORT>::bind_static_object(
    const rc_object_handle& obj_to_bind)
{
    if (obj_to_bind.is_port()) {
        port_type* port_to_bind =
            dynamic_cast<port_type*>(*obj_to_bind);
        if (port_to_bind != NULL) {
            static_port.bind(*port_to_bind);
            return;
        }
    } else if (obj_to_bind.is_channel()) {
        if_type* if_to_bind = dynamic_cast<if_type*>(*obj_to_bind);
        if (if_to_bind != NULL) {
            static_port.bind(*if_to_bind);
            return;
        }
    }
    RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
        "'" << obj_to_bind->name() << "'"
        " is incompatible with the static port"
        " (in portal '" << this->name() << "')");
}

template<class PORT>
void rc_abstract_portal<PORT>::bind_dynamic_object(
    const rc_object_handle& obj_to_bind)
{
    if (obj_to_bind.is_port()) {
        port_type* port_to_bind = dynamic_cast<port_type*>(*obj_to_bind);
        if (port_to_bind != NULL) {
            this->bind_dynamic(*port_to_bind);
            return;
        }
    }
    RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
        "'" << obj_to_bind->name() << "'"
        " is incompatible with the dynamic port"
        " (in portal '" << this->name() << "')");
}

} // namespace ReChannel

#endif // RC_ABSTRACT_PORTAL_H_
//
// $Id: rc_abstract_portal.h,v 1.12 2008/01/01 13:46:06 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/portals/rc_abstract_portal.h,v $
//
