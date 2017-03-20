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
 * @file    rc_abstract_exportal.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Definition of class rc_abstract_exportal.
 *
 *
 * $Date: 2008/01/01 13:46:06 $
 * $Revision: 1.20 $
 */

#ifndef RC_ABSTRACT_EXPORTAL_H_
#define RC_ABSTRACT_EXPORTAL_H_

#include "ReChannel/communication/rc_abstract_switch.h"
#include "ReChannel/communication/rc_abstract_interface_wrapper.h"
#include "ReChannel/communication/exportals/rc_wrapper_pool.h"
#include "ReChannel/communication/accessors/rc_accessor.h"
#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/core/rc_non_reconfigurable.h"

namespace ReChannel {

/**
 * @brief Base class of an exportal switch.
 */
class rc_exportal_base
    : public sc_module,
      virtual public rc_switch,
      virtual protected internals::exportal::wrapper_factory
{
private:
    typedef internals::exportal::wrapper_pool   wrapper_pool;
    typedef internals::exportal::wrapper_handle wrapper_handle;
    typedef rc_switch::state_type state_type;

protected:
    typedef internals::exportal::wrapper_factory wrapper_factory;

    typedef rc_interface_wrapper_base::accessor_base accessor_base_type;
    typedef rc_switch::filter_chain                  filter_chain;

protected:
    rc_exportal_base(
        const sc_module_name& module_name,
        state_type& m_state, unsigned int& m_transaction_count,
        wrapper_pool& s_wrapper_pool_);

    /** @brief initialize (must be called (once) after construction) */
    void _rc_init(accessor_base_type& accessor);

public:

    /** @brief returns the currently active dynamic module */
    virtual rc_reconfigurable* get_current_reconfigurable() const
        { return p_dyn_module; }

    /** @brief returns the currently active dynamic interface */
    virtual sc_interface* get_dynamic_interface() const;

protected:

    /** @brief binds a dynamic export to this exportal */
    void bind_dynamic(rc_export_handle export_);

    /** @brief binds a dynamic interface to this exportal */
    void bind_dynamic(sc_interface& dynamic_if_);

    /** @brief binds a dynamic interface (+wrapper) exclusively to this exportal */
    void bind_exclusively(rc_interface_wrapper_base& wrapper);

    /** @brief this method is called when the exportal is opened up */
    virtual void rc_on_open() { }

    /** @brief this method is called when the exportal is closed */
    virtual void rc_on_close() { }

    /** @brief this method is called when the exportal refreshes its event notif. */
    virtual void rc_on_refresh_notify() = 0;

    /** @brief opens up the exportal irrespectively of a specific rc_reconfigurable */
    virtual void open();

    /** @brief opens up the exportal and activates the given dynamic module */
    virtual void open(
        rc_reconfigurable& module,
        const filter_chain& filters = filter_chain());

    /** @brief closes the exportal and deactivates the current dynamic interface */
    virtual void close();

    /** @brief deactivates the current dynamic module and set to undefined state */
    virtual void set_undefined();

    /** @brief refreshes the event notifications of this exportal */
    virtual void refresh_notify()
        { this->rc_on_refresh_notify(); }

    /** @brief registers the given reconfigurable module/interface pair */
    virtual void register_reconfigurable(
        rc_reconfigurable& module, sc_interface& dyn_interface);

    /** @brief unregisters the given reconfigurable module from this switch */
    virtual void unregister_reconfigurable(rc_reconfigurable& module);

    /** @brief callback for interface accesses (begin access) */
    inline rc_process_behavior_change begin_access_callback();

    /** @brief callback for interface accesses (end access) */
    inline void end_access_callback();

    /** @brief notifies the specified dynamic event */
    void notify_event(const std::string& event_name);

    /** @brief returns least recently active dynamic interface wrapper */
    rc_interface_wrapper_base* get_interface_wrapper_base() const;

    virtual ~rc_exportal_base() {}

private:

    void add_event_forwarder_target(
        sc_interface& target_if, rc_event_filter* filter=NULL);

    void clear_event_forwarder_targets();

private:

/* special callback methods */

    virtual accessor_base_type& _rc_get_static_accessor() = 0;

    virtual bool _rc_check_interface_type(sc_interface* dyn_if) const = 0;

    virtual bool _rc_check_accessor_if_type(
        accessor_base_type* accessor) const = 0;

    virtual void _rc_set_interface_wrapper_owner(
        rc_interface_wrapper_base* wrapper) = 0;

private:

    /** @brief a reference to the m_state member */
    state_type&    p_state;

    /** @brief a reference to the m_transaction_count member */
    unsigned int&  p_transaction_count;

    /** @brief a reference to the static interface wrapper pool */
    wrapper_pool&  s_wrapper_pool;

    /** @brief access to the wrapper of the currently active dynamic interface */
    wrapper_handle p_dyn_wrapper_handle;

    /** @brief currently active dynamic filter chain */
    filter_chain   p_dyn_filter_chain;

    /** @brief first currently active dynamic filter in the chain */
    accessor_base_type* p_first_dyn_filter;

    /** @brief currently active dynamic module */
    rc_reconfigurable*  p_dyn_module;

    /** @brief reserving the last active wrapper for later reuse */
    wrapper_handle      p_reserved_wrapper_handle;

    /** @brief wrapper of an exclusive bound interface */
    rc_interface_wrapper_base* p_exclusive_wrapper;
};

/**
 * @brief Class of an abstract exportal switch.
 */
template<class IF>
class rc_abstract_exportal
    : public rc_exportal_base,
      public rc_abstract_switch<IF>
{
    RC_STATIC_ASSERT_VALID_INTERFACE(IF);

private:
    typedef rc_abstract_exportal<IF> this_type;
    typedef rc_exportal_base         base_type;
    typedef rc_abstract_switch<IF>   switch_parent_type;
    typedef rc_switch::state_type    state_type;

    typedef typename base_type::accessor_base_type      accessor_base_type;
    typedef typename rc_interface_wrapper<IF>::accessor accessor_b_type;

    typedef boost::function<const sc_event& (IF*)>    event_getter_type;
    typedef std::pair<std::string, event_getter_type> ef_decl_type;
    typedef std::vector<ef_decl_type>                 ef_decl_vector;

    typedef internals::exportal::wrapper_pool wrapper_pool;

protected:
    typedef rc_exportal_base::wrapper_factory wrapper_factory;

    using switch_parent_type::m_state;
    using switch_parent_type::m_transaction_count;

public:
    typedef IF              if_type;
    typedef rc_accessor<IF> accessor_type;

private:
    class interface_wrapper;

public:

    /** @brief returns the kind of this portal */
    virtual std::string get_switch_kind() const
        { return "portal"; }

    /** @brief returns the name of this portal */
    virtual std::string get_switch_name() const
        { return this->name(); }

    /** @brief returns the static interface */
    virtual sc_interface* get_static_interface() const
        { return &this->get_static_accessor(); }

    /** @brief returns the static sc_export (creates it if not exists) */
    sc_export<IF>& static_export();

    /** @brief returns the static sc_export (creates it if not exists) */
    operator sc_export<IF>&()
        { return this->static_export(); }

    /** @brief provides access to the export (creates accessor if not exists) */
    IF* operator->()
        { return this->static_export().operator->(); }

    /** @brief bind a static port to this exportal */
    void static_export(sc_port<IF>& port_)
        { port_.bind(this->static_export()); }

    /** @brief bind a static port to this exportal */
    void bind_static(sc_port<IF>& port_)
        { port_.bind(this->static_export()); }

    /** @brief bind a static export to this exportal */
    void bind_static(sc_export<IF>& export_)
        { export_.bind(this->static_export()); }

    /** @brief bind a dynamic export to this exportal */
    void bind_dynamic(sc_export<IF>& export_)
        { base_type::bind_dynamic(rc_export_handle(export_)); }

    /** @brief bind a dynamic interface to this exportal */
    void bind_dynamic(IF& dynamic_if_)
        { base_type::bind_dynamic(dynamic_if_); }

    /** @brief exclusively bind an interface to this exportal */
    rc_interface_wrapper<IF>& bind_exclusively(IF& dynamic_if_);

    /** @brief alias for bind_dynamic() */
    void dynamic_export(sc_export<IF>& export_)
        { base_type::bind_dynamic(export_); }

protected:

    /** @brief constructor */
    explicit rc_abstract_exportal(const sc_module_name& module_name);

    virtual void bind_static_object(const rc_object_handle& obj_to_bind);

    virtual void bind_dynamic_object(const rc_object_handle& obj_to_bind);

    /** @brief this method is called when the exportal refreshes its event notif. */
    virtual void rc_on_refresh_notify() { }

    /** @brief registers the given reconfigurable module/interface pair */
    virtual void register_reconfigurable(
        rc_reconfigurable& module, sc_interface& dyn_interface);

    /** @brief unregisters the given reconfigurable module from this switch */
    virtual void unregister_reconfigurable(rc_reconfigurable& module);

    /** @brief declares an event forwarder for the specified event getter method */
    void add_event_forwarder(
        event_getter_type event_getter,
        const std::string& event_name=NULL);

    /** @brief returns least recently active dynamic interface wrapper */
    rc_interface_wrapper<IF>* get_interface_wrapper() const;

    /** @brief returns least recently active dynamic interface */
    IF* get_interface() const;

    /** @brief interface wrapper factory */
    virtual rc_interface_wrapper_base* create_interface_wrapper(
        sc_interface& wrapped_if);

    /** @brief creates an unconnected accessor */
    virtual accessor_type* create_accessor() const
        { return new accessor_type(); }

    virtual void before_end_of_elaboration();

private:

    /** @brief returns the static accessor (creates it if not exists) */
    accessor_type& get_static_accessor() const;

private:

/* special callback method definitions */

    virtual accessor_base_type& _rc_get_static_accessor()
        { return this->get_static_accessor(); }

    virtual bool _rc_check_interface_type(sc_interface* dyn_if) const
        { return (dynamic_cast<IF*>(dyn_if) != NULL); }

    virtual bool _rc_check_accessor_if_type(
        accessor_base_type* accessor) const
        { return (dynamic_cast<accessor_b_type*>(accessor) != NULL); }

    /**
     * @brief set this exportal as the new owner of the interface wrapper
     * (this requires the interface wrapper to be of the internal
     *    interface wrapper type)
     */
    virtual void _rc_set_interface_wrapper_owner(
        rc_interface_wrapper_base* wrapper);

private:
    /** @brief the sc_export for the static side
     * \note
     * p_static_export cannot be created directly, because of a bug
     * in OSCI's-SystemC (v2.2.0) concerning binding checks before the
     * call of before_end_of_elaboration(). This is not in compliance
     * with the IEEE standard.
     * Solution: The creation of the object is delayed until a call
     * to static_export() or before_end_of_elaboration() alternatively
     * (depends on which one is called first).
     */
    sc_export<IF>* p_static_export;

    /** @brief the static accessor
     * \note
     * The concrete accessor cannot be created when the exportal's
     * constructor is executed. Thus, to get around this, the point of
     * creation is located at the get_static_accessor() method.
     * p_static_accessor is declared as 'mutable' to allow its creation
     * from out of a 'const' context.
     */
    mutable accessor_type* p_static_accessor;

    /** @brief list of event forwarder declarations */
    ef_decl_vector p_ef_decl_vector;

private:
    /** @brief the static interface wrapper pool */
    static wrapper_pool s_wrapper_pool;
};

template<class IF>
typename rc_abstract_exportal<IF>::wrapper_pool
    rc_abstract_exportal<IF>::s_wrapper_pool;

/**
 * @brief Interface wrapper within an exportal switch.
 */
template<class IF>
class rc_abstract_exportal<IF>::interface_wrapper
    : public rc_abstract_interface_wrapper<IF>
{
    friend class rc_abstract_exportal<IF>;

private:
    typedef rc_abstract_interface_wrapper<IF> base_type;

    typedef rc_abstract_exportal<IF>               exportal_type;
    typedef typename exportal_type::ef_decl_vector ef_decl_vector;

public:
    typedef typename base_type::accessor_type accessor_type;

public:
    interface_wrapper(IF& wrapped_if, exportal_type& owner_exportal);

    /** @brief tests if the given exportal is the owner of the interface wrapper */
    bool is_owner(exportal_type& owner_exportal) const
        { return (p_exportal == &owner_exportal); }

    /** @brief returns the name of this exportal */
    virtual std::string get_interface_wrapper_name() const
        { return p_exportal->name(); }

protected:
    /** @brief changes the owning exportal */
    void set_owner(exportal_type& owner_exportal)
        { p_exportal = &owner_exportal; }

    /** @brief creates an accessor */
    virtual accessor_type* create_accessor() const
        { return p_exportal->create_accessor(); }

    /** @brief callback for interface accesses (begin access) */
    virtual rc_process_behavior_change begin_access_callback()
        { return p_exportal->begin_access_callback(); }

    /** @brief callback for interface accesses (end access) */
    virtual void end_access_callback()
        { p_exportal->end_access_callback(); }

private:
    /** @brief a reference to the exportal owning this object */
    exportal_type* p_exportal;
};

/* inline code */

inline
rc_process_behavior_change rc_exportal_base::begin_access_callback()
{
    ++p_transaction_count;
    if (p_dyn_module != NULL) {
        p_dyn_module->rc_begin_transaction();
        rc_process_handle hproc = rc_get_current_process_handle();
        return hproc.behavior_change(false);
    } else {
        return rc_process_behavior_change();
    }
}

inline
void rc_exportal_base::end_access_callback()
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
rc_abstract_exportal<IF>::rc_abstract_exportal(
    const sc_module_name& module_name)
    : base_type(module_name, m_state, m_transaction_count, s_wrapper_pool),
      p_static_export(NULL), p_static_accessor(NULL)
{ }

template<class IF>
void rc_abstract_exportal<IF>::bind_static_object(
    const rc_object_handle& obj_to_bind)
{
    if (obj_to_bind.is_export()) {
        sc_export<IF>* export_to_bind =
            dynamic_cast<sc_export<IF>*>(*obj_to_bind);
        if (export_to_bind != NULL) {
            (*export_to_bind).bind(this->static_export());
            return;
        }
    } else if (obj_to_bind.is_port()) {
        sc_port<IF>* port_to_bind =
            dynamic_cast<sc_port<IF>*>(*obj_to_bind);
        if (port_to_bind != NULL) {
            (*port_to_bind).bind(this->static_export());
            return;
        }
    }
    RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
        "'" << obj_to_bind->name() << "'"
        " is incompatible with the static export"
        " (in exportal '" << this->name() << "')");
}

