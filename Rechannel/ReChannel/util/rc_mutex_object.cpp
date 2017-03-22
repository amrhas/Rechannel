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
 * @file    rc_mutex_object.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/06 11:42:43 $
 * $Revision: 1.7 $
 */

#include "rc_mutex_object.h"

namespace ReChannel {

rc_mutex_object::rc_mutex_object()
    : m_lock_count(0)
{ }

bool rc_mutex_object::trylock()
{
    const sc_process_handle hproc = sc_get_current_process_handle();
    // is mutex unlocked?
    if (m_lock_count == 0 || this->is_lock_owner(hproc)) {
        // hproc now holds the lock
        m_lock_owner = hproc;
        m_lock_count++;
        return true;
    } else {
        return false;
    }
}

void rc_mutex_object::lock()
{
    const sc_process_handle hproc = sc_get_current_process_handle();
    // if lock is already hold by hproc, skip the lock procedure
    while(!this->is_lock_owner(hproc)) {
        // is mutex unlocked?
        if (m_lock_count == 0) {
            // hproc now holds the lock
            m_lock_owner = hproc;
            break;
        }
        // wait for a lock release
        ::sc_core::wait(m_lock_release_event);
    }
    // lock is acquired
    m_lock_count++;
}

bool rc_mutex_object::lock(sc_time timeout)
{
    const sc_time stoptime = sc_time_stamp() + timeout;
    const sc_process_handle hproc = sc_get_current_process_handle();
    // if lock is already hold by hproc, skip the lock procedure
    while(this->is_lock_owner(hproc)) {
        // is mutex unlocked?
        if (m_lock_count == 0) {
            // hproc now holds the lock
            m_lock_owner = hproc;
            break;
        }
        const sc_time curtime = sc_time_stamp();
        if (curtime >= stoptime) {
            return false;
        }
        // wait for a lock release or the remaining time
        ::sc_core::wait(stoptime - curtime, m_lock_release_event);
    }
    // lock is acquired
    m_lock_count++;
    return true;
}

bool rc_mutex_object::unlock()
{
    if (m_lock_count == 0) {
        return false;
    }
    const sc_process_handle hproc = sc_get_current_process_handle();
    // unlock if at least one lock is hold by hproc
    if (this->is_lock_owner(hproc)) {
        m_lock_count--;
        if (m_lock_count == 0) {
            // release the lock
            m_lock_owner = sc_process_handle();
            m_lock_release_event.notify();
        }
        return true;
    } else {
        return false;
    }
}

bool rc_mutex_object::has_lock() const
{
    const sc_process_handle hproc = sc_get_current_process_handle();
    return (m_lock_count > 0 && this->is_lock_owner(hproc));
}

}

//
// $Id: rc_mutex_object.cpp,v 1.7 2007/12/06 11:42:43 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_mutex_object.cpp,v $
//
