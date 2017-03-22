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
 * @file    rc_rv_portals.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Defines portals for the use with SystemC's rv ports/channels.
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.10 $
 */

#ifndef RC_RV_PORTALS_H_
#define RC_RV_PORTALS_H_

#include "ReChannel/communication/portals/rc_portal.h"
#include "ReChannel/communication/accessors/rc_signal_accessors.h"

namespace ReChannel {

/**
 * @brief Portal for an resolved vector signal input port.
 */
template<int W>
RC_PORTAL_TEMPLATE(sc_in_rv<W>)
{
    RC_PORTAL_TEMPLATE_CTOR(sc_in_rv<W>)
    {
        RC_PORTAL_FORWARD_EVENT(value_changed_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
    }
};

/**
 * @brief Portal for an resolved vector signal input/output port.
 */
template<int W>
RC_PORTAL_TEMPLATE(sc_inout_rv<W>)
{
    RC_PORTAL_TEMPLATE_CTOR(sc_inout_rv<W>)
    {
        RC_PORTAL_FORWARD_EVENT(value_changed_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_CLOSE()
    {
        // write 'Z..Z' to the static channel (for each registered driver)
        rc_interface_wrapper<if_type>& ifw = this->get_interface_wrapper();
        const int drv_count = ifw.get_nb_driver_count();
        for (int i=0; i < drv_count; i++) {
            ifw.get_nb_driver_access(i)->call(
                &if_type::write, rc_cref(s_ZZZ));
        }
    }

    RC_ON_UNDEF()
    {
        // write 'X..X' to the static channel (disable other drivers first)
        rc_interface_wrapper<if_type>& ifw = this->get_interface_wrapper();
        const int drv_count = ifw.get_nb_driver_count();
        for (int i=1; i < drv_count; i++) {
            ifw.get_nb_driver_access(i)->call(
                &if_type::write, rc_cref(s_ZZZ));
        }
        ifw.get_nb_driver_access(0)->call(&if_type::write, rc_cref(s_XXX));
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
    }

private:
    static const sc_dt::sc_lv<W> s_ZZZ;
    static const sc_dt::sc_lv<W> s_XXX;
};

template<int W>
const typename sc_dt::sc_lv<W>
rc_portal<sc_inout_rv<W> >::s_ZZZ = sc_lv<W>('Z');

template<int W>
const typename sc_dt::sc_lv<W>
rc_portal<sc_inout_rv<W> >::s_XXX = sc_lv<W>('X');

/**
 * @brief Portal for an resolved vector signal output port.
 */
template<int W>
class rc_portal<sc_out_rv<W> >
    : public rc_portal<sc_inout_rv<W> >
{
    typedef rc_portal<sc_inout_rv<W> > base_type;
public:
    typedef typename base_type::port_type     port_type;
    typedef typename base_type::if_type       if_type;
    typedef typename base_type::accessor_type accessor_type;

    rc_portal<sc_out_rv<W> >(
        const sc_module_name& module_name =
            sc_gen_unique_name("rc_portal"))
        : base_type(module_name)
    { }
};

/* named portal specializations */

/**
 * @brief Portal for an resolved vector signal input port (named).
 */
template<int W>
class rc_in_rv_portal
    : public rc_portal<sc_in_rv<W> >
{
    typedef rc_portal<sc_in_rv<W> > base_type;
public:
    typedef typename base_type::port_type     port_type;
    typedef typename base_type::if_type       if_type;
    typedef typename base_type::accessor_type accessor_type;

    explicit rc_in_rv_portal(
        const sc_module_name& name_ =
            sc_gen_unique_name("rc_in_rv_portal"))
        : base_type(name_)
    { }
};

/**
 * @brief Portal for an resolved vector signal input/output port (named).
 */
template<int W>
class rc_inout_rv_portal
    : public rc_portal<sc_inout_rv<W> >
{
    typedef rc_portal<sc_inout_rv<W> > base_type;
public:
    typedef typename base_type::port_type     port_type;
    typedef typename base_type::if_type       if_type;
    typedef typename base_type::accessor_type accessor_type;

    explicit rc_inout_rv_portal(
        const sc_module_name& name_ =
            sc_gen_unique_name("rc_inout_rv_portal"))
        : base_type(name_)
    { }
};

/**
 * @brief Portal for an resolved vector signal output port (named).
 */
template<int W>
class rc_out_rv_portal
    : public rc_portal<sc_out_rv<W> >
{
    typedef rc_portal<sc_out_rv<W> > base_type;
public:
    typedef typename base_type::port_type     port_type;
    typedef typename base_type::if_type       if_type;
    typedef typename base_type::accessor_type accessor_type;

    explicit rc_out_rv_portal(
        const sc_module_name& name_ =
            sc_gen_unique_name("rc_out_rv_portal"))
        : base_type(name_)
    { }
};

} // namespace ReChannel

#endif // RC_RV_PORTALS_H_
//
// $Id: rc_rv_portals.h,v 1.10 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/portals/rc_rv_portals.h,v $
//
