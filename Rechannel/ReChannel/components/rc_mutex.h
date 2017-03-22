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
 * @file    rc_mutex.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:24:55 $
 * $Revision: 1.4 $
 */

#ifndef RC_MUTEX_H_
#define RC_MUTEX_H_

#include "ReChannel/components/rc_prim_channel.h"

namespace ReChannel {

/**
 * @brief Resettable representation of sc_mutex.
 */
class rc_mutex
    : public sc_mutex_if,
      public rc_prim_channel
{
public:

    rc_mutex();

    explicit rc_mutex(const char* name_);

    inline virtual const char* kind() const
        { return "rc_mutex"; }

    virtual int lock();

    virtual int trylock();

    virtual int unlock();

protected:

    virtual void rc_on_reset();    

    inline bool is_locked() const
        { return m_lock_owner.valid(); }

protected:

    sc_process_handle m_lock_owner;
    sc_event          m_free;

private:
    // disabled
    rc_mutex(const rc_mutex& other);
    rc_mutex& operator=(const rc_mutex& other);
};

} // namespace ReChannel

#endif //RC_MUTEX_H_

//
// $Id: rc_mutex.h,v 1.4 2007/11/23 13:24:55 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_mutex.h,v $
//

