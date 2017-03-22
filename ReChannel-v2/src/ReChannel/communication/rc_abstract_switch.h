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
 * @file    rc_abstract_switch.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:25:55 $
 * $Revision: 1.10 $
 */

#ifndef RC_ABSTRACT_SWITCH_H_
#define RC_ABSTRACT_SWITCH_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_switch.h"
#include "ReChannel/communication/rc_fallback_interface.h"
#include "ReChannel/communication/rc_event_forwarder.h"

namespace ReChannel {

class rc_reconfigurable;

template<class IF> class rc_abstract_switch;

/**
 * @brief Base class of rc_abstract_switch<IF>.
 */
class rc_abstract_switch_b
    : virtual public rc_switch
{
    template<class IF> friend class rc_abstract_switch;

private:
    typedef std::set<void*>                     set;
    typedef std::map<rc_reconfigurable*, void*> module_void_map;
public:
    typedef rc_switch::state_type state_type;

private:
    rc_abstract_switch_b();

public:
    virtual state_type get_switch_state() const
        { return m_state; }

    virtual unsigned int get_transaction_count() const
        { return m_transaction_count; }

    virtual bool is_registered(const rc_reconfigurable& module) const
    {
        return (p_mod_if_map.find(const_cast<rc_reconfigurable*>(&module))
            != p_mod_if_map.end());
    }

    virtual bool is_locked() const
        { return (p_lock_owner != NULL); }

    virtual std::string get_switch_kind() const
        { return "switch"; }

    virtual std::string get_switch_name() const
        { return "(unknown)"; };

    virtual ~rc_abstract_switch_b();

protected:

    virtual void register_reconfigurable(
        rc_reconfigurable& module, sc_interface& dyn_if);

    virtual void unregister_reconfigurable(rc_reconfigurable& module);

    virtual bool is_lock_owner(const rc_reconfigurable& reconf) const
        { return (p_lock_owner == &reconf); }

    virtual bool set_locked(rc_reconfigurable& lock_owner, bool lock);

private:

    inline void* find_registered_if(rc_reconfigurable* module) const;

    inline bool is_registered(void* dyn_if) const;

private:

/* special (untyped!!!) callback methods */

    virtual void* _rc_dynamic_cast(sc_interface* dyn_if) const = 0;

    virtual const char* _rc_get_if_type() const = 0;

protected:
    state_type   m_state;
    unsigned int m_transaction_count;

private:

/* for internal use only! */

    rc_reconfigurable* p_lock_owner;
    set                p_interface_set;
    module_void_map    p_mod_if_map;

private:
    // disabled
    rc_abstract_switch_b(const rc_abstract_switch_b& orig);
    rc_abstract_switch_b& operator=(const rc_abstract_switch_b& orig);
};

/**
 * @brief Abstract switch class.
 */
template<class IF>
class rc_abstract_switch
    : public rc_abstract_switch_b
{
    RC_STATIC_ASSERT_VALID_INTERFACE(IF);

private:
    typedef rc_abstract_switch_b base_type;

protected:
    rc_abstract_switch() { }

public:

    virtual bool is_registered(const sc_interface& dyn_if) const;

protected:

    inline IF* get_registered_if(rc_reconfigurable& module) const;

    virtual sc_interface* get_registered_interface(
        rc_reconfigurable& module) const
        { return get_registered_if(module); }

private:

/* for internal use only */

    virtual void* _rc_dynamic_cast(sc_interface* dyn_if) const;

    virtual const char* _rc_get_if_type() const;

private:
    // disabled
    rc_abstract_switch(const rc_abstract_switch& orig);
    rc_abstract_switch& operator=(const rc_abstract_switch& orig);
};

/* inline code */

inline
void* rc_abstract_switch_b::find_registered_if(
    rc_reconfigurable* module) const
{
    module_void_map::const_iterator it = p_mod_if_map.find(module);
    return (it != p_mod_if_map.end() ? it->second : NULL);
}

inline
bool rc_abstract_switch_b::is_registered(void* dyn_if) const
{
    return (p_interface_set.find(dyn_if) != p_interface_set.end());
}

/* template code */

template<class IF>
inline
bool rc_abstract_switch<IF>::is_registered(
    const sc_interface& dyn_if) const
{
    return base_type::is_registered(
        dynamic_cast<IF*>(const_cast<sc_interface*>(&dyn_if)));
}

template<class IF>
inline
IF* rc_abstract_switch<IF>::get_registered_if(
    rc_reconfigurable& module) const
{
    return reinterpret_cast<IF*>(base_type::find_registered_if(&module));
}

template<class IF>
void* rc_abstract_switch<IF>::_rc_dynamic_cast(sc_interface* dyn_if) const
{
    return dynamic_cast<IF*>(dyn_if);
}

template<class IF>
const char* rc_abstract_switch<IF>::_rc_get_if_type() const
{
    return typeid(IF).name();
}

} // namespace ReChannel

/* macros to declare callback methods for opening and closing switches */

#define RC_ON_OPEN() \
    virtual void rc_on_open()

#define RC_ON_CLOSE() \
    virtual void rc_on_close()

#define RC_ON_UNDEF() \
    virtual void rc_on_undef()

#define RC_ON_REFRESH_NOTIFY() \
    virtual void rc_on_refresh_notify()

#endif // RC_ABSTRACT_SWITCH_H_

//
// $Id: rc_abstract_switch.h,v 1.10 2007/12/20 20:25:55 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/rc_abstract_switch.h,v $
//
