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
 * @file    rc_object_handle.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Generalised handling of SystemC objects.
 *
 *
 * $Date: 2007/10/09 00:22:26 $
 * $Revision: 1.3 $
 */

#include "rc_object_handle.h"

namespace ReChannel {

rc_object_handle::rc_object_handle()
    : p_object(NULL), p_channel_if(NULL)
{ }

rc_object_handle::rc_object_handle(sc_object& obj_)
    : p_object(&obj_)
{
    p_channel_if = dynamic_cast<sc_interface*>(&obj_);
}

rc_object_handle::rc_object_handle(const rc_port_handle& port_)
    : p_object(port_.get_object()), p_channel_if(NULL), p_port(port_)
{ }

rc_object_handle::rc_object_handle(const rc_export_handle& export_)
    : p_object(export_.get_object()), p_channel_if(NULL), p_export(export_)
{ }

rc_object_handle& rc_object_handle::operator=(
    sc_object& obj_)
{
    p_object = &obj_;
    p_channel_if = dynamic_cast<sc_interface*>(&obj_);
    p_port.reset();
    p_export.reset();
    return *this;
}

rc_object_handle& rc_object_handle::operator=(
    const rc_port_handle& port_)
{
    p_object = port_.get_object();
    p_channel_if = NULL;
    p_port = port_;
    p_export.reset();
    return *this;
}

rc_object_handle& rc_object_handle::operator=(
    const rc_export_handle& export_)
{
    p_object = export_.get_object();
    p_channel_if = NULL;
    p_port.reset();
    p_export = export_;
    return *this;
}

sc_interface* rc_object_handle::get_interface() const
{
    if (p_channel_if != NULL) {
        return p_channel_if;
    } else if (p_port.valid()) {
        return p_port.get_interface();
    } else if (p_export.valid()) {
        return p_export.get_interface();
    } else {
        return NULL;
    }
}

} // namespace ReChannel

//
// $Id: rc_object_handle.cpp,v 1.3 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_object_handle.cpp,v $
//

