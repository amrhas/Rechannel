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
 * @file    rc_process.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:35:40 $
 * $Revision: 1.9 $
 */

#include "rc_process.h"

namespace ReChannel {

rc_process_control::rc_process_control(
    bool is_initially_active, bool deactivate_silently)
:  p_activation_delta(0), p_cancel_flag(!is_initially_active),
   p_cancel_delta(0), p_deactivate_silently(deactivate_silently),
   p_parent(NULL)
{ }

rc_process_control::rc_process_control(
    const rc_process_control& parent, bool is_initially_active,
    bool deactivate_silently)
:  p_activation_delta(0), p_cancel_flag(!is_initially_active),
   p_cancel_delta(0), p_deactivate_silently(deactivate_silently),
   p_parent(&parent)
{
    parent.p_child_set.insert(this);
}

rc_process_control::~rc_process_control()
{
    if (p_parent != NULL) {
        p_parent->p_child_set.erase(this);
    }
    for (pctrl_set::iterator it=p_child_set.begin();
        it != p_child_set.end();
        ++it)
    {
        (*it)->p_parent = NULL;
    }
}

void rc_process_control::activate()
{
    if (sc_is_running()) {
        rc_notify(p_activation_event);
        p_activation_delta = sc_delta_count();
    }
    p_cancel_flag = false;

    // activate dependent process controls
    for (pctrl_set::iterator it=p_child_set.begin();
        it != p_child_set.end();
        ++it)
    {
        (*it)->activate();
    }
}

void rc_process_control::deactivate()
{
    if (sc_is_running()) {
        if (!p_deactivate_silently) {
            rc_notify(p_cancel_event);
        }
        p_cancel_delta = sc_delta_count();
    }
    p_cancel_flag = true;

    // deactivate dependent process controls
    for (pctrl_set::iterator it=p_child_set.begin();
        it != p_child_set.end();
        ++it)
    {
        rc_process_control* const pctrl = *it;
        pctrl->deactivate();
        if (pctrl->p_deactivate_silently) {
            //pctrl->p_cancel_event.notify(SC_ZERO_TIME);
            rc_notify(pctrl->p_cancel_event);
        }
    }
}

const sc_event rc_process_handle::s_never_notified;

rc_process_registry* rc_process_registry::s_instance = NULL;

rc_process_registry::rc_process_registry()
{}

rc_process_handle rc_process_registry::get_process_handle(
    const sc_process_handle& hproc, bool implicit_creation)
{
    process_map::iterator it =
        p_process_map.find(hproc.get_process_object());
    if (it != p_process_map.end()) {
        return rc_process_handle(&it->second);
    } else if (implicit_creation) {
        return this->register_process(hproc);
    } else {
        return rc_process_handle();
    }
}

rc_process_handle rc_process_registry::register_process(
    const sc_process_handle& hproc)
{
    if ((p_process_map.size() % gc_interval) == 0) {
        this->gc();
    }
    rc_process_info& pinfo = p_process_map[hproc.get_process_object()];
    pinfo.ref_count = 0;
    pinfo.handle = hproc;
    pinfo.dont_initialize = false;
    pinfo.is_cancelable = false;
    pinfo.is_terminated = false;
    pinfo.is_temporary = false;
    pinfo.cancel_event = NULL;
    pinfo.cancel_flag = NULL;
    pinfo.cancel_delta = NULL;
    pinfo.terminated_event = NULL;
    return rc_process_handle(pinfo);
}

rc_process_handle rc_process_registry::register_process(
    const sc_process_handle& hproc,
    bool dont_initialize, bool is_cancelable,
    const rc_process_control& pctrl, bool is_temporary)
{
    if ((p_process_map.size() % gc_interval) == 0) {
        this->gc();
    }
    rc_process_info& pinfo = p_process_map[hproc.get_process_object()];
    pinfo.ref_count = 0;
    pinfo.handle = hproc;
    pinfo.dont_initialize = dont_initialize;
    pinfo.is_cancelable = is_cancelable;
    pinfo.is_terminated = false;
    pinfo.is_temporary = is_temporary;
    pinfo.cancel_event = &pctrl.p_cancel_event;
    pinfo.cancel_flag = &pctrl.p_cancel_flag;
    pinfo.cancel_delta = &pctrl.p_cancel_delta;
    pinfo.terminated_event = new sc_event(); // will be deleted by gc()
    return rc_process_handle(pinfo);
}

rc_process_handle rc_process_registry::register_process(
    const sc_process_handle& hproc,
    bool dont_initialize, bool is_cancelable,
    const sc_event* cancel_event, const bool* cancel_flag,
    const sc_dt::uint64* cancel_delta, bool is_temporary)
{
    if ((p_process_map.size() % gc_interval) == 0) {
        this->gc();
    }
    rc_process_info& pinfo = p_process_map[hproc.get_process_object()];
    pinfo.ref_count = 0;
    pinfo.handle = hproc;
    pinfo.dont_initialize = dont_initialize;
    pinfo.is_cancelable = is_cancelable;
    pinfo.is_terminated = false;
    pinfo.is_temporary = is_temporary;
    pinfo.cancel_event = cancel_event;
    pinfo.cancel_flag = cancel_flag;
    pinfo.cancel_delta = cancel_delta;
    pinfo.terminated_event = new sc_event(); // will be deleted by gc()
    return rc_process_handle(pinfo);
}

void rc_process_registry::gc()
{
    for(process_map::iterator it = p_process_map.begin();
        it != p_process_map.end();
        ++it)
    {
        rc_process_info& pinfo = it->second;
        // delete process if it is unreferenced and physically terminated
        if (pinfo.ref_count == 0 && pinfo.handle.terminated()) {
            if (pinfo.terminated_event != NULL) {
                delete pinfo.terminated_event;
            }
            p_process_map.erase(it);
        }
    }
}

void rc_process_registry::_rc_create_singleton()
{
    if (rc_process_registry::s_instance == NULL) {
        rc_process_registry::s_instance = new rc_process_registry();
    }
}

void rc_process_registry::_rc_wait_event_and_list_proc(
    sc_event_and_list& el,
    sc_event* continue_event)
{
    std::auto_ptr<sc_event> delete_on_exit(continue_event);
    ::sc_core::wait(el);
    continue_event->notify();

    ::sc_core::wait(SC_ZERO_TIME); // (only for safety reasons)
    // delete continue_event
}

void rc_process_registry::_rc_wait_event_and_list_proc(
    const sc_time& t, sc_event_and_list& el,
    sc_event* continue_event)
{
    std::auto_ptr<sc_event> delete_on_exit(continue_event);
    ::sc_core::wait(t, el);
    continue_event->notify();

    ::sc_core::wait(SC_ZERO_TIME); // (only for safety reasons)
    // delete continue_event
}

bool rc_process_handle::terminate()
{
    if (p_proc_info != NULL
    && p_proc_info->terminated_event != NULL) {
        if (p_proc_info->is_terminated) {
            return true;
        }
        p_proc_info->is_terminated = true;
        p_proc_info->terminated_event->notify();
        throw new rc_process_cancel_exception();
    }
    return false;
}

void rc_process_handle::wait_terminated(const sc_event& reactivation_event)
{
    if (p_proc_info != NULL
    && p_proc_info->terminated_event != NULL) {
        if (!p_proc_info->is_terminated) {
            p_proc_info->is_terminated = true;
            p_proc_info->terminated_event->notify();
        }
        ::sc_core::wait(reactivation_event);
        p_proc_info->is_terminated = false;
    } else {
        ::sc_core::wait(reactivation_event);
    }
}

rc_spawn_options::rc_spawn_options()
    : p_is_dont_initialize(false)
{ }

void rc_spawn_options::set_reset_signal(
    const sc_in<bool>& reset_port_, bool active_level_)
{
    p_reset_port_vector.push_back(
        reset_port_vector::value_type(&reset_port_, active_level_));
}

void rc_spawn_options::set_reset_signal(
    const sc_signal<bool>& reset_signal_, bool active_level_)
{
    p_reset_signal_vector.push_back(
        reset_signal_vector::value_type(&reset_signal_, active_level_));
}

void rc_spawn_options::set_reset_signal(
    rc_reset_channel_if& reset_channel_, bool active_level_)
{
    p_reset_channel_vector.push_back(
        reset_channel_vector::value_type(&reset_channel_, active_level_));
}

void rc_spawn_options::set_reset(rc_process_reset& process_reset_)
{
    p_process_reset_vector.push_back(&process_reset_);
}

void rc_next_trigger()
{
    sc_core::next_trigger();
}

void rc_next_trigger(const sc_event& e)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        sc_core::next_trigger(e | hproc.get_cancel_trigger_event());
    } else {
        sc_core::next_trigger(e);
    }
}

