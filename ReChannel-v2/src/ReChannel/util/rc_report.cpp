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
 * @file    rc_report.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   ReChannel's error reporting and assertion functionality.
 *
 *
 * $Date: 2007/10/09 00:22:26 $
 * $Revision: 1.4 $
 */

#include "rc_report.h"

namespace ReChannel {

// predefined message types
const char RC_ID_INTERNAL_ERROR_[]   =
    RC_REPORT_MSG_TYPE "/internal_error";

const char RC_ID_UNKNOWN_ERROR_[]    =
    RC_REPORT_MSG_TYPE "/unknown_error";

const char RC_ID_NOT_IMPLEMENTED_[]  =
    RC_REPORT_MSG_TYPE "/not_implemented";

const char RC_ID_OUT_OF_BOUNDS_[]    =
    RC_REPORT_MSG_TYPE "/out_of_bounds";

const char RC_ID_NULL_POINTER_[]     =
    RC_REPORT_MSG_TYPE "/null_pointer";

const char RC_ID_INVALID_USAGE_[]    =
    RC_REPORT_MSG_TYPE "/invalid_usage";

const char RC_ID_DUPLICATE_ENTRY_[]  =
    RC_REPORT_MSG_TYPE "/duplicate_entry";

const char RC_ID_ASSERTION_FAILED_[] =
    RC_REPORT_MSG_TYPE "/assertion_failed";

} // namespace ReChannel

//
// $Id: rc_report.cpp,v 1.4 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_report.cpp,v $
//

