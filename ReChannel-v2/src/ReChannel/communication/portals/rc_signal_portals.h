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
 * @file    rc_signal_portals.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Defines portals for the use with SystemC's signal ports/channels.
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.13 $
 */

#ifndef RC_SIGNAL_PORTALS_H_
#define RC_SIGNAL_PORTALS_H_

#include "ReChannel/communication/portals/rc_portal.h"
#include "ReChannel/communication/accessors/rc_signal_accessors.h"

namespace ReChannel {

/**
 * @brief Portal for signal input port.
 */
template<class T>
RC_PORTAL_TEMPLATE(sc_in<T>)
{
    RC_PORTAL_TEMPLATE_CTOR(sc_in<T>)
    {
        RC_PORTAL_FORWARD_EVENT(value_changed_event);
    }

    RC_ON_OPEN()
    {
        this->notify_event("value_changed_event");
    }
};

/**
 * @brief Portal for signal<bool> input port.
 */
RC_PORTAL(sc_in<bool>)
{
    RC_PORTAL_CTOR(sc_in<bool>)
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
        this->notify_event("value_changed_event");
        if (this->get_static_port().read() == true) {
            this->notify_event("posedge_event");
        } else {
            this->notify_event("negedge_event");
        }
    }
};

/**
 * @brief Portal for signal<sc_logic> input port.
 */
RC_PORTAL(sc_in<sc_logic>)
{
    RC_PORTAL_CTOR(sc_in<sc_logic>)
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
        } else if (value == SC_LOGIC_0) {
            this->notify_event("negedge_event");
        }
    }
};

/**
 * @brief Portal for signal input/output port.
 */
template<class T>
RC_PORTAL_TEMPLATE(sc_inout<T>)
{
    typedef rc_portal<sc_inout<T> > this_type;

    RC_PORTAL_TEMPLATE_CTOR(sc_inout<T>),
        p_is_register(false)
    {
        RC_PORTAL_FORWARD_EVENT(value_changed_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_CLOSE()
    {
        if (!p_is_register) {
            // write undefined value to the static channel
            rc_interface_wrapper<if_type>& ifw = this->get_interface_wrapper();
            ifw.get_nb_driver_access(0)->call(
                &if_type::write, rc_undefined_value<T>());
        }
    }

    RC_ON_UNDEF()
    {
        this_type::rc_on_close();
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
    }

    inline void set_register()
        { if (!sc_is_running()) p_is_register = true; }
    inline bool is_register() const
        { return p_is_register; }

private:
    bool p_is_register;
};

/**
 * @brief Portal for signal<bool> input/output port.
 */
RC_PORTAL(sc_inout<bool>)
{
    typedef rc_portal<sc_inout<bool> > this_type;

    RC_PORTAL_CTOR(sc_inout<bool>),
        p_is_register(false)
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
        if (!p_is_register) {
            // write undefined value to the static channel
            rc_interface_wrapper<if_type>& ifw = this->get_interface_wrapper();
            ifw.get_nb_driver_access(0)->call(
                &if_type::write, rc_undefined_value<bool>());
        }
    }

    RC_ON_UNDEF()
    {
        this_type::rc_on_close();
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
        if (this->get_static_port().read() == true) {
            this->notify_event("posedge_event");
        } else {
            this->notify_event("negedge_event");
        }
    }

    inline void set_register()
        { if (!sc_is_running()) p_is_register = true; }

    inline bool is_register() const
        { return p_is_register; }

private:
    bool p_is_register;
};

/**
 * @brief Portal for signal<sc_logic> input/output port.
 */
RC_PORTAL(sc_inout<sc_logic>)
{
    RC_PORTAL_CTOR(sc_inout<sc_logic>),
        p_is_register(false)
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
        if (!p_is_register) {
            // write 'Z' to the static channel
            rc_interface_wrapper<if_type>& ifw = this->get_interface_wrapper();
            ifw.get_nb_driver_access(0)->call(&if_type::write, SC_LOGIC_Z);
        }
    }

    RC_ON_UNDEF()
    {
        if (!p_is_register) {
            // write 'X' to the static channel
            rc_interface_wrapper<if_type>& ifw = this->get_interface_wrapper();
            ifw.get_nb_driver_access(0)->call(&if_type::write, SC_LOGIC_X);
        }
    }

    RC_ON_REFRESH_NOTIFY()
    {
        const sc_logic value = this->get_static_port().read();
        this->notify_event("value_changed_event");
        if (value == SC_LOGIC_1) {
            this->notify_event("posedge_event");
        } else if (value == SC_LOGIC_0) {
            this->notify_event("negedge_event");
        }
    }

    inline void set_register()
        { if (!sc_is_running()) p_is_register = true; }

    inline bool is_register() const
        { return p_is_register; }

private:
    bool p_is_register;
};

