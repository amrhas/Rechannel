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
 * @file    rc_report_ids.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Error and warning messages used by the "core" package.
 *
 *
 * $Date: 2007/10/09 21:47:01 $
 * $Revision: 1.7 $
 */

#include "rc_report_ids.h"

namespace ReChannel {

const char RC_ID_RELNAME_SYNTAX_ERROR_[]
    = RC_REPORT_MSG_TYPE "/relname_syntax_error";

const char RC_ID_CONTROL_CONFLICT_[]
    = RC_REPORT_MSG_TYPE "/control/conflict";

const char RC_ID_SWITCH_BINDING_ERROR_[]
    = RC_REPORT_MSG_TYPE "/switch/binding_error";

const char RC_ID_SWITCH_INVALID_FILTER_[]
    = RC_REPORT_MSG_TYPE "/switch/invalid_filter";

const char RC_ID_SWITCH_CONFLICT_[]
    = RC_REPORT_MSG_TYPE "/switch/conflict";

const char RC_ID_TIME_CONSUMED_WARNING_[]
    = RC_REPORT_MSG_TYPE "/time_consumed";

const char RC_ID_DELTAS_CONSUMED_WARNING_[]
    = RC_REPORT_MSG_TYPE "/delta_cycles_consumed";

const char RC_ID_PORTMAP_INIT_ERROR_[]
    = RC_REPORT_MSG_TYPE "/portmap/init_error";

const char RC_ID_PORTMAP_INCOMPATIBLE_ERROR_[]
    = RC_REPORT_MSG_TYPE "/portmap/incompatible_error";

const char RC_ID_SWITCH_CONNECTOR_INIT_ERROR_[]
    = RC_REPORT_MSG_TYPE "/switch_connector/init_error";

const char RC_ID_SWITCH_CONNECTOR_BINDING_ERROR_[]
    = RC_REPORT_MSG_TYPE "/switch_connector/binding_error";

const char RC_ID_SWITCH_CONNECTOR_INCOMPATIBLE_ERROR_[]
    = RC_REPORT_MSG_TYPE "/switch_connector/incompatible_error";

} // namespace ReChannel

//
// $Id: rc_report_ids.cpp,v 1.7 2007/10/09 21:47:01 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_report_ids.cpp,v $
//
