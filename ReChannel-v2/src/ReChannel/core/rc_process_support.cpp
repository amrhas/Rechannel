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
 * @file    rc_process_support.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:37:29 $
 * $Revision: 1.15 $
 */

#include "rc_process_support.h"

namespace ReChannel {
namespace internals {

process_support::process_info::reset_signal_map
    process_support::process_info::p_reset_signal_map;

const rc_reset_channel_if&

process_support::process_info::_rc_get_reset_channel(
    const sc_signal_in_if<bool>& signal_interface)
{
    const rc_reset_channel_if* reset_channel =
        dynamic_cast<const rc_reset_channel_if*>(&signal_interface);
    if (reset_channel == NULL) {
        reset_signal*& signal_ = p_reset_signal_map[&signal_interface];
        if (signal_ == NULL) {
            signal_ = new reset_signal(
                sc_gen_unique_name("_rc_process_support_reset_signal"));
            // (reset_signal will be deleted by SystemC)

            sc_spawn_options opt;
            {
                opt.spawn_method();
                opt.set_sensitivity(
                    &signal_interface.value_changed_event());
            }
            sc_spawn(
                sc_bind(
                    &process_info::_rc_reset_updater_proc,
                    sc_cref(signal_interface), sc_ref(*signal_)),
                sc_gen_unique_name(
                    "_rc_process_support_reset_updater_proc"),
                &opt);
        }
        reset_channel = static_cast<const rc_reset_channel_if*>(signal_);
    }
    return *reset_channel;
}

void process_support::process_info::_rc_reset_updater_proc(
    const sc_signal_in_if<bool>& from_, reset_signal& to_)
{
    to_.write(from_.read());
}

process_support::process_info::process_info(
    boost::function<void (void)> func_,
    const rc_process_control& parent_pctrl, bool is_thread_,
    bool is_dont_initialize, bool has_sync_reset)
    : func(func_), process_control(
          parent_pctrl, parent_pctrl.is_active(), has_sync_reset),
      p_is_thread(is_thread_), p_is_dont_initialize(false),
      p_is_init_run(false)
{ }

process_support::process_info::~process_info()
{
    for (int level = 0; level <= 1; ++level) {
        reset_channel_vector& vector = p_reset_channels[level];
        unsigned int count = vector.size();
        for (unsigned int i=0; i < count; ++i) {
            process_support::unregister_process_control(
                *vector[i], process_control);
        }
    }
}

void process_support::process_info::register_reset_channel(
    const rc_reset_channel_if& reset_channel, bool active_level)
{
    p_reset_channels[(active_level ? 1 : 0)].push_back(&reset_channel);
    process_support::register_process_control(
        reset_channel, process_control, active_level);
}

void process_support::process_info::register_reset_interface(
    const sc_signal_in_if<bool>& reset_interface, bool active_level)
{
    const rc_reset_channel_if& reset_channel =
        process_info::_rc_get_reset_channel(reset_interface);
    this->register_reset_channel(reset_channel, active_level);
}

void process_support::process_info::register_reset_signal(
    const sc_signal<bool>& reset_signal, bool active_level)
{
    const rc_reset_channel_if& reset_channel =
        process_info::_rc_get_reset_channel(reset_signal);
    this->register_reset_channel(reset_channel, active_level);
}

void process_support::process_info::register_reset_port(
    const sc_in<bool>& reset_port, bool active_level)
{
    const sc_signal_in_if<bool>* const reset_interface =
        reset_port.get_interface(0);
    if (reset_interface != NULL) {
        const rc_reset_channel_if& reset_channel =
            process_info::_rc_get_reset_channel(*reset_interface);
        this->register_reset_channel(reset_channel, active_level);
    } else {
        p_reset_ports[(active_level ? 1 : 0)].push_back(&reset_port);
    }
}

void process_support::process_info::register_reset_port_interfaces()
{
    for (int level = 0; level <= 1; ++level) {
        reset_port_vector& reset_ports = p_reset_ports[level];
        const unsigned int count = reset_ports.size();
        for (unsigned int i=0; i < count; ++i) {
            const sc_in<bool>* const reset_port = reset_ports[i];
            if (reset_port->get_interface() != NULL) {
                this->register_reset_port(*reset_port, level);
            }
        }
        reset_ports.clear();
    }
}

void process_support::process_info::update_process_control_state()
{
    for (int level = 0; level <= 1; ++level) {
        reset_channel_vector& reset_channels = p_reset_channels[level];
        const unsigned int count = reset_channels.size();
        for (unsigned int i=0; i < count; ++i) {
            if (process_support::get_current_level(*reset_channels[i])
            == (bool)level) {
                if (process_control.is_active()) {
                    process_control.deactivate();
                }
                return;
            }
        }
    }
    if (!process_control.is_active()) {
        process_control.activate();
    }
}

process_support::process_support()
    : sc_prim_channel(sc_gen_unique_name("_rc_process_support")),
      p_reconf(
          process_support::_rc_find_context(this->get_parent_object())),
      p_last_pinfo(NULL), p_last_host(NULL),
      p_last_host_child_count(0)
{
    // check if process support is used within a resettable module
    sc_object* parent = this->get_parent_object();
    if (dynamic_cast<sc_module*>(parent) == NULL
    || dynamic_cast<rc_resettable*>(parent) == NULL)
    {
        RC_REPORT_ERROR("RC_ID_INVALID_USAGE_",
                "Resettable processes are only allowed within"
                " resettable modules (in '" << this->name() << "')");
    }

    /* ensure that the rc_non_reconfigurable instance exists
       before it is used during simulation */
    rc_non_reconfigurable::get_instance();
}

void process_support::declare_process(
    const sc_object& host,
    const boost::function<void (void)>& declare_proc_callback,
    const boost::function<void (void)>& func, sc_sensitive& sensitive,
    bool is_thread, bool dont_initialize, bool has_sync_reset)
{
    // store the current process object before the creation
    sc_object* const proc_obj_before =
        sc_get_current_process_handle().get_process_object();

    // suppress 'object name already exists' warning
    const sc_actions actions =
        sc_report_handler::set_actions(
            SC_WARNING, SC_DO_NOTHING);

    // invoke the callback to let it create the process
    declare_proc_callback();

    // reset report action
    sc_report_handler::set_actions(SC_WARNING, actions);

    sc_object* const proc_obj =
        sc_get_current_process_handle().get_process_object();

    // check if process was created correctly by declare_proc_callback()
    if (proc_obj == NULL || proc_obj == proc_obj_before) {
        // declare_proc_callback() didn't do what was expected from it
        RC_REPORT_ERROR(RC_ID_INVALID_USAGE_,
            "invalid usage (in '" << this->name() << "')");
    }

    const rc_process_control& reconf_pctrl =
        p_reconf.rc_get_process_control();

    process_info* const pinfo =
        new process_info(
            func, reconf_pctrl, is_thread, dont_initialize,
            has_sync_reset);
    p_reconf_pinfo_map[proc_obj] = pinfo;
    p_last_pinfo = pinfo;
    p_last_host = &host;
    p_last_host_child_count = host.get_child_objects().size();

    sensitive << (pinfo->process_control.get_deactivation_event());
}

void process_support::method_process()
{
    const sc_process_handle curr_proc = sc_get_current_process_handle();
    process_info* const pinfo =
        this->_rc_find_process_info(curr_proc.get_process_object());

    if (pinfo == NULL || curr_proc.proc_kind() != SC_METHOD_PROC_ )
    {
        RC_REPORT_ERROR(RC_ID_INVALID_USAGE_,
            "invalid usage (in '" << this->name() << "')");
    }

    const rc_process_control& reconf_pctrl =
        p_reconf.rc_get_process_control();

    rc_process_registry& preg = rc_get_process_registry();
    rc_process_handle hproc = preg.get_process_handle(curr_proc, false);
    if (!hproc.valid()) {
        hproc = preg.register_process(
            curr_proc, pinfo->is_dont_initialize(), true,
            pinfo->process_control);
    }

    if (reconf_pctrl.is_active()) {
        if (pinfo->is_init_run() && pinfo->is_dont_initialize()) {
            pinfo->set_init_run(false);
            // exit normally
            return;
        }
        try {
            // call the user method
            pinfo->func();
            // exit normally
            return;
        } catch(rc_process_cancel_exception* e) {
            delete e; // consume exception
        }
    }

    if (pinfo->is_dont_initialize()) {
        pinfo->set_init_run(true);
    }

    ::sc_core::next_trigger(reconf_pctrl.get_activation_event());
}

void process_support::thread_process()
{
    const sc_process_handle curr_proc = sc_get_current_process_handle();
    const std::auto_ptr<process_info> pinfo(
        this->_rc_fetch_process_info(curr_proc.get_process_object()));

    if (pinfo.get() == NULL || curr_proc.proc_kind() != SC_THREAD_PROC_ )
    {
        RC_REPORT_ERROR(RC_ID_INVALID_USAGE_,
            "invalid usage (in '" << this->name() << "')");
    }

    rc_process_registry& preg = rc_get_process_registry();
    rc_process_handle hproc = preg.get_process_handle(curr_proc, false);
    if (!hproc.valid()) {
        hproc = preg.register_process(
            curr_proc, pinfo->is_dont_initialize(), true,
            pinfo->process_control);
    }

    const rc_process_control& reconf_pctrl =
        p_reconf.rc_get_process_control();

    pinfo->update_process_control_state();

    bool is_dont_initialize_allowed = false;
    while(true)
    {
        bool has_exited_normally = false;

        if (reconf_pctrl.is_active()) {
            try {
                if (is_dont_initialize_allowed
                    && pinfo->is_dont_initialize())
                {
                    // wait for next trigger
                    rc_wait();
                }
                // call the actual process function
                pinfo->func();
                // process exited normally
                has_exited_normally = true;
            } catch(rc_process_cancel_exception* e) {
                delete e; // consume exception
            }
        }

        // has the reconf. been deactivated or has the process terminated?
        if (!reconf_pctrl.is_active()
        || has_exited_normally
        || hproc.terminated())
        {
            // is process temporary?
            if (hproc.is_temporary()) {
                // notify termination event
                hproc.notify_terminated_event();
                // terminate process
                break;
            }

            // wait for next activation
            hproc.wait_terminated(reconf_pctrl.get_activation_event());
            is_dont_initialize_allowed = true;

        } else { // just this process has been reset
            if (pinfo->has_sync_reset()) {
                pinfo->update_process_control_state();
                if (!pinfo->process_control.is_active()) {
                    ::sc_core::wait();
                    pinfo->update_process_control_state();
                }
            } else {
                pinfo->process_control.activate();
            }
            is_dont_initialize_allowed = false;

        }
        // restart process...
        continue;
    }
    // terminate physically...
}

void process_support::dont_initialize(
    const sc_object& host) const
{
    if (this->has_reconfigurable_context()) {
        if (!sc_is_running() && this->_rc_check_valid_last_proc(host)) {
            p_last_pinfo->set_dont_initialize(true);
        }
    }
}

rc_process_handle process_support::rc_spawn(
    boost::function<void (void)> func, const char* name_p,
    rc_spawn_options* opt_p)
{
    // no function specified?
    if (func.empty()) {
        RC_REPORT_ERROR(RC_ID_INVALID_USAGE_,
            "invalid usage (in '" << this->name() << "')");
    }
    sc_spawn_options  opt_tmp;
    sc_spawn_options* spawn_options;
    if (opt_p != NULL) {
        spawn_options = static_cast<sc_spawn_options*>(opt_p);
    } else {
        spawn_options = &opt_tmp;
    }

    rc_process_registry& preg = rc_get_process_registry();

    // create the entry point of the reconfigurable process
    boost::function<void (void)> entry_func;
    if (spawn_options->is_method()) {
        entry_func =
            boost::bind(&process_support::method_process, this);
    } else {
        entry_func =
            boost::bind(&process_support::thread_process, this);
    }

    const rc_process_control& reconf_pctrl =
        p_reconf.rc_get_process_control();
    bool is_thread = !spawn_options->is_method();
    bool dont_initialize = (opt_p != NULL && opt_p->is_dont_initialize());
    std::auto_ptr<process_info> pinfo(
        new process_info(
            func, reconf_pctrl, is_thread, dont_initialize, false));

    if (opt_p != NULL && !opt_p->is_method()) {
        rc_spawn_options::reset_port_vector& rpvec =
            opt_p->get_reset_port_vector();
        for (rc_spawn_options::reset_port_vector::iterator it =
                rpvec.begin(); it != rpvec.end(); ++it)
        { pinfo->register_reset_port(*(it->first), it->second); }

        rc_spawn_options::reset_signal_vector& rsvec =
            opt_p->get_reset_signal_vector();
        for (rc_spawn_options::reset_signal_vector::iterator it =
                rsvec.begin(); it != rsvec.end(); ++it)
        { pinfo->register_reset_signal(*(it->first), it->second); }

        rc_spawn_options::reset_channel_vector& rcvec =
            opt_p->get_reset_channel_vector();
        for (rc_spawn_options::reset_channel_vector::iterator it =
                rcvec.begin(); it != rcvec.end(); ++it)
        { pinfo->register_reset_channel(*(it->first), it->second); }

        rc_spawn_options::process_reset_vector& prvec =
            opt_p->get_process_reset_vector();
        for (rc_spawn_options::process_reset_vector::iterator it =
                prvec.begin(); it != prvec.end(); ++it)
        {
            pinfo->register_reset_channel(
                static_cast<rc_reset_channel_if&>(*(*it)), true);
        }
    }

    const rc_process_control& pctrl = pinfo->process_control;
    spawn_options->set_sensitivity(&pctrl.get_deactivation_event());

    // suppress 'object name already exists' warning
    const sc_actions actions =
        sc_report_handler::set_actions(
            SC_WARNING, SC_DO_NOTHING);

    // spawn process
    const sc_process_handle spawned_process =
        sc_spawn(entry_func, name_p, spawn_options);

    // reset report action
    sc_report_handler::set_actions(SC_WARNING, actions);

    sc_object* const proc_obj = spawned_process.get_process_object();
    p_reconf_pinfo_map[proc_obj] = pinfo.release();

    const bool is_temporary = sc_is_running();
    return preg.register_process(
        spawned_process, dont_initialize, true, pctrl, is_temporary);
}

void process_support::terminate()
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (!hproc.terminate()) {
        sc_object* parent = this->get_parent_object();
        RC_REPORT_ERROR(RC_ID_INVALID_USAGE_,
            "attempt to terminate non-reconfigurable process (in '"
            << (parent ? parent->name() : "(top level)") << "')");
    }
}

bool process_support::reset_signal_is(
    const sc_object& host, const sc_in<bool>& reset_port, bool active_level)
{
    if (!sc_is_running() && this->_rc_check_valid_last_proc(host)) {
        // register port as reset trigger
        p_last_pinfo->register_reset_port(reset_port, active_level);
        // update child count, it could have changed
        p_last_host_child_count = host.get_child_objects().size();
        return true;
    } else {
        return false;
    }
}


bool process_support::reset_signal_is(
    const sc_object& host, const sc_signal<bool>& reset_signal,
    bool active_level)
{
    if (!sc_is_running() && this->_rc_check_valid_last_proc(host)) {
        // register signal as reset trigger
        p_last_pinfo->register_reset_signal(reset_signal, active_level);
        // update child count, it could have changed
        p_last_host_child_count = host.get_child_objects().size();
        return true;
    } else {
        return false;
    }
}

const sc_signal<bool>* process_support::reset_channel_is(
    const sc_object& host, rc_reset_channel_if& reset_channel,
    bool active_level)
{
    const sc_signal<bool>* reset_signal = NULL;
    if (!sc_is_running() && this->_rc_check_valid_last_proc(host)) {
        // register reset_channel as reset trigger
        p_last_pinfo->register_reset_channel(
            reset_channel, active_level);
    } else {
        reset_signal = reset_channel.get_underlying_reset_signal();
        if (reset_signal == NULL) {
            RC_REPORT_ERROR(RC_ID_INVALID_USAGE_,
                "channel cannot be used as a reset signal for"
                " standard SC_CTHREAD processes"
                " (in '" << host.name() << "')");
        }
    }
    return reset_signal;
}

void process_support::reset_is(
    const sc_object& host, rc_process_reset& process_reset)
{
    if (!sc_is_running() && this->_rc_check_valid_last_proc(host)) {
        // register process_reset as reset trigger
        p_last_pinfo->register_reset_channel(
            static_cast<rc_process_reset&>(process_reset), true);
    } else {
        RC_REPORT_ERROR(RC_ID_INVALID_USAGE_,
            "unrelated reset_is() (in '" << host.name() << "')");
    }
}

void process_support::set_sync_reset(const sc_object& host, bool b) const
{
    if (!sc_is_running() && this->_rc_check_valid_last_proc(host)) {
        if (p_last_pinfo->is_thread()) {
            p_last_pinfo->set_sync_reset(b);
        } else {
            RC_REPORT_WARNING(RC_ID_INVALID_USAGE_,
                "rc_set_" << (b ? "sync" : "async") << "_reset()"
                " has no effect on method processes (in '"
                << host.name() << "')");
        }

    } else {
        RC_REPORT_WARNING(RC_ID_INVALID_USAGE_,
            "unrelated rc_set_" << (b ? "sync" : "async") << "_reset()"
            " ignored (in '" << host.name() << "')");
    }
}

rc_process_behavior_change process_support::disable_process_control() const
{
    rc_process_handle hproc = rc_get_current_process_handle();
    if (this->has_reconfigurable_context() && hproc.is_cancelable()) {
        // process is reconfigurable
        return hproc.behavior_change(p_reconf.rc_get_process_control());
    } else {
        // process is considered as non-reconfigurable
        return hproc.behavior_change(false);
    }
}

void process_support::before_end_of_elaboration()
{
    for (process_info_map::iterator it = p_reconf_pinfo_map.begin();
        it != p_reconf_pinfo_map.end();
        ++it)
    {
        it->second->register_reset_port_interfaces();
    }
}

process_support::process_info* process_support::_rc_find_process_info(
    sc_object* proc_obj) const
{
    process_info_map::const_iterator it =
        p_reconf_pinfo_map.find(proc_obj);
    return (it != p_reconf_pinfo_map.end() ? it->second : NULL);
}

process_support::process_info* process_support::_rc_fetch_process_info(
    sc_object* proc_obj)
{
    process_info_map::iterator it =
        p_reconf_pinfo_map.find(proc_obj);
    if (it != p_reconf_pinfo_map.end()) {
        process_info* const pinfo = it->second;
        p_reconf_pinfo_map.erase(it);
        return pinfo;
    } else {
        return NULL;
    }
}

bool process_support::_rc_check_valid_last_proc(
    const sc_object& host) const
{
    return (p_last_pinfo != NULL
        && p_last_host == &host
        && host.get_child_objects().size() == p_last_host_child_count);
}

rc_reconfigurable& process_support::_rc_find_context(
    sc_object* start_search)
{
    rc_reconfigurable* const context =
        rc_get_reconfigurable_context(start_search);

    if (context != NULL) {
        return *context;
    } else {
        return rc_non_reconfigurable::get_instance();
    }
}

} // namespace internals

rc_process_reset::rc_process_reset()
    : sc_prim_channel(sc_gen_unique_name("_rc_process_reset"))
    { }

rc_process_reset::rc_process_reset(const char* name_)
    : sc_prim_channel(name_)
    { }


void rc_process_reset::trigger()
{
    rc_process_handle hproc = rc_get_current_process_handle();
    bool was_canceled = hproc.is_canceled();

    for (pctrl_set::iterator it = p_pctrl_set.begin();
        it != p_pctrl_set.end();
        ++it)
    {
        (*it)->deactivate();
    }

    if (hproc.is_canceled()) {
        if (!was_canceled) {
            sc_core::wait(SC_ZERO_TIME);
        }
        throw new rc_process_cancel_exception();
    }
}

void rc_process_reset::register_process_control(
    rc_process_control& pctrl, bool active_level) const
{
    assert(active_level == true);

    if (active_level == true) {
        p_pctrl_set.insert(&pctrl);
    }
}

void rc_process_reset::unregister_process_control(
    rc_process_control& pctrl) const
{
    p_pctrl_set.erase(&pctrl);
}

} // namespace ReChannel

//
// $Id: rc_process_support.cpp,v 1.15 2007/12/20 20:37:29 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_process_support.cpp,v $
//
