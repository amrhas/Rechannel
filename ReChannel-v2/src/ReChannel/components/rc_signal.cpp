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
 * @file    rc_signal.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:33:51 $
 * $Revision: 1.4 $
 */

#include "rc_signal.h"

namespace ReChannel {

namespace internals {
namespace signals {

void report_binding_error(
    const sc_object& target, const sc_object& port1,
    const sc_object& port2)
{
    RC_REPORT_ERROR(RC_ID_SIGNAL_BINDING_ERROR_,
        "signal cannot be bound to more than one output port"
        " (in rc_signal '" << target.name() << "', first port: '"
        << port1.name() << "', second port: '"
        << port2.name() << "')");
}

void report_driver_conflict(
    const sc_object& target, const sc_object& driver1,
    const sc_object& driver2)
{
    RC_REPORT_ERROR(RC_ID_SIGNAL_DRIVER_CONFLICT_,
        "signal cannot have more than one driver"
        " (in rc_signal '" << target.name() << "', first driver: '"
        << driver1.name() << "', second driver: '"
        << driver2.name() << "')");
}

} // namespace internals
} // namespace signals

void rc_signal<bool>::register_process_control(
    rc_process_control& pctrl, bool active_level) const
{
    p_pctrl_set[(active_level ? 1 : 0)].insert(&pctrl);
}

void rc_signal<bool>::unregister_process_control(
    rc_process_control& pctrl) const
{
    p_pctrl_set[0].erase(&pctrl);
    p_pctrl_set[1].erase(&pctrl);
}

bool rc_signal<bool>::get_current_level() const
{
    return m_current_value;
}

const sc_signal<bool>* rc_signal<bool>::get_underlying_reset_signal() const
{
    if (!this->rc_has_reconfigurable()) {
        return &this->_rc_get_reset_signal();
    } else {
        return NULL;
    }
}

void rc_signal<bool>::update()
{
    if (this->rc_is_active() && !(m_new_value == m_current_value)) {
        m_current_value = m_new_value;
        if (m_notify_value_changed_event) {
            m_value_changed_event.notify(SC_ZERO_TIME);
        }
        if (m_new_value) {
            if (m_notify_posedge_event) {
                m_posedge_event.notify(SC_ZERO_TIME);
            }
        } else {
            if (m_notify_negedge_event) {
                m_negedge_event.notify(SC_ZERO_TIME);
            }
        }
        m_delta = sc_delta_count();

        pctrl_set& set_ = p_pctrl_set[(m_new_value ? 1 : 0)];
        for (pctrl_set::iterator it = set_.begin();
            it != set_.end();
            ++it)
        {
            (*it)->deactivate();
        }
    }
}

sc_signal<bool>& rc_signal<bool>::_rc_get_reset_signal() const
{
    if (p_reset_signal == NULL) {
        p_reset_signal =
            new sc_signal<bool>(
                sc_gen_unique_name("_rc_signal_reset_signal"));
        sc_spawn_options opt;
        {
            opt.spawn_method();
            opt.set_sensitivity(&this->value_changed_event());
        }
        sc_spawn(
            sc_bind(
                &rc_signal<bool>::_rc_reset_updater_proc,
                const_cast<rc_signal<bool>*>(this)),
            sc_gen_unique_name("_rc_reset_updater_proc"), &opt);
    }
    return *p_reset_signal;
}

void rc_signal<bool>::_rc_reset_updater_proc()
{
    p_reset_signal->write(this->read());
}

void rc_signal<sc_dt::sc_logic>::update()
{
    if (this->rc_is_active() && !(m_new_value == m_current_value)) {
        m_current_value = m_new_value;
        if (m_notify_value_changed_event) {
            m_value_changed_event.notify(SC_ZERO_TIME);
        }
        if (m_new_value == sc_dt::SC_LOGIC_1) {
            if (m_notify_posedge_event) {
                m_posedge_event.notify(SC_ZERO_TIME);
            }
        } else if (m_new_value == sc_dt::SC_LOGIC_0) {
            if (m_notify_negedge_event) {
                m_negedge_event.notify(SC_ZERO_TIME);
            }
        }
        m_delta = sc_delta_count();
    }
}

} // namespace ReChannel

//
// $Id: rc_signal.cpp,v 1.4 2007/12/20 20:33:51 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_signal.cpp,v $
//
