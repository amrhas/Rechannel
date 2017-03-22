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
 * @file    rc_portmap.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/10/14 05:26:41 $
 * $Revision: 1.3 $
 */

#include "rc_portmap.h"

namespace ReChannel {

rc_object_handle rc_portmap_base::get(int index) const
{
    if (index >= 0 && index < p_size) {
        return p_arr[index];
    } else {
        RC_REPORT_ERROR(RC_ID_OUT_OF_BOUNDS_,
            "out of bounds (in port map, at index: " << index <<")");
    }
}

sc_object& rc_portmap_base::get_object(int index) const
{
    if (index >= 0 && index < p_size) {
        return *(p_arr[index].get_object());
    } else {
        RC_REPORT_ERROR(RC_ID_OUT_OF_BOUNDS_,
            "out of bounds (in port map, at index: " << index <<")");
    }
}

sc_interface* rc_portmap_base::get_interface(int index) const
{
    if (index >= 0 && index < p_size) {
        return p_arr[index].get_interface();
    } else {
        RC_REPORT_ERROR(RC_ID_OUT_OF_BOUNDS_,
            "out of bounds (in port map, at index: " << index <<")");
    }
}

void rc_portmap_base::_rc_init(rc_object_handle arr[], int size)
{
    assert(p_arr == NULL);
    assert(arr != NULL && size > 0);

    if (arr != NULL && p_arr == NULL) {
        p_arr = arr;
        p_size = size;
        for(int i=0; i < size; ++i) {
            if (p_arr[i].is_comm_object() == false) {
                RC_REPORT_ERROR(RC_ID_PORTMAP_INIT_ERROR_,
                    "missing communication object:"
                    " port, export or channel expected"
                    " (in port map, at index: " << i <<")");
            }
        }
    }
}

} // namespace ReChannel
//
// $Id: rc_portmap.cpp,v 1.3 2007/10/14 05:26:41 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_portmap.cpp,v $
//
