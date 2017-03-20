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
 * @file    rc_interface_wrapper.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2008/01/01 13:48:36 $
 * $Revision: 1.13 $
 */

#ifndef RC_INTERFACE_WRAPPER_H_
#define RC_INTERFACE_WRAPPER_H_

#include "ReChannel/core/rc_interface_filter.h"
#include "ReChannel/core/rc_process.h"
#include "ReChannel/util/rc_driver_object.h"
#include "ReChannel/util/rc_member_func.h"
#include "ReChannel/util/rc_port_handle.h"

#include "ReChannel/communication/rc_event_forwarder.h"

namespace ReChannel {

/**
 * @brief Base ABI of rc_interface_wrapper<IF>.
 */
class rc_interface_wrapper_base
{
public:
    class accessor_base;

protected:
    class access_base;

public:
    virtual std::string get_interface_wrapper_name() const = 0;

    virtual void register_port(
        sc_port_base& port_, const char* if_typename) = 0;

    virtual sc_interface& get_wrapped_interface() const = 0;

    virtual rc_event_forwarder_host* get_event_forwarder_host() = 0;

    virtual ~rc_interface_wrapper_base() {}

protected:
    virtual bool set_wrapped_interface(sc_interface& if_) = 0;

    virtual rc_process_behavior_change begin_access_callback() = 0;

    virtual void end_access_callback() = 0;
};

/**
 * @brief Base class of a temporary interface access object.
 */
class rc_interface_wrapper_base::access_base
{
public:
    access_base() { }

protected:
    inline rc_process_behavior_change
        begin_access(rc_interface_wrapper_base& wrapper) const
        { return wrapper.begin_access_callback(); }

