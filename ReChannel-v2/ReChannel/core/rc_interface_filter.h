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
 * @file    rc_interface_filter.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   ABI rc_interface_filter.
 *
 *
 * $Date: 2007/11/23 13:24:55 $
 * $Revision: 1.6 $
 */

#ifndef RC_INTERFACE_FILTER_H_
#define RC_INTERFACE_FILTER_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_event_filter.h"

namespace ReChannel {

/**
 * @brief ABI of the interface filter.
 */
class rc_interface_filter
    : virtual public sc_interface
{
public:
    virtual sc_interface* rc_get_target_interface() const = 0;
    virtual bool rc_set_target(sc_interface& target) = 0;
    virtual void rc_clear_target() = 0;

    virtual rc_event_filter* rc_get_event_filter() = 0;
};

} // namespace ReChannel

#endif //RC_INTERFACE_FILTER_H_

//
// $Id: rc_interface_filter.h,v 1.6 2007/11/23 13:24:55 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_interface_filter.h,v $
//