void rc_next_trigger(sc_event_or_list& el)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        sc_core::next_trigger(el | hproc.get_cancel_trigger_event());
    } else {
        sc_core::next_trigger(el);
    }
}

void rc_next_trigger(sc_event_and_list& el)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        // omit time and effort if the process was already canceled
        if (!hproc.is_canceled()) {
            std::auto_ptr<sc_event> continue_event(new sc_event());
            sc_spawn(
                sc_bind(
                    &rc_process_registry::_rc_wait_event_and_list_proc,
                    sc_ref(el), continue_event.get()));
            sc_core::next_trigger(
                *(continue_event.release()) | hproc.get_cancel_trigger_event());
            // (continue_event will be deleted by the spawned process)
        } else {
            sc_core::next_trigger(el);
        }
    } else {
        sc_core::next_trigger(el);
    }
}

void rc_next_trigger(const sc_time& t)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        sc_core::next_trigger(t, hproc.get_cancel_trigger_event());
    } else {
        sc_core::next_trigger(t);
    }
}

void rc_next_trigger(double v, sc_time_unit tu)
{
    rc_next_trigger(sc_time(v, tu));
}

void rc_next_trigger(const sc_time& t, const sc_event& e)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        sc_core::next_trigger(t, e | hproc.get_cancel_trigger_event());
    } else {
        sc_core::next_trigger(t, e);
    }
}