template<class IF>
void rc_abstract_exportal<IF>::bind_dynamic_object(
    const rc_object_handle& obj_to_bind)
{
    if (obj_to_bind.is_export()) {
        sc_export<IF>* export_to_bind =
            dynamic_cast<sc_export<IF>*>(*obj_to_bind);
        if (export_to_bind != NULL) {
            this->bind_dynamic((sc_export<IF>&)(*export_to_bind));
            return;
        }
    } else if (obj_to_bind.is_channel()) {
        IF* if_to_bind = dynamic_cast<IF*>(*obj_to_bind);
        if (if_to_bind != NULL) {
            this->bind_dynamic(*if_to_bind);
            return;
        }
    }
    RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
        "'" << obj_to_bind->name() << "'"
        " is incompatible with the dynamic export"
        " (in exportal '" << this->name() << "')");
}

template<class IF>
rc_interface_wrapper<IF>&
rc_abstract_exportal<IF>::bind_exclusively(IF& dynamic_if_)
{
    std::auto_ptr<interface_wrapper> wrapper(
        new interface_wrapper(dynamic_if_, *this));
    base_type::bind_exclusively(*(wrapper.get()));
    return *(wrapper.release());
}

template<class IF>
sc_export<IF>& rc_abstract_exportal<IF>::static_export()
{
    if (p_static_export == NULL) {
        p_static_export = new sc_export<IF>("static_export");
        // (sc_export object will be deleted by SystemC)
    }
    if (p_static_export->get_interface() == NULL) {
        p_static_export->bind(this->get_static_accessor());
    }
    return *p_static_export;
}

