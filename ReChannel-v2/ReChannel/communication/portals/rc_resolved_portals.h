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
 * @file    rc_resolved_portals.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Defines portals for the use with SystemC's resolved ports/channels.
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.11 $
 */

#ifndef RC_RESOLVED_PORTALS_H_
#define RC_RESOLVED_PORTALS_H_

#include "ReChannel/communication/portals/rc_portal.h"
#include "ReChannel/communication/accessors/rc_signal_accessors.h"

namespace ReChannel {

/**
 * @brief Portal for a resolved signal input port.
 */
RC_PORTAL(sc_in_resolved)
{
    RC_PORTAL_CTOR(sc_in_resolved)
    {
        RC_PORTAL_FORWARD_EVENT(value_changed_event);
        RC_PORTAL_FORWARD_EVENT(posedge_event);
        RC_PORTAL_FORWARD_EVENT(negedge_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_REFRESH_NOTIFY()
    {
        const sc_logic value = this->get_static_port().read();
        this->notify_event("value_changed_event");
        if (value == SC_LOGIC_1) {
            this->notify_event("posedge_event");
        }
    }
};

/**
 * @brief Portal for a resolved signal input/output port.
 */
RC_PORTAL(sc_inout_resolved)
{
    RC_PORTAL_CTOR(sc_inout_resolved)
    {
        RC_PORTAL_FORWARD_EVENT(value_changed_event);
        RC_PORTAL_FORWARD_EVENT(posedge_event);
        RC_PORTAL_FORWARD_EVENT(negedge_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_CLOSE()
    {
        // write 'Z' to the static channel (for each registered driver)
        rc_interface_wrapper<if_type>& ifw = this->get_interface_wrapper();
        const int drv_count = ifw.get_nb_driver_count();
        for (int i=0; i < drv_count; i++) {
            ifw.get_nb_driver_access(i)->call(&if_type::write, SC_LOGIC_Z);
        }
    }

    RC_ON_UNDEF()
    {
        // write 'X' to the static channel (disable other drivers first)
        rc_interface_wrapper<if_type>& ifw = this->get_interface_wrapper();
        const int drv_count = ifw.get_nb_driver_count();
        for (int i=1; i < drv_count; i++) {
            ifw.get_nb_driver_access(i)->call(&if_type::write, SC_LOGIC_Z);
        }
        ifw.get_nb_driver_access(0)->call(&if_type::write, SC_LOGIC_X);
    }

    RC_ON_REFRESH_NOTIFY()
    {
        const sc_logic value = this->get_static_port().read();
        this->notify_event("value_changed_event");
        if (value == SC_LOGIC_1) {
            this->notify_event("posedge_event");
        }
    }
};

/**
 * @brief Portal for a resolved signal output port.
 */
template<>
class rc_portal<sc_out_resolved>
    : public rc_portal<sc_inout_resolved>
{
    typedef rc_portal<sc_inout_resolved> base_type;
public:
    typedef base_type::port_type     port_type;
    typedef base_type::if_type       if_type;
    typedef base_type::accessor_type accessor_type;

    rc_portal<sc_out_resolved>(
        const sc_module_name& module_name =
            sc_gen_unique_name("rc_portal"))
        : base_type(module_name)
    { }
};

/* named portal specializations */

/**
 * @brief Portal for a resolved signal input port (named).
 */
class rc_in_resolved_portal
    : public rc_portal<sc_in_resolved>
{
    typedef rc_portal<sc_in_resolved> base_type;
public:
    typedef base_type::port_type     port_type;
    typedef base_type::if_type       if_type;
    typedef base_type::accessor_type accessor_type;

    explicit rc_in_resolved_portal(
        const sc_module_name& name_ =
            sc_gen_unique_name("rc_in_resolved_portal"))
        : base_type(name_)
    { }
};

/**
 * @brief Portal for a resolved signal input/output port (named).
 */
class rc_inout_resolved_portal
    : public rc_portal<sc_inout_resolved>
{
    typedef rc_portal<sc_inout_resolved> base_type;
public:
    typedef base_type::port_type     port_type;
    typedef base_type::if_type       if_type;
    typedef base_type::accessor_type accessor_type;

    explicit rc_inout_resolved_portal(
        const sc_module_name& name_ =
            sc_gen_unique_name("rc_inout_resolved_portal"))
        : base_type(name_)
    { }
};

/**
 * @brief Portal for a resolved signal output port (named).
 */
class rc_out_resolved_portal
    : public rc_portal<sc_out_resolved>
{
    typedef rc_portal<sc_out_resolved> base_type;
public:
    typedef base_type::port_type     port_type;
    typedef base_type::if_type       if_type;
    typedef base_type::accessor_type accessor_type;

    explicit rc_out_resolved_portal(
        const sc_module_name& name_ =
            sc_gen_unique_name("rc_out_resolved_portal"))
        : base_type(name_)
    { }
};

} // namespace ReChannel

#endif // RC_RESOLVED_PORTALS_H_
//
// $Id: rc_resolved_portals.h,v 1.11 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/portals/rc_resolved_portals.h,v $
//

