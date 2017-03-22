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
 * @file    rc_event_queue_accessor.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Defines the accessor for the sc_event_queue channel.
 *
 * This file contains the accessor for the sc_event_queue_if interface
 * as well as the implementation of the respective rc_fallback_interface.
 *
 * $Date: 2007/11/23 13:21:48 $
 * $Revision: 1.7 $
 */

#ifndef RC_EVENT_QUEUE_ACCESSOR_H_
#define RC_EVENT_QUEUE_ACCESSOR_H_

#include "ReChannel/communication/accessors/rc_accessor.h"

namespace ReChannel {

/**
 * @brief Fallback interface for event queues.
 */
RC_FALLBACK_INTERFACE(sc_event_queue_if)
{
public:
    virtual void notify(double t, sc_time_unit tu)
        { }
    virtual void notify(const sc_time& t)
        { }
    virtual void cancel_all()
        { }
    virtual const sc_event& default_event() const
    {
        SC_REPORT_WARNING(RC_ID_UNDEFINED_EVENT_WARNING_, 0);
        return p_undef;
    }
private:
    sc_event p_undef;
};

/**
 * @brief Accessor for event queues.
 */
RC_ACCESSOR(sc_event_queue_if)
{
    RC_ACCESSOR_CTOR(sc_event_queue_if) { }

    RC_EVENT(default_event);

    virtual void notify(double t, sc_time_unit tu)
    {
        this->rc_nb_forward(&if_type::notify, t, tu);
    }
    virtual void notify(const sc_time& t)
    {
        this->rc_nb_forward(&if_type::notify, t);
    }
    virtual void cancel_all()
    {
        this->rc_nb_forward(&if_type::cancel_all);
    }
};

} // namespace ReChannel

#endif // RC_EVENT_QUEUE_ACCESSOR_H_
//
// $Id: rc_event_queue_accessor.h,v 1.7 2007/11/23 13:21:48 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/accessors/rc_event_queue_accessor.h,v $
//

