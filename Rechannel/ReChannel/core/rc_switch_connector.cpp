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
 * @file    rc_switch_connector.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class rc_switch_connector<PortMap>.
 *
 *
 * $Date: 2007/12/06 11:42:43 $
 * $Revision: 1.6 $
 */

#include "rc_switch_connector.h"
#include "ReChannel/core/rc_reconfigurable.h"

namespace ReChannel {

rc_switch& rc_switch_connector_base::get(int index) const
{
    if (index >= 0 && index < p_size) {
        return *(p_switch_arr[index]);
    } else {
        RC_REPORT_ERROR(RC_ID_OUT_OF_BOUNDS_,
            "out of bounds (in switch connector '" << this->name()
            << "', at index: " << index <<")");
    }
}

void rc_switch_connector_base::bind_static(
    const rc_portmap_base& portmap)
{
    if (p_size != portmap.size()) {
        RC_REPORT_ERROR(RC_ID_SWITCH_CONNECTOR_BINDING_ERROR_,
            "static switch binding failed due to incompatible port map"
            " (in switch connector '" << this->name() << "')");
    }
    for (int i=0; i < p_size; ++i) {
        p_switch_arr[i]->bind_static_object(portmap[i]);
    }
}

void rc_switch_connector_base::bind_dynamic(
    const rc_portmap_base& portmap)
{
    if (!this->is_compatible(portmap)) {
        RC_REPORT_ERROR(RC_ID_SWITCH_CONNECTOR_BINDING_ERROR_,
            "dynamic switch binding failed due to incompatible port map"
            " (in switch connector '" << this->name() << "')");
    }
    for (int i=0; i < p_size; ++i) {
        p_switch_arr[i]->bind_dynamic_object(portmap[i]);
    }
}

void rc_switch_connector_base::bind_dynamic(rc_reconfigurable& reconf)
{
    reconf.bind(*this);
}

void rc_switch_connector_base::_rc_init(rc_switch* switch_arr[], int size)
{
    assert(p_switch_arr == NULL);
    assert(switch_arr != NULL && size > 0);

    if (switch_arr != NULL && p_switch_arr == NULL) {
        p_switch_arr = switch_arr;
        p_size = size;
        for(int i=0; i < size; ++i) {
            rc_switch* const switch_obj = p_switch_arr[i];
            if (switch_obj == NULL) {
                RC_REPORT_ERROR(RC_ID_SWITCH_CONNECTOR_INIT_ERROR_,
                    "switch missing (in switch connector '"
                    << this->name() << "', at index: "
                    << i <<")");
            }
        }
    }
}

} // namespace ReChannel

//
// $Id: rc_switch_connector.cpp,v 1.6 2007/12/06 11:42:43 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_switch_connector.cpp,v $
//
