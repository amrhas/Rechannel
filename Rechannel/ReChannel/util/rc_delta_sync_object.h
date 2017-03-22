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
 * @file    rc_delta_sync_object.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class rc_delta_sync_object.
 *
 * $Date: 2007/10/24 23:55:23 $
 * $Revision: 1.5 $
 */

#ifndef RC_DELTA_SYNC_OBJECT_H_
#define RC_DELTA_SYNC_OBJECT_H_

#include <systemc.h>
#include <boost/function.hpp>

namespace ReChannel {

/**
 * @brief Helper channel to synchronise arbitrary operations
 *        with delta cycle borders.
 *
 * If this is object is enabled (see set_enabled()) a call to request_update()
 * will trigger the update of the channel at the end of the current delta cycle.
 * A user-specified callback function is then called during the following
 * update phase of SystemC. This mechanism allows operations to be
 * synchronised with the delta cycle transitions.
 */
class rc_delta_sync_object
    : protected sc_prim_channel
{
public:
    /**
     * @brief the type of the callback function
     *
     * When a callback is called, the parameter passes a reference to the
     * rc_delta_sync_object the call is originating from.
     */
    typedef boost::function<void (rc_delta_sync_object&)>
        delta_sync_callback_type;

public:
    /**
     * @brief default constructor
     *
     * Initially the channel is disabled and no callback is set.
     */
    rc_delta_sync_object();

    /**
     * @brief constructor setting the initial state
     * @param callback the user-specified callback that shall be called in case
     *        of an update.
     * @param enabled the inital activity state
     */
    rc_delta_sync_object(
        delta_sync_callback_type callback, bool enabled=false);

    /**
     * @brief Requests a call to the callback at the end of this delta cycle.
     */
    virtual void request_update();

    /**
     * @brief Returns if the channel is enabled.
     */
    inline bool is_enabled() const
        { return p_enabled; }

    /**
     * @brief Sets the activity state of the channel.
     */
    inline void set_enabled(bool b)
        { p_enabled = b; }

    /**
     * @brief Changes the user defined callback.
     */
    void set_callback(delta_sync_callback_type callback);

protected:
    /**
     * @brief Is called during update phase (if previously requested).
     */
    virtual void update();

private:
    /**
     * @brief the "enabled" flag
     */
    bool                     p_enabled;
    /**
     * @brief the user-specified callback
     */
    delta_sync_callback_type p_callback;

private:
    // disabled
    /** @brief //(disabled) */
    rc_delta_sync_object(const rc_delta_sync_object& orig);
    /** @brief //(disabled) */
    rc_delta_sync_object& operator=(const rc_delta_sync_object& orig);
};

} // namespace ReChannel

#endif // RC_DELTA_SYNC_OBJECT_H_

//
// $Id: rc_delta_sync_object.h,v 1.5 2007/10/24 23:55:23 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_delta_sync_object.h,v $
//
