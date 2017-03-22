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
 * @file    rc_event_filter.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   ABI rc_event_filter.
 *
 *
 * $Date: 2007/12/27 00:26:13 $
 * $Revision: 1.3 $
 */

#ifndef RC_EVENT_FILTER_H_
#define RC_EVENT_FILTER_H_

#include "ReChannel/core/rc_common_header.h"

namespace ReChannel {

// forward declaration
class rc_event_trigger;

/**
 * @brief ABI of the event filter.
 */
class rc_event_filter
    : virtual public sc_interface
{
public:
    virtual bool rc_on_event(const sc_event& e) = 0;

    virtual void rc_set_event_trigger(
        const sc_event& e, rc_event_trigger& t) = 0;

    virtual void rc_clear_event_trigger(const sc_event& e) = 0;

protected:
    virtual ~rc_event_filter() {}
};

/**
 * @brief ABI of an event trigger control.
 */
class rc_event_trigger
{
public:
    virtual void rc_trigger_event(const sc_event& e) = 0;

protected:
    virtual ~rc_event_trigger() {}
};

} // namespace ReChannel

#endif //RC_INTERFACE_FILTER_H_

//
// $Id: rc_event_filter.h,v 1.3 2007/12/27 00:26:13 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_event_filter.h,v $
//
