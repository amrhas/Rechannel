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
 * @file    rc_abstract_switch.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:26:39 $
 * $Revision: 1.6 $
 */

#include "rc_abstract_switch.h"
#include "../core/rc_reconfigurable.h"

namespace ReChannel {

rc_abstract_switch_b::rc_abstract_switch_b()
    : m_state(UNDEF), m_transaction_count(0), p_lock_owner(NULL)
{ }

rc_abstract_switch_b::~rc_abstract_switch_b()
{ }

bool rc_abstract_switch_b::set_locked(
    rc_reconfigurable& lock_owner, bool lock)
{
    if (p_lock_owner == NULL || p_lock_owner == &lock_owner) {
        p_lock_owner = (lock ? &lock_owner : NULL);
        return true;
    } else {
        return false;
    }
}

void rc_abstract_switch_b::register_reconfigurable(
    rc_reconfigurable& module, sc_interface& dyn_if)
{
    if (is_registered(module)) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "reconfigurable module '" << module.rc_get_name()
            << "' already bound (in switch '"
            << this->get_switch_name() << "')");
    }
    void* dyn_if_ = _rc_dynamic_cast(&dyn_if);
    if (dyn_if_ == NULL) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "given interface ('" << _rc_get_if_type()
            << "' is incompatible (in switch '"
            << this->get_switch_name() << "')");
    } else if (p_interface_set.find(dyn_if_) !=
        p_interface_set.end()) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "interface ('" << _rc_get_if_type()
            << "' already registered (in switch '"
            << this->get_switch_name() << "')");
    }
    p_interface_set.insert(dyn_if_);
    p_mod_if_map[&module] = dyn_if_;
}

void rc_abstract_switch_b::unregister_reconfigurable(
    rc_reconfigurable& module)
{
    void* if_ = find_registered_if(&module);
    if (if_ != NULL) {
        p_interface_set.erase(if_);
        p_mod_if_map.erase(&module);
    }
}

} // namespace ReChannel

//
// $Id: rc_abstract_switch.cpp,v 1.6 2007/12/20 20:26:39 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/rc_abstract_switch.cpp,v $
//
