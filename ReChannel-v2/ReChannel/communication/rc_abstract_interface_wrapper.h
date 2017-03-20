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
 * @file    rc_abstract_interface_wrapper.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2008/01/01 13:46:06 $
 * $Revision: 1.17 $
 */

#ifndef RC_ABSTRACT_INTERFACE_WRAPPER_H_
#define RC_ABSTRACT_INTERFACE_WRAPPER_H_

#include "ReChannel/communication/rc_interface_wrapper.h"
#include "ReChannel/communication/rc_fallback_interface.h"
#include "ReChannel/communication/rc_report_ids.h"

#ifndef RC_INTERFACE_WRAPPER_MAX_DRIVER_COUNT
 #define RC_INTERFACE_WRAPPER_MAX_DRIVER_COUNT 32
#endif

namespace ReChannel {

template<class IF> class rc_abstract_interface_wrapper;

/**
 * @brief Base class of rc_abstract_interface_wrapper<IF>.
 */
class rc_abstract_interface_wrapper_b
    : virtual public rc_interface_wrapper_base,
      virtual protected rc_event_forwarder_host
{
    template<class IF> friend class rc_abstract_interface_wrapper;

private:
    typedef std::vector<void*> vector;
    typedef std::vector<std::string> string_vector;
    typedef std::map<std::string, rc_event_forwarder_base*>
        event_forwarder_base_map;
    typedef std::vector<rc_event_forwarder_base*> ef_vector;
    typedef std::pair<rc_event_forwarder_base*, ef_vector>
        event_ef_map_entry;
    typedef std::map<const sc_event*, event_ef_map_entry> event_ef_map;

private:
    explicit rc_abstract_interface_wrapper_b(void* if_);

public:

    virtual std::string get_interface_wrapper_name() const
        { return "(unknown)"; }

    virtual rc_event_forwarder_host* get_event_forwarder_host()
        { return static_cast<rc_event_forwarder_host*>(this); }

protected:

    /** @brief directly adds an event forwarder being sensitive to an event */
    virtual void add_event_forwarder(
        rc_event_forwarder_base& forwarder, const sc_event& e,
        const std::string& event_name=NULL);

    /** @brief adds a target interface to all event forwarders */
    virtual void add_event_forwarder_target(
        sc_interface& if_, rc_event_filter* filter=NULL);

    /** @brief adds a target interface to the specified event forwarder */
    virtual bool add_event_forwarder_target(
        const std::string& event_name, sc_interface& if_,
        rc_event_filter* filter=NULL);

    /** @brief returns the number of added event forwarder targets */
    virtual bool get_event_forwarder_target_count() const
        { return p_event_forwarder_base_map.size(); }

    /** @brief clears the target lists of all event forwarders */
    virtual void clear_event_forwarder_targets();

    /** @brief clears the target lists of the specified event forwarder */
    virtual void clear_event_forwarder_targets(
        const std::string& event_name);

    /** @brief returns the names of all added event forwarders */
    virtual string_vector get_event_forwarder_names() const;

    /** @brief triggers the notification of the specified dynamic event. */
    virtual void notify_event(const std::string& event_name);

    virtual void register_port(
        sc_port_base& port_, const char* if_typename)
    { this->get_wrapped_interface().register_port(port_, if_typename); }

    virtual rc_process_behavior_change begin_access_callback()
        { return rc_process_behavior_change(); }

    virtual void end_access_callback() { };

    inline int get_driver_count() const
        { return p_drv_obj_vector.size(); }

    inline int get_nb_driver_count() const
        { return p_nb_drv_obj_vector.size(); }

    virtual ~rc_abstract_interface_wrapper_b();

private:

    inline void* get_wrapped_if() const
        { return p_interface; }

    void set_wrapped_if(void* if_);

    /** @brief finds a blocking driver by its index */
    void* find_driver_object(int index);

    /** @brief finds a blocking driver by its index */
    inline void* find_nb_driver_object(int index);

    void* create_driver_object(int index);

    void* create_nb_driver_object(int index);

    void delete_all_drivers();

    void delete_all_event_forwarders();

    void missing_fallback_if_error() const;

private:

/* for internal use only */

    rc_event_forwarder_base& _rc_get_event_forwarder(
        const std::string& event_name);

    /** @brief spawns a new event forwarder process */
    void _rc_spawn_event_forwarder_process(
        rc_event_forwarder_base& forwarder, const sc_event& e);

    /** @brief event forwarder process (static) */
    static void _rc_event_forwarder_proc(
        event_ef_map_entry& map_entry);

/* special (untyped!!!) callback methods */

    virtual void* _rc_create_driver_object(void* if_) const = 0;

    virtual void* _rc_create_nb_driver_object(void* if_) const = 0;

    virtual void _rc_set_driver_channel(
        void* drv_obj, void* if_) const = 0;

    virtual void _rc_set_nb_driver_channel(
        void* nb_drv_obj, void* if_) const = 0;

    virtual void _rc_delete_driver_object(void* drv_obj) const = 0;

    virtual void _rc_delete_nb_driver_object(void* nb_drv_obj) const = 0;

private:

    /** @brief map: event name -> event forwarder */
    event_forwarder_base_map p_event_forwarder_base_map;

private:

    /** @brief map: event -> associated event forwarders */
    static event_ef_map s_event_ef_map;

private:

/* for internal use only! */

    void*  p_interface;
    vector p_drv_obj_vector;
    vector p_nb_drv_obj_vector;

private:
    // disabled
    rc_abstract_interface_wrapper_b(
        const rc_abstract_interface_wrapper_b& orig) { }
    rc_abstract_interface_wrapper_b& operator=(
        const rc_abstract_interface_wrapper_b& orig);
};

/**
 * @brief Abstract interface wrapper class.
 */
template<class IF>
class rc_abstract_interface_wrapper
    : public rc_abstract_interface_wrapper_b,
      virtual public rc_interface_wrapper<IF>
{
    RC_STATIC_ASSERT_VALID_INTERFACE(IF);

private:
    typedef rc_abstract_interface_wrapper_b base_type;
    typedef rc_driver_object<IF>            drv_obj_type;
    typedef rc_nb_driver_object<IF>         nb_drv_obj_type;

protected:
    typedef typename rc_interface_wrapper<IF>::interface_access
        interface_access;
    typedef typename rc_interface_wrapper<IF>::nb_interface_access
        nb_interface_access;
    typedef typename rc_interface_wrapper<IF>::driver_access
        driver_access;
    typedef typename rc_interface_wrapper<IF>::nb_driver_access
        nb_driver_access;

public:
    typedef rc_interface_wrapper<IF> interface_wrapper_type;
    typedef typename rc_interface_wrapper<IF>::accessor accessor_type;

protected:
    rc_abstract_interface_wrapper();

    explicit rc_abstract_interface_wrapper(IF& if_);

public:

    virtual interface_access get_interface_access()
        { return interface_access(*this); }

    virtual nb_interface_access get_nb_interface_access()
        { return nb_interface_access(*this); }

    virtual driver_access get_driver_access(int driver_index)
        { return driver_access(*this, driver_index); }

    virtual nb_driver_access get_nb_driver_access(int driver_index)
        { return nb_driver_access(*this, driver_index); }

    virtual int get_driver_count() const
        { return base_type::get_driver_count(); }

    virtual int get_nb_driver_count() const
        { return base_type::get_nb_driver_count(); }

    inline virtual sc_interface& get_wrapped_interface() const
        { return this->get_wrapped_if(); }

protected:

    inline IF& get_wrapped_if() const
    { return *(reinterpret_cast<IF*>(base_type::get_wrapped_if())); }

    virtual bool set_wrapped_interface(sc_interface& if_);

    inline virtual bool set_wrapped_interface(IF& if_)
        { base_type::set_wrapped_if(&if_); return true; }

    virtual IF& rc_get_fallback_if() const;

    virtual IF* nb_interface_access_callback()
    { return reinterpret_cast<IF*>(base_type::get_wrapped_if()); }

    virtual IF* interface_access_callback()
    { return reinterpret_cast<IF*>(base_type::get_wrapped_if()); }

    virtual typename rc_nb_driver_object<IF>::driver_access*
        nb_driver_access_callback(int driver_index)
        { return &(this->get_nb_driver_object(driver_index).access); }

    virtual typename rc_driver_object<IF>::driver_access*
        driver_access_callback(int driver_index)
        { return &(this->get_driver_object(driver_index).access); }

    /** @brief returns a (possibly newly created) blocking driver by its index */
    inline rc_driver_object<IF>& get_driver_object(int index);

    /** @brief returns a (possibly newly created) non-blocking driver by its index */
    inline rc_nb_driver_object<IF>& get_nb_driver_object(int index);

    /** @brief creates an event forwarder for the specified event getter method */
    rc_event_forwarder<IF>& add_event_forwarder(
        const sc_event& source_event,
        boost::function<const sc_event& (IF*)> event_getter,
        const std::string& event_name=NULL);

    /** @brief creates an event forwarder for the specified event getter method */
    rc_event_forwarder<IF>& add_event_forwarder(
        IF& trigger_if,
        boost::function<const sc_event& (IF*)> event_getter,
        const std::string& event_name=NULL);

    virtual ~rc_abstract_interface_wrapper();

private:

/* (for internal use only) */

    virtual void* _rc_create_driver_object(void* if_) const;

    virtual void* _rc_create_nb_driver_object(void* if_) const;

    virtual void _rc_set_driver_channel(void* drv_obj, void* if_) const;

    virtual void _rc_set_nb_driver_channel(void* nb_drv_obj, void* if_) const;

    virtual void _rc_delete_driver_object(void* drv_obj) const;

    virtual void _rc_delete_nb_driver_object(void* nb_drv_obj) const;
};

/* inline code */

inline
void* rc_abstract_interface_wrapper_b::find_driver_object(int index)
{
    int size = (int)p_drv_obj_vector.size();
    if (size > 0 && index >= 0 && index < size) {
         return p_drv_obj_vector[index];
    } else {
        return NULL;
    }
}

inline
void* rc_abstract_interface_wrapper_b::find_nb_driver_object(int index)
{
    int size = (int)p_nb_drv_obj_vector.size();
    if (size > 0 && index >= 0 && index < size) {
         return p_nb_drv_obj_vector[index];
    } else {
        return NULL;
    }
}

/* template code */

template<class IF>
rc_abstract_interface_wrapper<IF>::rc_abstract_interface_wrapper()
    : base_type(&rc_get_fallback_if())
{ }

template<class IF>
rc_abstract_interface_wrapper<IF>::rc_abstract_interface_wrapper(
    IF& if_)
    : base_type(&if_)
{ }

template<class IF>
rc_abstract_interface_wrapper<IF>::~rc_abstract_interface_wrapper()
{
    this->delete_all_drivers();
    this->delete_all_event_forwarders();
}

template<class IF>
rc_event_forwarder<IF>&
rc_abstract_interface_wrapper<IF>::add_event_forwarder(
    const sc_event& source_event,
    boost::function<const sc_event& (IF*)> event_getter,
    const std::string& event_name)
{
    std::auto_ptr<rc_event_forwarder<IF> > forwarder(
        new rc_event_forwarder<IF>(event_getter));
    base_type::add_event_forwarder(
        *forwarder, source_event, event_name);
    return *(forwarder.release());
}

template<class IF>
rc_event_forwarder<IF>&
rc_abstract_interface_wrapper<IF>::add_event_forwarder(
    IF& trigger_if,
    boost::function<const sc_event& (IF*)> event_getter,
    const std::string& event_name)
{
    const sc_event& source_event = event_getter(&trigger_if);
    return this->add_event_forwarder(
        source_event, event_getter, event_name);
}

template<class IF>
bool rc_abstract_interface_wrapper<IF>::set_wrapped_interface(
    sc_interface& if_)
{
    IF* if__ = dynamic_cast<IF*>(&if_);
    if (if__ == NULL) {
        return false;
    } else {
        base_type::set_wrapped_if(if__);
        return true;
    }
}

template<class IF>
IF& rc_abstract_interface_wrapper<IF>::rc_get_fallback_if() const
{
    static IF* s_fallback_if =
        rc_fallback_interface_factory<IF>::create();

    if (s_fallback_if != NULL) {
        return *s_fallback_if;
    } else {
        base_type::missing_fallback_if_error();
        throw 0;
    }
}

template<class IF>
inline
rc_driver_object<IF>&
rc_abstract_interface_wrapper<IF>::get_driver_object(int index)
{
    void* drv = base_type::find_driver_object(index);
    if (drv == NULL) {
        drv = base_type::create_driver_object(index);
    }
    return *(reinterpret_cast<drv_obj_type*>(drv));
}

template<class IF>
inline
rc_nb_driver_object<IF>&
rc_abstract_interface_wrapper<IF>::get_nb_driver_object(int index)
{
    void* drv = base_type::find_nb_driver_object(index);
    if (drv == NULL) {
        drv = base_type::create_nb_driver_object(index);
    }
    return *(reinterpret_cast<nb_drv_obj_type*>(drv));
}

template<class IF>
void* rc_abstract_interface_wrapper<IF>::_rc_create_driver_object(
    void* if_) const
{
    IF* if__ = reinterpret_cast<IF*>(if_);
    return new drv_obj_type(*if__);
}

template<class IF>
void* rc_abstract_interface_wrapper<IF>::_rc_create_nb_driver_object(
    void* if_) const
{
    IF* if__ = reinterpret_cast<IF*>(if_);
    return new nb_drv_obj_type(*if__);
}

template<class IF>
void rc_abstract_interface_wrapper<IF>::_rc_set_driver_channel(
    void* drv_obj, void* if_) const
{
    IF* if__ = reinterpret_cast<IF*>(if_);
    drv_obj_type* drv_obj__ = reinterpret_cast<drv_obj_type*>(drv_obj);
    drv_obj__->set_channel(*if__);
}

template<class IF>
void rc_abstract_interface_wrapper<IF>::_rc_set_nb_driver_channel(
    void* nb_drv_obj, void* if_) const
{
    IF* if__ = reinterpret_cast<IF*>(if_);
    nb_drv_obj_type* nb_drv_obj__ =
        reinterpret_cast<nb_drv_obj_type*>(nb_drv_obj);
    nb_drv_obj__->set_channel(*if__);
}

template<class IF>
void rc_abstract_interface_wrapper<IF>::_rc_delete_driver_object(
    void* drv_obj) const
{
    drv_obj_type* drv_obj__ =
        reinterpret_cast<drv_obj_type*>(drv_obj);
    delete drv_obj__;
}

template<class IF>
void rc_abstract_interface_wrapper<IF>::_rc_delete_nb_driver_object(
    void* nb_drv_obj) const
{
    nb_drv_obj_type* nb_drv_obj__ =
        reinterpret_cast<nb_drv_obj_type*>(nb_drv_obj);
    delete nb_drv_obj__;
}

} // namespace ReChannel

#endif // RC_ABSTRACT_SWITCH_H_

//
// $Id: rc_abstract_interface_wrapper.h,v 1.17 2008/01/01 13:46:06 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/rc_abstract_interface_wrapper.h,v $
//
