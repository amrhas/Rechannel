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
 * @file    rc_fifo_exportals.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Exportals for sc_fifo channels.
 *
 * This file contains the exportals for sc_fifo_in_if<T> and
 * sc_fifo_out_if<T>.
 *
 * $Date: 2007/11/23 13:24:55 $
 * $Revision: 1.6 $
 */

#ifndef RC_FIFO_EXPORTALS_H_
#define RC_FIFO_EXPORTALS_H_

#include "ReChannel/communication/exportals/rc_exportal.h"
#include "ReChannel/communication/accessors/rc_fifo_accessors.h"

namespace ReChannel {

/**
 * @brief Exportal switch for a FIFO (input).
 */
template<class T>
RC_EXPORTAL_TEMPLATE(sc_fifo_in_if<T>)
{
    RC_EXPORTAL_TEMPLATE_CTOR(sc_fifo_in_if<T>)
    {
        RC_EXPORTAL_FORWARD_EVENT(data_written_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_REFRESH_NOTIFY()
    {
        if_type* const interface_ = this->get_interface();
        if (interface_ != NULL) {
            if (interface_->num_available() > 0) {
                this->notify_event("data_written_event");
            }
        }
    }
};

/**
 * @brief Exportal switch for a FIFO (output).
 */
template<class T>
RC_EXPORTAL_TEMPLATE(sc_fifo_out_if<T>)
{
    RC_EXPORTAL_TEMPLATE_CTOR(sc_fifo_out_if<T>)
    {
        RC_EXPORTAL_FORWARD_EVENT(data_read_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_REFRESH_NOTIFY()
    {
        if_type* const interface_ = this->get_interface();
        if (interface_ != NULL) {
            if (interface_->num_free() > 0) {
                this->notify_event("data_read_event");
            }
        }
    }
};

} // namespace ReChannel

#endif // RC_FIFO_EXPORTALS_H_
//
// $Id: rc_fifo_exportals.h,v 1.6 2007/11/23 13:24:55 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/exportals/rc_fifo_exportals.h,v $
//