    inline void end_access(rc_interface_wrapper_base& wrapper) const
        { wrapper.end_access_callback(); }
};

/**
 * @brief Base ABI of an accessor.
 */
class rc_interface_wrapper_base::accessor_base
    : virtual public rc_interface_filter,
      virtual protected rc_event_filter
{
public:
    virtual rc_interface_wrapper_base*
        rc_get_target_wrapper_base() const = 0;

    virtual bool rc_set_target(rc_interface_wrapper_base& target) = 0;

    virtual sc_interface* rc_get_target_interface() const = 0;

    virtual bool rc_set_target(sc_interface& target) = 0;

    virtual bool rc_set_target(
        sc_interface& target, const rc_process_control& pctrl) = 0;

    virtual void rc_clear_target() = 0;

    virtual int rc_get_bound_port_count() const = 0;

    virtual sc_port_base* rc_get_bound_port(int index) const = 0;

    virtual rc_event_filter* rc_get_event_filter() = 0;

protected:
    virtual bool rc_on_event(const sc_event& e) = 0;

    virtual void rc_set_event_trigger(
        const sc_event& e, rc_event_trigger& t) = 0;

    virtual void rc_clear_event_trigger(const sc_event& e) = 0;
};

/**
 * @brief ABI of an interface wrapper.
 */
template<class IF>
class rc_interface_wrapper
    : virtual public rc_interface_wrapper_base
{
    RC_STATIC_ASSERT_VALID_INTERFACE(IF);

public:
    class accessor;

protected:
    class nb_interface_access;
    class interface_access;
    class nb_driver_access;
    class driver_access;

public:
    virtual interface_access get_interface_access() = 0;

    virtual nb_interface_access get_nb_interface_access() = 0;

    virtual driver_access get_driver_access(int driver_index) = 0;

    virtual nb_driver_access get_nb_driver_access(int driver_index) = 0;

    virtual accessor* create_accessor() const = 0;

    virtual int get_driver_count() const = 0;

    virtual int get_nb_driver_count() const = 0;

protected:
    virtual bool set_wrapped_interface(IF& if_) = 0;

    virtual IF* nb_interface_access_callback() = 0;

    virtual IF* interface_access_callback() = 0;

    virtual typename rc_nb_driver_object<IF>::driver_access*
        nb_driver_access_callback(int driver_index) = 0;

    virtual typename rc_driver_object<IF>::driver_access*
        driver_access_callback(int driver_index) = 0;
};

/**
 * @brief ABI of an accessor.
 */
template<class IF>
class rc_interface_wrapper<IF>::accessor
    : virtual public rc_interface_wrapper_base::accessor_base,
      virtual public IF
{
    RC_STATIC_ASSERT_VALID_INTERFACE(IF);

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
    typedef rc_interface_wrapper<IF> interface_wrapper;

public:
    virtual interface_wrapper* rc_get_target_wrapper() const = 0;

    virtual bool rc_set_target(interface_wrapper& target) = 0;

    virtual bool rc_set_target(IF& target) = 0;

    virtual bool rc_set_target(
        IF& target, const rc_process_control& pctrl) = 0;
};

/**
 * @brief Temporary object for a non-blocking interface access.
 */
template<class IF>
class rc_interface_wrapper<IF>::nb_interface_access
    : public rc_interface_wrapper_base::access_base
{
public:
    inline explicit nb_interface_access(rc_interface_wrapper<IF>& wrapper)
        : p_wrapper(wrapper)
    { }

    inline IF* operator->() const
        { return p_wrapper.nb_interface_access_callback(); }

    template<class METHOD_PTR>
    inline rc_member_func<METHOD_PTR> operator->*(METHOD_PTR mp) const
    {
        return rc_member_func<METHOD_PTR>(
            *p_wrapper.nb_interface_access_callback(), mp);
    }

private:
    rc_interface_wrapper<IF>& p_wrapper;
};

/**
 * @brief Temporary object for a blocking interface access.
 */
template<class IF>
class rc_interface_wrapper<IF>::interface_access
    : public rc_interface_wrapper_base::access_base
{
public:
    inline explicit interface_access(rc_interface_wrapper<IF>& wrapper)
        : p_wrapper(wrapper),
          p_process_behavior_change(begin_access(wrapper))
    { }

    inline IF* operator->() const
        { return p_wrapper.interface_access_callback(); }

    template<class METHOD_PTR>
    inline
    rc_member_func<METHOD_PTR> operator->*(METHOD_PTR mp) const
    {
        return rc_member_func<METHOD_PTR>(
            *p_wrapper.interface_access_callback(), mp);
    }

    inline ~interface_access()
        { end_access(p_wrapper); }

private:
    rc_interface_wrapper<IF>&        p_wrapper;
    const rc_process_behavior_change p_process_behavior_change;
};

/**
 * @brief Temporary object for a non-blocking driver access.
 */
template<class IF>
class rc_interface_wrapper<IF>::nb_driver_access
    : public rc_interface_wrapper_base::access_base
{
public:
    inline nb_driver_access(
        rc_interface_wrapper<IF>& wrapper, int driver_index)
        : p_wrapper(wrapper), p_driver_index(driver_index)
    { }

    inline
    typename rc_nb_driver_object<IF>::driver_access* operator->() const
        { return p_wrapper.nb_driver_access_callback(p_driver_index); }

private:
    rc_interface_wrapper<IF>& p_wrapper;
    const int                 p_driver_index;
};

/**
 * @brief Temporary object for a blocking driver access.
 */
template<class IF>
class rc_interface_wrapper<IF>::driver_access
    : public rc_interface_wrapper_base::access_base
{
public:
    inline driver_access(
        rc_interface_wrapper<IF>& wrapper, int driver_index)
        : p_wrapper(wrapper), p_driver_index(driver_index),
          p_process_behavior_change(begin_access(wrapper))
    { }

    inline
    typename rc_driver_object<IF>::driver_access* operator->() const
        { return p_wrapper.driver_access_callback(p_driver_index); }

    inline ~driver_access()
        { end_access(p_wrapper); }

private:
    rc_interface_wrapper<IF>&        p_wrapper;
    const int                        p_driver_index;
    const rc_process_behavior_change p_process_behavior_change;
};

} // namespace ReChannel

#endif //RC_INTERFACE_WRAPPER_H_

//
// $Id: rc_interface_wrapper.h,v 1.13 2008/01/01 13:48:36 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/rc_interface_wrapper.h,v $
//

