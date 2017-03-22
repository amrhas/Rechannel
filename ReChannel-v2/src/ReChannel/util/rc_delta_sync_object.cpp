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
 * @file    rc_delta_sync_object.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class rc_delta_sync_object.
 *
 *
 * $Date: 2007/10/09 00:22:26 $
 * $Revision: 1.4 $
 */

#include "rc_delta_sync_object.h"

namespace ReChannel {

rc_delta_sync_object::rc_delta_sync_object(
    delta_sync_callback_type callback, bool enabled)
    : sc_prim_channel(sc_gen_unique_name("_rc_delta_sync_object")),
      p_enabled(enabled), p_callback(callback)
{ }

rc_delta_sync_object::rc_delta_sync_object()
    : sc_prim_channel(sc_gen_unique_name("_rc_delta_sync_object")),
      p_enabled(false)
{ }

void rc_delta_sync_object::request_update()
{
    if (p_enabled) {
        sc_prim_channel::request_update();
    }
}

void rc_delta_sync_object::set_callback(
    delta_sync_callback_type callback)
{
    p_callback = callback;
}

void rc_delta_sync_object::update()
{
    // is enabled?
    if (p_enabled == true) {
        // call the callback (if not empty)
        if (!p_callback.empty()) {
            p_callback(*this); // supply this object as reference
        }
    }
}

} // namespace ReChannel

//
// $Id: rc_delta_sync_object.cpp,v 1.4 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_delta_sync_object.cpp,v $
//

