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
 * @file    rc_event_queue_exportal.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   The exportal for the sc_event_queue_if interface.
 *
 * This file contains the exportal for the sc_event_queue_if interface.
 *
 * $Date: 2007/11/23 13:21:48 $
 * $Revision: 1.5 $
 */

#ifndef RC_EVENT_QUEUE_EXPORTAL_H_
#define RC_EVENT_QUEUE_EXPORTAL_H_

#include "ReChannel/communication/exportals/rc_exportal.h"
#include "ReChannel/communication/accessors/rc_event_queue_accessor.h"

namespace ReChannel {

/**
 * @brief Exportal switch for an event queue.
 */
RC_EXPORTAL(sc_event_queue_if)
{
    RC_EXPORTAL_CTOR(sc_event_queue_if)
    {
        RC_EXPORTAL_FORWARD_EVENT(default_event);
    }
};

} // namespace ReChannel

#endif // RC_EVENT_QUEUE_EXPORTAL_H_
//
// $Id: rc_event_queue_exportal.h,v 1.5 2007/11/23 13:21:48 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/exportals/rc_event_queue_exportal.h,v $
//