template<class IF>
void rc_abstract_exportal<IF>::register_reconfigurable(
        rc_reconfigurable& module, sc_interface& dyn_if)
{
    base_type::register_reconfigurable(module, dyn_if);
    switch_parent_type::register_reconfigurable(module, dyn_if);
}

template<class IF>
void rc_abstract_exportal<IF>::unregister_reconfigurable(
        rc_reconfigurable& module)
{
    base_type::unregister_reconfigurable(module);
    switch_parent_type::unregister_reconfigurable(module);
}

template<class IF>
void rc_abstract_exportal<IF>::add_event_forwarder(
    event_getter_type event_getter,
    const std::string& event_name)
{
    std::string _event_name(event_name);
    if (_event_name.empty()) {
        _event_name = sc_gen_unique_name("event");
    }
    p_ef_decl_vector.push_back(ef_decl_type(_event_name, event_getter));
}

template<class IF>
rc_interface_wrapper<IF>*
rc_abstract_exportal<IF>::get_interface_wrapper() const
{
    return dynamic_cast<rc_interface_wrapper<IF>*>(
        base_type::get_interface_wrapper_base());
}

template<class IF>
IF* rc_abstract_exportal<IF>::get_interface() const
{
    rc_interface_wrapper_base* wrapper =
        base_type::get_interface_wrapper_base();
    if (wrapper != NULL) {
        return dynamic_cast<IF*>(&wrapper->get_wrapped_interface());
    } else {
        return NULL;
    }
}