/**
 * @brief Portal for signal<sc_lv<W>> input/output port.
 */
template<int W>
RC_PORTAL_TEMPLATE(sc_inout<sc_lv<W> >)
{
    RC_PORTAL_TEMPLATE_CTOR(sc_inout<sc_lv<W> >),
        p_is_register(false)
    {
        RC_PORTAL_FORWARD_EVENT(value_changed_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_CLOSE()
    {
        if (!p_is_register) {
            // write 'Z..Z' to the static channel
            rc_interface_wrapper<if_type>& ifw = this->get_interface_wrapper();
            ifw.get_nb_driver_access(0)->call(&if_type::write, s_ZZZ);
        }
    }

    RC_ON_UNDEF()
    {
        if (!p_is_register) {
            // write 'X..X' to the static channel
            rc_interface_wrapper<if_type>& ifw = this->get_interface_wrapper();
            ifw.get_nb_driver_access(0)->call(&if_type::write, s_XXX);
        }
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
    }

    inline void set_register()
        { if (!sc_is_running()) { p_is_register = true; } }

    inline bool is_register() const
        { return p_is_register; }

private:
    bool p_is_register;

private:
    static const sc_dt::sc_lv<W> s_ZZZ;
    static const sc_dt::sc_lv<W> s_XXX;
};

template<int W>
const typename sc_dt::sc_lv<W>
rc_portal<sc_inout<sc_lv<W> > >::s_ZZZ = sc_lv<W>('Z');

template<int W>
const typename sc_dt::sc_lv<W>
rc_portal<sc_inout<sc_lv<W> > >::s_XXX = sc_lv<W>('X');

/**
 * @brief Portal for signal output port.
 */
template<class T>
class rc_portal<sc_out<T> >
    : public rc_portal<sc_inout<T> >
{
    typedef rc_portal<sc_inout<T> > base_type;
public:
    typedef typename base_type::port_type     port_type;
    typedef typename base_type::if_type       if_type;
    typedef typename base_type::accessor_type accessor_type;

    rc_portal<sc_out<T> >(
        const sc_module_name& module_name =
            sc_gen_unique_name("rc_portal"))
        : base_type(module_name)
    { }
};

/* named portal specializations */

/**
 * @brief Portal for signal input port (named).
 */
template<class T>
class rc_in_portal
    : public rc_portal<sc_in<T> >
{
    typedef rc_portal<sc_in<T> > base_type;
public:
    typedef typename base_type::port_type     port_type;
    typedef typename base_type::if_type       if_type;
    typedef typename base_type::accessor_type accessor_type;

    explicit rc_in_portal(
        const sc_module_name& name_ =
            sc_gen_unique_name("rc_in_portal"))
        : base_type(name_)
    { }
};

/**
 * @brief Portal for signal input/output port (named).
 */
template<class T>
class rc_inout_portal
    : public rc_portal<sc_inout<T> >
{
    typedef rc_portal<sc_inout<T> > base_type;
public:
    typedef typename base_type::port_type     port_type;
    typedef typename base_type::if_type       if_type;
    typedef typename base_type::accessor_type accessor_type;

    explicit rc_inout_portal(
        const sc_module_name& name_ =
            sc_gen_unique_name("rc_inout_portal"))
        : base_type(name_)
    { }
};

/**
 * @brief Portal for signal output port (named).
 */
template<class T>
class rc_out_portal
    : public rc_portal<sc_out<T> >
{
    typedef rc_portal<sc_out<T> > base_type;
public:
    typedef typename base_type::port_type     port_type;
    typedef typename base_type::if_type       if_type;
    typedef typename base_type::accessor_type accessor_type;

    explicit rc_out_portal(
        const sc_module_name& name_ =
            sc_gen_unique_name("rc_out_portal"))
        : base_type(name_)
    { }
};

} // namespace ReChannel

#endif // RC_SIGNAL_PORTALS_H_
//
// $Id: rc_signal_portals.h,v 1.13 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/portals/rc_signal_portals.h,v $
//
