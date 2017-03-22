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
 * @file    rc_throwable.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   ABI rc_throwable.
 *
 *
 * $Date: 2007/10/14 04:47:29 $
 * $Revision: 1.7 $
 */

#include "rc_throwable.h"

namespace ReChannel {

    // throws a rc_throwable object
    void rc_throw(rc_throwable* t)
    {
        if (t == NULL) {
            RC_REPORT_FATAL(RC_ID_NULL_POINTER_,
                "rc_throw() may not throw the null pointer");
        }
        t->rc_throw();

        // this line shall never be reached
        RC_REPORT_FATAL(RC_ID_INTERNAL_ERROR_,
            "rc_throwable instance failed to throw itself");
    }

} // namespace ReChannel

//
// $Id: rc_throwable.cpp,v 1.7 2007/10/14 04:47:29 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_throwable.cpp,v $
//