template<class IF>
rc_interface_wrapper_base*
rc_abstract_exportal<IF>::create_interface_wrapper(
    sc_interface& wrapped_if)
{
    IF* wrapped_if_ = dynamic_cast<IF*>(&wrapped_if);
    if (wrapped_if_ != NULL) {
        return new interface_wrapper(*wrapped_if_, *this);
    } else {
        return NULL;
    }
}

template<class IF>
typename rc_abstract_exportal<IF>::accessor_type&
rc_abstract_exportal<IF>::get_static_accessor() const
{
    if (p_static_accessor == NULL) {
        p_static_accessor = this->create_accessor();
    }
    return *p_static_accessor;
}

template<class IF>
void rc_abstract_exportal<IF>::before_end_of_elaboration()
{
    base_type::before_end_of_elaboration();

    // ensure existence of static_export and its binding
    this->static_export();
}

template<class IF>
rc_abstract_exportal<IF>::interface_wrapper::interface_wrapper(
    IF& wrapped_if, exportal_type& owner_exportal)
    : base_type(wrapped_if), p_exportal(&owner_exportal)
{
    for (typename ef_decl_vector::iterator it =
            owner_exportal.p_ef_decl_vector.begin();
        it != owner_exportal.p_ef_decl_vector.end();
        ++it)
    {
        this->add_event_forwarder(
            wrapped_if, it->second, it->first.c_str());
    }
}

template<class IF>
void rc_abstract_exportal<IF>::_rc_set_interface_wrapper_owner(
        rc_interface_wrapper_base* wrapper)
{
    interface_wrapper* wrapper_ =
        dynamic_cast<interface_wrapper*>(wrapper);
    if (wrapper_ != NULL) {
        wrapper_->set_owner(*this);
    }
}

} // namespace ReChannel

#endif // RC_ABSTRACT_EXPORTAL_H_
//
// $Id: rc_abstract_exportal.h,v 1.20 2008/01/01 13:46:06 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/exportals/rc_abstract_exportal.h,v $
//
