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
 * @file    rc_mutex_object.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class rc_mutex_object
 *
 *
 * $Date: 2007/12/06 11:42:43 $
 * $Revision: 1.7 $
 */

#ifndef RC_MUTEX_OBJECT_H_
#define RC_MUTEX_OBJECT_H_

#include <systemc.h>

namespace ReChannel {

/** @brief Implementation of a mutex object.
 *
 * rc_mutex_object is ReChannl's light-weight mutex implementation.
 * sc_mutex and its interface sc_mutex_if is not appropriate for an
 * internally used helper class.
 */
class rc_mutex_object
{
public:
    rc_mutex_object();

    /** @brief Try to lock the mutex for the current process.
     *
     * Tries to lock the mutex for the current process.
     * trylock() is non-blocking and returns immediately.
     * @return true if succeeded, false otherwise
     */
    bool trylock();

    /** @brief Lock the mutex for the current process.
     *
     * Waits until a lock for the current process can be acquired.
     */
    void lock();

    /** @brief Lock the mutex for the current process (with timeout).
     *
     * Waits until a lock for the current process can be acquired.
     * timeout is specified to cancel waiting after this delay.
     * @return true if lock was acquired, false if the timeout occured.
     */
    bool lock(sc_time timeout);

    /** @brief Unlock the mutex for the current process.
     *
     * Unlocks the mutex for the current process.
     * unlock() is non-blocking and returns immediately.
     * @return true if process had previously hold a lock, false otherwise.
     */
    bool unlock();

    /** @brief Does the current process hold a lock?
     * @return true if hold is locked by current process, false otherwise
     */
    bool has_lock() const;

    /** @brief Is this mutex locked by a process?
     * @return true if this mutex is locked, false otherwise
     */
    inline bool is_locked() const
        { return (m_lock_count > 0); }

    /** @brief The lock release event
     * @return the lock release event
     */
    inline const sc_event& get_lock_release_event() const
        { return m_lock_release_event; }

protected:
    /** @brief Does the given process hold a lock?
     * @param hproc the process handle
     * @return true if hold is locked by the given process, false otherwise
     */
    inline bool is_lock_owner(const sc_process_handle& hproc) const;

protected:
    /** @brief the process currently holding a lock on this mutex */
    sc_process_handle m_lock_owner;
    /** @brief the number of locks on this mutex */
    int               m_lock_count;
    /** @brief the event that is notified if a lock is released */
    sc_event          m_lock_release_event;

private:
    // disabled
    rc_mutex_object(const rc_mutex_object& orig);
    const rc_mutex_object& operator=(const rc_mutex_object& orig);
};

inline bool rc_mutex_object::is_lock_owner(
    const sc_process_handle& hproc) const
{
    return (m_lock_owner == hproc
        || (!hproc.valid() && !m_lock_owner.valid()));
}

} // namespace ReChannel

#endif //RC_MUTEX_OBJECT_H_

//
// $Id: rc_mutex_object.h,v 1.7 2007/12/06 11:42:43 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_mutex_object.h,v $
//
