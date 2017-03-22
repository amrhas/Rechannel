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
 * @file    rc_fifo_portals.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Portals for sc_fifo channels.
 *
 * This file contains the rc_fifo_in_portal and the rc_fifo_out_portal class.
 *
 * $Date: 2007/11/23 13:24:55 $
 * $Revision: 1.8 $
 */

#ifndef RC_FIFO_PORTALS_H_
#define RC_FIFO_PORTALS_H_

#include "ReChannel/communication/portals/rc_portal.h"
#include "ReChannel/communication/accessors/rc_fifo_accessors.h"

namespace ReChannel {

/**
 * @brief Portal for a FIFO input port.
 */
template<class T>
RC_PORTAL_TEMPLATE(sc_fifo_in<T>)
{
    RC_PORTAL_TEMPLATE_CTOR(sc_fifo_in<T>)
    {
        RC_PORTAL_FORWARD_EVENT(data_written_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_REFRESH_NOTIFY()
    {
        if (this->get_static_port().num_available() > 0) {
            this->notify_event("data_written_event");
        }
    }
};

/**
 * @brief Portal for a FIFO output port.
 */
template<class T>
RC_PORTAL_TEMPLATE(sc_fifo_out<T>)
{
    RC_PORTAL_TEMPLATE_CTOR(sc_fifo_out<T>)
    {
        RC_PORTAL_FORWARD_EVENT(data_read_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_REFRESH_NOTIFY()
    {
        if (this->get_static_port().num_free() > 0) {
            this->notify_event("data_read_event");
        }
    }
};

/* named portal specializations */

/**
 * @brief Portal for a FIFO input port (named).
 */
template<class T>
class rc_fifo_in_portal
    : public rc_portal<sc_fifo_in<T> >
{
    typedef rc_portal<sc_fifo_in<T> > base_type;
public:
    typedef typename base_type::port_type     port_type;
    typedef typename base_type::if_type       if_type;
    typedef typename base_type::accessor_type accessor_type;

    explicit rc_fifo_in_portal(
        const sc_module_name& name_ =
            sc_gen_unique_name("rc_fifo_in_portal"))
        : base_type(name_)
    { }
};

/**
 * @brief Portal for a FIFO output port (named).
 */
template<class T>
class rc_fifo_out_portal
    : public rc_portal<sc_fifo_out<T> >
{
    typedef rc_portal<sc_fifo_out<T> > base_type;
public:
    typedef typename base_type::port_type     port_type;
    typedef typename base_type::if_type       if_type;
    typedef typename base_type::accessor_type accessor_type;

    explicit rc_fifo_out_portal(
        const sc_module_name& name_ =
            sc_gen_unique_name("rc_fifo_out_portal"))
        : base_type(name_)
    { }
};

} // namespace ReChannel

#endif // RC_FIFO_PORTALS_H_
//
// $Id: rc_fifo_portals.h,v 1.8 2007/11/23 13:24:55 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/portals/rc_fifo_portals.h,v $
//