void rc_next_trigger(double v, sc_time_unit tu, const sc_event& e)
{
    rc_next_trigger(sc_time(v, tu), e);
}

void rc_next_trigger(const sc_time& t, sc_event_or_list& el)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        sc_core::next_trigger(t, el | hproc.get_cancel_trigger_event());
    } else {
        sc_core::next_trigger(t, el);
    }
}

void rc_next_trigger(double v, sc_time_unit tu, sc_event_or_list& el)
{
    rc_next_trigger(sc_time(v, tu), el);
}

void rc_next_trigger(const sc_time& t, sc_event_and_list& el)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        // omit time and effort if the process was already canceled
        if (!hproc.is_canceled()) {
            std::auto_ptr<sc_event> continue_event(new sc_event());
            sc_spawn(
                sc_bind(
                    &rc_process_registry::_rc_wait_event_and_list_proc,
                    sc_cref(t), sc_ref(el), continue_event.get()));
            sc_core::next_trigger(
                t, (*(continue_event.release())
                | hproc.get_cancel_trigger_event()));
            // (continue_event will be deleted by the spawned process)
        } else {
            sc_core::next_trigger(t, el);
        }
    } else {
        sc_core::next_trigger(t, el);
    }
}

void rc_next_trigger(double v, sc_time_unit tu, sc_event_and_list& el)
{
    rc_next_trigger(sc_time(v, tu), el);
}

void rc_wait()
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        if (hproc.is_canceled()) {
            throw new rc_process_cancel_exception();
        } else {
            ::sc_core::wait();
            if (hproc.is_canceled() || hproc.is_cancel_event()) {
                throw new rc_process_cancel_exception();
            }
        }
    } else {
        ::sc_core::wait();
    }
}

void rc_wait(int n)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable() && n > 0) {
        if (hproc.is_canceled()) {
            throw new rc_process_cancel_exception();
        } else {
            for (int i=0; i < n; i++) {
                ::sc_core::wait();
                if (hproc.is_canceled() || hproc.is_cancel_event()) {
                    throw new rc_process_cancel_exception();
                }
            }
        }
    } else {
        ::sc_core::wait(n);
    }
}

void rc_wait(const sc_event& e)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        if (hproc.is_canceled()) {
            throw new rc_process_cancel_exception();
        } else {
            ::sc_core::wait(e | hproc.get_cancel_trigger_event());
            if (hproc.is_canceled() || hproc.is_cancel_event()) {
                throw new rc_process_cancel_exception();
            }
        }
    } else {
        ::sc_core::wait(e);
    }
}

void rc_wait(sc_event_or_list& el)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        if (hproc.is_canceled()) {
            throw new rc_process_cancel_exception();
        } else {
            ::sc_core::wait(el | hproc.get_cancel_trigger_event());
            if (hproc.is_canceled() || hproc.is_cancel_event()) {
                throw new rc_process_cancel_exception();
            }
        }
    } else {
        ::sc_core::wait(el);
    }
}

