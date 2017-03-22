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
 * @file    rc_driver_object.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Driver object classes.
 *
 *
 * $Date: 2007/10/09 00:22:26 $
 * $Revision: 1.4 $
 */

#include "rc_driver_object.h"

namespace ReChannel {

// static variables
sc_process_handle rc_driver_object_base::s_hproc;

rc_driver_object_base::write_call_vector
    rc_driver_object_base::s_write_call_vector;

sc_event rc_driver_object_base::s_pending_write_event;

bool rc_driver_object_base::s_pending_write_event_notified_flag = false;

// shared static driver method process
void rc_driver_object_base::s_writer_method_proc()
{
    // the loop that executes the pending write calls
    for (write_call_iterator it = s_write_call_vector.begin();
     it != s_write_call_vector.end();
     ++it) {
        it->m_call();
    }
    // clear the write call vector
    s_write_call_vector.clear();

    // consume event
    s_pending_write_event_notified_flag = false;

    // prevent accidental change of sensitivity
    sc_core::next_trigger();
}

// constructor
rc_driver_object_base::rc_driver_object_base()
    : m_write_call_vector(&s_write_call_vector),
      m_pending_write_event(&s_pending_write_event),
      m_pending_write_event_notified_flag(false),
      m_pending_write_event_notified(&s_pending_write_event_notified_flag)
{
    // create the static driver method process if it does not exist
    if (s_hproc.valid() == false) {
        sc_spawn_options opt;
        opt.set_sensitivity(&s_pending_write_event);
        opt.dont_initialize();
        opt.spawn_method();
        s_hproc = sc_spawn(
            &rc_driver_object_base::s_writer_method_proc,
            sc_gen_unique_name("_rc_shared_nb_driver_object_proc"),
            &opt);
    }
    m_hproc = s_hproc;
}

rc_driver_object_base::~rc_driver_object_base()
{
    // destroy the created objects
    if (m_write_call_vector != &s_write_call_vector) {
        delete m_write_call_vector;
        m_write_call_vector = NULL;
    }
    if (m_pending_write_event != &s_pending_write_event) {
        delete m_pending_write_event;
        m_pending_write_event = NULL;
    }
}

void rc_driver_object_base::safe_event_notify(sc_event& e)
{
    static bool          s_is_update_phase = false;
    static sc_dt::uint64 s_last_delta = 0;

    if (s_is_update_phase == true) {
        if (s_last_delta == sc_delta_count()) {
            e.notify(SC_ZERO_TIME);
            return;
        } else {
            s_is_update_phase = false;
        }
    }
    try {
        e.notify();
    } catch(sc_report r) {
        s_is_update_phase = true;
        s_last_delta = sc_delta_count();
        e.notify(SC_ZERO_TIME);
    }
}

namespace internals {
namespace driver_object {

void nb_driver_access_call::operator()(
    driver_object_type& dobj, call_type call_obj)
{
    // create the write call and append it to the write call vector
    dobj.m_write_call_vector->push_back(write_call(call_obj));
    // signal the driver object that a new write call is pending
    //   by notifying the pending write event
    if (!*dobj.m_pending_write_event_notified) {
        *dobj.m_pending_write_event_notified = true;    
        rc_driver_object_base::safe_event_notify(
            *dobj.m_pending_write_event);
    }
}

void driver_access_call<void>::operator()(
    driver_object_type& dobj, call_type call_obj) const
{
    // create the event that will indicate the execution of the call
    sc_event write_done_event;
    // initialise the thrown_obj to NULL
    rc_throwable* thrown_obj = NULL;
    // create the write call and append it to the write call vector
    dobj.m_write_call_vector->push_back(
        write_call(call_obj, &write_done_event, &thrown_obj));
    // signal the driver object that a new write call is pending
    //   by notifying the pending write event
    if (!*dobj.m_pending_write_event_notified) {
        *dobj.m_pending_write_event_notified = true;
        rc_driver_object_base::safe_event_notify(
            *dobj.m_pending_write_event);
    }
    // wait for the call to be executed
    ::sc_core::wait(write_done_event);
    // if an event of type rc_throwable occured rethrow it in this context
    if (thrown_obj != NULL) {
        ::ReChannel::rc_throw(thrown_obj);
    }
    // execution was successful
}

} // namespace driver_object
} // namespace internals

} // namespace ReChannel

//
// $Id: rc_driver_object.cpp,v 1.4 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_driver_object.cpp,v $
//
