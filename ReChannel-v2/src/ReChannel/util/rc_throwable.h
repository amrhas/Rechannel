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
 * @file    rc_throwable.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   ABI rc_throwable.
 *
 *
 * $Date: 2007/10/14 04:47:29 $
 * $Revision: 1.6 $
 */

#ifndef RC_THROWABLE_H_
#define RC_THROWABLE_H_

#include "ReChannel/util/rc_report.h"

namespace ReChannel {

/**
 * @brief rc_throwable is the interface of a polymorphic error object.
 *
 * By implementing this interface a class declares itself to be throwable.
 * It supports polymorphic throws which is necessary for rethrowing an error.
 *
 * Constraints on usage:
 * - rc_throwable objects must be thrown as a pointer
 * - rc_throwable objects shall be deleted in the consuming catch block
 * - rc_throwable objects are rethrown either by method
 *   rc_throwable::rc_throw() or by global function #rc_throw()
 */
class rc_throwable
{
public:
    /**
     * @brief Throws a pointer of this instance (preserving the type).
     **/
    virtual void rc_throw() = 0;

    /**
     * @brief The virtual destructor.
     **/
    virtual ~rc_throwable() {}
};

/**
* @brief This function throws a rc_throwable object (polymorphically).
* @param t A pointer to an object of type rc_throwable.
* @exception RC_ID_NULL_POINTER_ if t is NULL.
* @exception RC_ID_INTERNAL_ERROR if rc_throwable object failed to throw itself
* @relates rc_throwable
**/
extern void rc_throw(rc_throwable* t);

} // namespace ReChannel

#endif // RC_THROWABLE_H_

//
// $Id: rc_throwable.h,v 1.6 2007/10/14 04:47:29 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_throwable.h,v $
//

