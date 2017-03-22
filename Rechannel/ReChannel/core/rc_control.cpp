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
 * @file    rc_control.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:29:16 $
 * $Revision: 1.19 $
 */

#include "rc_control.h"

namespace ReChannel {

bool rc_control::has_control(
    const rc_reconfigurable_set& reconf_set) const
{
    rc_reconfigurable_set::const_iterator it;
    for(it = reconf_set.begin(); it != reconf_set.end(); ++it) {
        rc_reconfigurable& reconf = *(*it);
        if (p_reconfigurable_set.find(reconf)
        == p_reconfigurable_set.end()) {
            return false;
        }
    }
    return true;
}

void rc_control::add(const rc_reconfigurable_set& reconf_set)
{
    rc_reconfigurable_set::const_iterator it;
    for(it = reconf_set.begin(); it != reconf_set.end(); ++it) {
        rc_reconfigurable& reconf = *(*it);
        rc_control* owner = reconf.get_control();
        if (owner != NULL && owner != this) {
            RC_REPORT_ERROR(RC_ID_CONTROL_CONFLICT_,
                "dynamic object '" << reconf.rc_get_name()
                << "' still controlled by control '"
                << owner->name() << "' (in control '"
                << this->name() << "'");
        }
        // let reconf be controlled by this object
        reconf.set_control(this);
        // calculate and set the delays for the reconfiguration actions
        for (int i=0; i < rc_reconfigurable::ACTION_COUNT; i++) {
            typedef rc_reconfigurable::action_type action_type;
            reconf.rc_set_delay(
                (action_type)i, takes_time(reconf, (action_type)i));
        }
        p_reconfigurable_set.insert(reconf);
        sc_object* obj = reconf.rc_get_object();
        if (obj != NULL) {
            p_obj_reconf_map.insert(
                obj_reconf_map::value_type(obj, &reconf));
        }
    }
}

void rc_control::remove(const rc_reconfigurable_set& reconf_set)
{
    lock(reconf_set);
    try {
        unload(reconf_set);
        rc_reconfigurable_set::const_iterator it;
        for(it = reconf_set.begin(); it != reconf_set.end(); ++it) {
            rc_reconfigurable& reconf = *(*it);
            reconf.set_control(NULL);
            p_reconfigurable_set.erase(reconf);
            sc_object* obj = reconf.rc_get_object();
            if (obj != NULL) {
                p_obj_reconf_map.erase(obj);
            }
        }
    } catch(...) {
        unlock(reconf_set);
        throw;
    }
    // release ALL locks hold by this process
    rc_reconfigurable_set::const_iterator it;
    for(it = reconf_set.begin(); it != reconf_set.end(); ++it) {
        rc_reconfigurable& reconf = *(*it);
        // calls unlock() until no lock is hold anymore
        while(reconf.unlock());
    }
    // the modules are "free" now
}

void rc_control::load(const rc_reconfigurable_set& reconf_set)
{
    _rc_do_action(
        reconf_set, rc_reconfigurable::INACTIVE,
        rc_reconfigurable::UNLOADED);
}

void rc_control::unload(const rc_reconfigurable_set& reconf_set)
{
    _rc_do_action(reconf_set, rc_reconfigurable::UNLOADED);
}

void rc_control::activate(const rc_reconfigurable_set& reconf_set)
{
    _rc_do_action(reconf_set, rc_reconfigurable::ACTIVE);
}

void rc_control::deactivate(const rc_reconfigurable_set& reconf_set)
{
    _rc_do_action(
        reconf_set, rc_reconfigurable::INACTIVE,
        rc_reconfigurable::ACTIVE);
}

void rc_control::move(
    const rc_reconfigurable_set& reconf_set,
    rc_switch_connector_base& target)
{
    lock(reconf_set);
    try {
        unload(reconf_set);
        rc_reconfigurable_set::const_iterator it;
        for(it = reconf_set.begin(); it != reconf_set.end(); ++it) {
            rc_reconfigurable& reconf = *(*it);
            reconf.move(target);
        }
    } catch(...) {
        unlock(reconf_set);
        throw;
    }
    unlock(reconf_set);
}

void rc_control::lock(const rc_reconfigurable_set& reconf_set)
{
    rc_reconfigurable_set::const_iterator it;
    for(it = reconf_set.begin(); it != reconf_set.end(); ++it) {
        rc_reconfigurable& reconf = *(*it);
        if (reconf.get_control() != this) {
            // unlock all previously locked reconfigurables
            _rc_unlock(reconf_set.begin(), it);
            RC_REPORT_ERROR(RC_ID_CONTROL_CONFLICT_,
                "no control over dynamic object '"
                << reconf.rc_get_name() << "' (in control '"
                << this->name() << "'");
        }
        reconf.lock();
    }
}

bool rc_control::trylock(const rc_reconfigurable_set& reconf_set)
{
    rc_reconfigurable_set::const_iterator it;
    for(it = reconf_set.begin(); it != reconf_set.end(); ++it) {
        rc_reconfigurable& reconf = *(*it);
        if (reconf.get_control() != this) {
            // unlock all previously locked reconfigurables
            _rc_unlock(reconf_set.begin(), it);
            RC_REPORT_ERROR(RC_ID_CONTROL_CONFLICT_,
                "no control over dynamic object '"
                << reconf.rc_get_name() << "' (in control '"
                << this->name() << "'");
        }
        if (!reconf.trylock()) {
            // unlock all previously locked reconfigurables
            _rc_unlock(reconf_set.begin(), it);
            return false;
        }
    }
    return true;
}

void rc_control::unlock(const rc_reconfigurable_set& reconf_set)
{
    _rc_unlock(reconf_set.begin(), reconf_set.end());
}

bool rc_control::is_locked(const rc_reconfigurable_set& reconf_set) const
{
    rc_reconfigurable_set::const_iterator it;
    for(it = reconf_set.begin(); it != reconf_set.end(); ++it) {
        rc_reconfigurable& reconf = *(*it);
        if (!reconf.is_locked()) {
            return false;
        }
    }
    return true;
}

sc_time rc_control::takes_time(
    const rc_reconfigurable& reconf,
    rc_reconfigurable::action_type action) const
{
    return reconf.rc_get_default_delay(action);
}

void rc_control::update_delays(rc_reconfigurable::action_type action)
{
    // iterate through all controlled rc_reconfigurables
    rc_reconfigurable_set::const_iterator it;
    for(it = p_reconfigurable_set.begin();
        it != p_reconfigurable_set.end();
        ++it)
    {
        rc_reconfigurable& reconf = *(*it);
        // update the delay of the given reconfiguration action
        reconf.rc_set_delay(action, takes_time(reconf, action));
    }
}

void rc_control::get_controlled_set(
    rc_reconfigurable_set& copy_dest) const
{
    copy_dest = p_reconfigurable_set; // copy the internal set
}

void rc_control::start_of_simulation()
{
    static bool s_start_of_simulation_called = false;

    // call start_of_simulation() ONCE on ALL existing rc_reconfigurables
    if (!s_start_of_simulation_called) {
        std::vector<rc_reconfigurable*> all_reconfigurables;
        rc_reconfigurable::get_all_reconfigurables(all_reconfigurables);
        std::vector<rc_reconfigurable*>::const_iterator it;
        for(it = all_reconfigurables.begin();
           it != all_reconfigurables.end();
           ++it)
        {
            rc_reconfigurable& reconf = *(*it);
            reconf.start_of_simulation();
        }
        s_start_of_simulation_called = true;
    }
}

void rc_control::_rc_unlock(
    rc_reconfigurable_set::const_iterator start_,
    rc_reconfigurable_set::const_iterator end_) const
{
    rc_reconfigurable_set::const_iterator it;
    for(it = start_; it != end_; ++it) {
        rc_reconfigurable& reconf = *(*it);
        reconf.unlock();
    }
}

void rc_control::_rc_do_action(
    const rc_reconfigurable_set& reconf_set,
    rc_reconfigurable::state_type new_state)
{
    if (reconf_set.size() == 0) {
        return;
    }
    lock(reconf_set);
    try {
        _rc_reconfigure(reconf_set, new_state);
    } catch(...) {
        unlock(reconf_set);
        throw;
    }
    unlock(reconf_set);
}

void rc_control::_rc_do_action(
    const rc_reconfigurable_set& reconf_set,
    rc_reconfigurable::state_type new_state,
    rc_reconfigurable::state_type only_change_state)
{
    if (reconf_set.size() == 0) {
        return;
    }
    lock(reconf_set);
    try {
        rc_reconfigurable_set dyn_objects_to_change;
        rc_reconfigurable_set::const_iterator it;
        for(it = reconf_set.begin(); it != reconf_set.end(); ++it) {
            rc_reconfigurable& reconf = *(*it);
            if (reconf.rc_get_state() == only_change_state) {
                dyn_objects_to_change.insert(reconf);
            }
        }
        _rc_reconfigure(dyn_objects_to_change, new_state);
    } catch(...) {
        unlock(reconf_set);
        throw;
    }
    unlock(reconf_set);
}

void rc_control::_rc_reconfigure(
    const rc_reconfigurable_set& reconf_set,
    rc_reconfigurable::state_type new_state)
{
    if (sc_is_running()) {
        if (reconf_set.size() == 0) {
            return;
        } else if (reconf_set.size() == 1) {
            rc_reconfigurable& reconf = *(*(reconf_set.begin()));
            if (reconf.rc_get_state() != new_state) {
                reconf.reconfigure(new_state);
            }
        } else {
            typedef std::vector<sc_process_handle> proc_vector_type;
            proc_vector_type procs;
            rc_reconfigurable_set::const_iterator it;
            for(it = reconf_set.begin(); it != reconf_set.end(); ++it) {
                rc_reconfigurable& reconf = *(*it);
                if (reconf.rc_get_state() != new_state) {
                    sc_process_handle proc =
                        sc_spawn(
                            sc_bind(
                                &rc_reconfigurable::reconfigure,
                                &reconf, new_state));
                    reconf.share_lock(proc);
                    procs.push_back(proc);
                }
            }
            if (procs.size() > 0) {
                if (procs.size() == 1) {
                    ::sc_core::wait((*procs.begin()).terminated_event());
                } else { // procs.size() >= 2
                    sc_event_and_list& event_and_list =
                        (*procs.begin()).terminated_event()
                        & (*(++procs.begin())).terminated_event();
                    proc_vector_type::iterator it;
                    for(it = procs.begin() + 3; it != procs.end(); ++it) {
                        event_and_list & (*it).terminated_event();
                    }
                    ::sc_core::wait(event_and_list);
                }
            }
        }
    } else {
        rc_reconfigurable_set::const_iterator it;
        for(it = reconf_set.begin(); it != reconf_set.end(); ++it) {
            rc_reconfigurable& reconf = *(*it);
            // (note: reconfigure() may also be invoked during elaboration)
            reconf.reconfigure(new_state);
        }
    }
}

} // namespace ReChannel

//
// $Id: rc_control.cpp,v 1.19 2007/12/20 20:29:16 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_control.cpp,v $
//
