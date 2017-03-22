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
 * @file    rc_event_queue_portal.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Portal for sc_event_queue channels.
 *
 * This file contains the rc_event_queue_portal class.
 *
 * $Date: 2007/11/23 13:21:48 $
 * $Revision: 1.6 $
 */

#ifndef RC_EVENT_QUEUE_PORTAL_H_
#define RC_EVENT_QUEUE_PORTAL_H_

#include "ReChannel/communication/portals/rc_portal.h"
#include "ReChannel/communication/accessors/rc_event_queue_accessor.h"

namespace ReChannel {

RC_PORTAL(sc_port<sc_event_queue_if>)
{
    RC_PORTAL_CTOR(sc_port<sc_event_queue_if>)
    {
        RC_PORTAL_FORWARD_EVENT(default_event);
    }
};

/**
 * @brief Portal for an event queue.
 */
class rc_event_queue_portal
    : public rc_portal<sc_port<sc_event_queue_if> >
{
    typedef rc_portal<sc_port<sc_event_queue_if> > base_type;
public:
    typedef base_type::port_type     port_type;
    typedef base_type::if_type       if_type;
    typedef base_type::accessor_type accessor_type;

    explicit rc_event_queue_portal(
        const sc_module_name& name_ =
            sc_gen_unique_name("rc_event_queue_portal"))
        : base_type(name_)
    { }
};

} // namespace ReChannel

#endif // RC_EVENT_QUEUE_PORTAL_H_
//
// $Id: rc_event_queue_portal.h,v 1.6 2007/11/23 13:21:48 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/portals/rc_event_queue_portal.h,v $
//
