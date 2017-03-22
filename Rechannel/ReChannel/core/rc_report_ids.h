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
 * @file    core/rc_report_ids.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Error and warning messages used by the "core" package.
 *
 *
 * $Date: 2007/10/09 21:47:01 $
 * $Revision: 1.8 $
 */

#ifndef RC_CORE_REPORT_IDS_H_
#define RC_CORE_REPORT_IDS_H_

#include "ReChannel/util/rc_report.h"

namespace ReChannel {

extern const char RC_ID_RELNAME_SYNTAX_ERROR_[];

extern const char RC_ID_CONTROL_CONFLICT_[];

extern const char RC_ID_SWITCH_BINDING_ERROR_[];

extern const char RC_ID_SWITCH_INVALID_FILTER_[];

extern const char RC_ID_SWITCH_CONFLICT_[];

extern const char RC_ID_TIME_CONSUMED_WARNING_[];

extern const char RC_ID_DELTAS_CONSUMED_WARNING_[];

extern const char RC_ID_PORTMAP_INIT_ERROR_[];

extern const char RC_ID_PORTMAP_INCOMPATIBLE_ERROR_[];

extern const char RC_ID_SWITCH_CONNECTOR_INIT_ERROR_[];

extern const char RC_ID_SWITCH_CONNECTOR_BINDING_ERROR_[];

extern const char RC_ID_SWITCH_CONNECTOR_INCOMPATIBLE_ERROR_[];

} // namespace ReChannel

#endif // RC_CORE_REPORT_IDS_H_

//
// $Id: rc_report_ids.h,v 1.8 2007/10/09 21:47:01 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_report_ids.h,v $
//

