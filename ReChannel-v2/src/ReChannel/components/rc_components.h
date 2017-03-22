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
 * @file    rc_components.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/10/09 00:22:26 $
 * $Revision: 1.8 $
 */

#ifndef RC_COMPONENTS_H_
#define RC_COMPONENTS_H_

#include "ReChannel/components/rc_module.h"
#include "ReChannel/components/rc_ports.h"
#include "ReChannel/components/rc_var.h"
#include "ReChannel/components/rc_join.h"

#include "ReChannel/components/rc_prim_channel.h"
#include "ReChannel/components/rc_signal.h"
#include "ReChannel/components/rc_buffer.h"
#include "ReChannel/components/rc_signal_resolved.h"
#include "ReChannel/components/rc_signal_rv.h"
#include "ReChannel/components/rc_fifo.h"
#include "ReChannel/components/rc_mutex.h"
#include "ReChannel/components/rc_semaphore.h"

#include "ReChannel/components/rc_sc_event.h"
#include "ReChannel/components/rc_sc_signal.h"
#include "ReChannel/components/rc_sc_buffer.h"
#include "ReChannel/components/rc_sc_signal_resolved.h"
#include "ReChannel/components/rc_sc_signal_rv.h"
#include "ReChannel/components/rc_sc_fifo.h"

namespace ReChannel {

typedef rc_sc_event rc_event;

} // namespace ReChannel

#endif //RC_COMPONENTS_H_

//
// $Id: rc_components.h,v 1.8 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_components.h,v $
//
