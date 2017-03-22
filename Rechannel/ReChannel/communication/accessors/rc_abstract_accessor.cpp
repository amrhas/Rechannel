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
 * @file    rc_abstract_accessor.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Code of class rc_abstract_accessor.
 *
 *
 * $Date: 2007/12/27 00:24:38 $
 * $Revision: 1.5 $
 */

#include "rc_abstract_accessor.h"

namespace ReChannel {

rc_abstract_accessor_b::rc_abstract_accessor_b()
{ }

void rc_abstract_accessor_b::register_port(
    sc_port_base& port_, const char* if_typename_)
{
    sc_interface* const target_if = rc_get_target_interface();
    rc_interface_wrapper_base* const target_wrapper =
        rc_get_target_wrapper_base();
    if (target_if != NULL) {
        target_if->register_port(port_, if_typename_);
    } else if (target_wrapper != NULL) {
        target_wrapper->register_port(port_, if_typename_);
    }
    p_bound_port_vector.push_back(&port_);
}

int rc_abstract_accessor_b::rc_get_driver_process_index(
    sc_object* proc) const
{
    if (proc == NULL) {
        return 0;
    } else {
        process_index_map::const_iterator it
            = p_process_index_map.find(proc);
        return (it != p_process_index_map.end() ? it->second : -1);
    }
}

int rc_abstract_accessor_b::rc_get_nb_driver_process_index(
    sc_object* proc) const
{
#ifndef RC_USE_SHARED_METHOD_DRIVER
    if (proc == NULL) {
        return 0;
    } else {
        process_index_map::const_iterator it
            = p_nb_process_index_map.find(proc);
        return (it != p_nb_process_index_map.end() ? it->second : -1);
    }
#else // RC_USE_SHARED_METHOD_DRIVER (optimisation)
    return 0; // always 0 because it will be the same driver anyway
#endif
}

int rc_abstract_accessor_b::rc_register_driver_process(
    sc_object* proc)
{
    if (proc == NULL) {
        return 0;
    } else if (p_process_index_map.find(proc) == p_process_index_map.end()) {
        const int new_index = p_process_index_map.size();
        p_process_index_map[proc] = new_index;
        return new_index;
    } else {
        return rc_get_driver_process_index(proc);
    }
}

int rc_abstract_accessor_b::rc_register_nb_driver_process(
    sc_object* proc)
{
#ifndef RC_USE_SHARED_METHOD_DRIVER
    if (proc == NULL) {
        return 0;
    } else if (p_nb_process_index_map.find(proc) == p_nb_process_index_map.end()) {
        const int new_index = p_nb_process_index_map.size();
        p_nb_process_index_map[proc] = new_index;
        return new_index;
    } else {
        return rc_get_nb_driver_process_index(proc);
    }
#else // RC_USE_SHARED_METHOD_DRIVER (optimisation)
    return 0; // always 0 because it will be the same driver anyway
#endif
}

} // namespace ReChannel
//
// $Id: rc_abstract_accessor.cpp,v 1.5 2007/12/27 00:24:38 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/accessors/rc_abstract_accessor.cpp,v $
//