void rc_wait(sc_event_and_list& el)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        if (hproc.is_canceled()) {
            throw new rc_process_cancel_exception();
        } else {
            std::auto_ptr<sc_event> continue_event(new sc_event());
            sc_spawn_options opt;
            opt.set_stack_size(RC_PROCESS_TEMP_PROC_STACK_SIZE);
            sc_spawn(
                sc_bind(
                    &rc_process_registry::_rc_wait_event_and_list_proc,
                    sc_ref(el), continue_event.get()),
                NULL, &opt);
            ::sc_core::wait(
                *(continue_event.release()) | hproc.get_cancel_trigger_event());
            // (continue_event will be deleted by the spawned process)

            if (hproc.is_canceled() || hproc.is_cancel_event()) {
                throw new rc_process_cancel_exception();
            }
        }
    } else {
        ::sc_core::wait(el);
    }
}

void rc_wait(const sc_time& t)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        if (hproc.is_canceled()) {
            throw new rc_process_cancel_exception();
        } else {
            ::sc_core::wait(t, hproc.get_cancel_trigger_event());
            if (hproc.is_canceled() || hproc.is_cancel_event()) {
                throw new rc_process_cancel_exception();
            }
        }
    } else {
        ::sc_core::wait(t);
    }
}

void rc_wait(double v, sc_time_unit tu)
{
    rc_wait(sc_time(v, tu));
}

void rc_wait(const sc_time& t, const sc_event& e)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        if (hproc.is_canceled()) {
            throw new rc_process_cancel_exception();
        } else {
            ::sc_core::wait(t, e | hproc.get_cancel_trigger_event());
            if (hproc.is_canceled() || hproc.is_cancel_event()) {
                throw new rc_process_cancel_exception();
            }
        }
    } else {
        ::sc_core::wait(t, e);
    }
}

void rc_wait(double v, sc_time_unit tu, const sc_event& e)
{
    rc_wait(sc_time(v, tu), e);
}

void rc_wait(const sc_time& t, sc_event_or_list& el)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        if (hproc.is_canceled()) {
            throw new rc_process_cancel_exception();
        } else {
            ::sc_core::wait(t, el | hproc.get_cancel_trigger_event());
            if (hproc.is_canceled() || hproc.is_cancel_event()) {
                throw new rc_process_cancel_exception();
            }
        }
    } else {
        ::sc_core::wait(t, el);
    }
}

void rc_wait(double v, sc_time_unit tu, sc_event_or_list& el)
{
    rc_wait(sc_time(v, tu), el);
}

void rc_wait(const sc_time& t, sc_event_and_list& el)
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (hproc.is_cancelable()) {
        if (hproc.is_canceled()) {
            throw new rc_process_cancel_exception();
        } else {
            std::auto_ptr<sc_event> continue_event(new sc_event());
            sc_spawn_options opt;
            opt.set_stack_size(RC_PROCESS_TEMP_PROC_STACK_SIZE);
            sc_spawn(
                sc_bind(
                    &rc_process_registry::_rc_wait_event_and_list_proc,
                    sc_cref(t), sc_ref(el), continue_event.get()),
                NULL, &opt);
            ::sc_core::wait(
                t, (*(continue_event.release())
                | hproc.get_cancel_trigger_event()));
            // (continue_event will be deleted by the spawned process)

            if (hproc.is_canceled() || hproc.is_cancel_event()) {
                throw new rc_process_cancel_exception();
            }
        }
    } else {
        ::sc_core::wait(t, el);
    }
}

void rc_wait(double v, sc_time_unit tu, sc_event_and_list& el)
{
    rc_wait(sc_time(v, tu), el);
}

bool rc_is_update_phase()
{
    static bool          s_is_update_phase = false;
    static sc_dt::uint64 s_last_delta = 0;
    static sc_event      s_test_event;

    if (s_is_update_phase == false
    || s_last_delta != sc_delta_count()) {
        try {
            s_test_event.notify();
            return false;
        } catch(sc_report r) {
            s_is_update_phase = true;
            s_last_delta = sc_delta_count();
        }
    }
    return true;
}

void rc_notify(sc_event& e)
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

} // namespace ReChannel

//
// $Id: rc_process.cpp,v 1.9 2007/12/20 20:35:40 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_process.cpp,v $
//
