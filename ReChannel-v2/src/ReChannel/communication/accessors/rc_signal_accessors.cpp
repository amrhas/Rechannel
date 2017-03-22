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
 * @file    rc_signal_accessors.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/06 11:42:43 $
 * $Revision: 1.5 $
 */

#include "rc_signal_accessors.h"

namespace ReChannel {

void rc_accessor<sc_signal_in_if<bool> >::register_process_control(
    rc_process_control& pctrl, bool active_level) const
{
    _rc_get_reset_signal().register_process_control(
        pctrl, active_level);
}

void rc_accessor<sc_signal_in_if<bool> >::unregister_process_control(
    rc_process_control& pctrl) const
{
    _rc_get_reset_signal().unregister_process_control(pctrl);
}

rc_accessor<sc_signal_in_if<bool> >::reset_signal_type&
rc_accessor<sc_signal_in_if<bool> >::_rc_get_reset_signal() const
{
    if (p_reset_signal == NULL) {
        p_reset_signal =
            new reset_signal_type(
                sc_gen_unique_name("_rc_signal_accessor_reset_signal"));
        sc_spawn_options opt;
        {
            opt.spawn_method();
            opt.set_sensitivity(&this->value_changed_event());
        }
        sc_spawn(
            sc_bind(
                &this_type::_rc_reset_updater_proc,
                const_cast<this_type*>(this)),
            sc_gen_unique_name("_rc_accessor_reset_updater_proc"), &opt);
    }
    return *p_reset_signal;
}

void rc_accessor<sc_signal_in_if<bool> >::_rc_reset_updater_proc()
{
    rc_interface_wrapper<if_type>* const target_wrapper =
        this->rc_get_target_wrapper();
    if (target_wrapper != NULL) {
        p_reset_signal->write(
            target_wrapper->get_nb_interface_access()->read());
    } else {
        p_reset_signal->write(rc_undefined_value<bool>());
    }
}

void rc_accessor<sc_signal_inout_if<bool> >::register_process_control(
    rc_process_control& pctrl, bool active_level) const
{
    _rc_get_reset_signal().register_process_control(
        pctrl, active_level);
}

void rc_accessor<sc_signal_inout_if<bool> >::unregister_process_control(
    rc_process_control& pctrl) const
{
    _rc_get_reset_signal().unregister_process_control(pctrl);
}

rc_accessor<sc_signal_inout_if<bool> >::reset_signal_type&
rc_accessor<sc_signal_inout_if<bool> >::_rc_get_reset_signal() const
{
    if (p_reset_signal == NULL) {
        p_reset_signal =
            new reset_signal_type(
                sc_gen_unique_name("_rc_signal_accessor_reset_signal"));
        sc_spawn_options opt;
        {
            opt.spawn_method();
            opt.set_sensitivity(&this->value_changed_event());
        }
        sc_spawn(
            sc_bind(
                &this_type::_rc_reset_updater_proc,
                const_cast<this_type*>(this)),
            sc_gen_unique_name("_rc_accessor_reset_updater_proc"), &opt);
    }
    return *p_reset_signal;
}

void rc_accessor<sc_signal_inout_if<bool> >::_rc_reset_updater_proc()
{
    rc_interface_wrapper<if_type>* const target_wrapper =
        this->rc_get_target_wrapper();
    if (target_wrapper != NULL) {
        p_reset_signal->write(
            target_wrapper->get_nb_interface_access()->read());
    } else {
        p_reset_signal->write(rc_undefined_value<bool>());
    }
}

} // namespace ReChannel

//
// $Id: rc_signal_accessors.cpp,v 1.5 2007/12/06 11:42:43 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/accessors/rc_signal_accessors.cpp,v $
//

