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
 * @file    rc_reconfigurable.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2008/01/01 13:34:00 $
 * $Revision: 1.22 $
 */

#include "rc_reconfigurable.h"
#include "rc_control.h"

namespace ReChannel {

namespace internals {
namespace reconfigurable {

rc_reconfigurable* begin_construction::s_current = NULL;

begin_construction::begin_construction(rc_reconfigurable& reconf)
    : p_previous(s_current), p_finished(false)
{
    s_current = &reconf;
}

void begin_construction::finish()
{
    if (!p_finished) {
        s_current = p_previous;
        p_finished = true;
    }
}

void begin_construction::finish(sc_object& reconf)
{
    if (!p_finished) {
        if (s_current != NULL) {
            s_current->_rc_set_sc_object(reconf);
        }
        s_current = p_previous;
        p_finished = true;
    }
}

} // namespace reconfigurable
} // namespace internals

std::vector<rc_reconfigurable*> rc_reconfigurable::s_reconfigurables;

rc_reconfigurable::rc_reconfigurable(sc_object* this_)
    : p_sc_object(this_), p_is_no_sc_object(false),
      p_state(UNLOADED), p_next_state(UNLOADED),
      p_transaction_count(0),
      p_switch_commobj_index(p_switch_commobj_map.get<0>()),
      p_commobj_switch_index(p_switch_commobj_map.get<1>()),
      p_control(NULL), p_curr_switch_conn(NULL)
{
    p_self_set.insert(*this);

    p_delta_sync.set_callback(
        boost::bind(
            &rc_reconfigurable::_rc_delta_sync_state_change, this, _1));

    // add this instance to the global registry vector
    s_reconfigurables.push_back(this);
}

rc_reconfigurable::~rc_reconfigurable()
{ }

std::string rc_reconfigurable::rc_get_name() const
{
    const sc_object* const obj = this->rc_get_object();
    if (obj != NULL) {
        return obj->name();
    } else {
        return "(unknown)";
    }
}

void rc_reconfigurable::bind(rc_switch_connector_base& switch_connector)
{
    for (portmap_vector::iterator it = p_portmap_vector.begin();
        it != p_portmap_vector.end();
        ++it)
    {
        rc_portmap_base& portmap = *(*it);
        if (switch_connector.is_compatible(portmap)) {
            switch_connector.bind_dynamic(portmap);
            p_curr_switch_conn = &switch_connector;
            return;
        }
    }
    RC_REPORT_ERROR(RC_ID_SWITCH_CONNECTOR_BINDING_ERROR_,
        "incompatible switch connector '" << switch_connector.name() << "'"
        " (in reconfigurable '" << this->rc_get_name() << "')");
}

void rc_reconfigurable::rc_register_switch(
    rc_switch& switch_obj, const rc_object_handle& bound_obj)
{
    if (rc_is_registered(switch_obj)) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "switch '" << switch_obj.get_switch_name()
            << "' already registered at this reconfigurable object"
               " (in reconfigurable '" << this->rc_get_name() << "')");
    }
    sc_interface* dyn_if = bound_obj.get_interface();
    sc_interface* dyn_if_switch =
        switch_obj.get_registered_interface(*this);
    if (dyn_if == NULL) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "registration of switch '" << switch_obj.get_switch_name()
            << "' failed: no interface given"
            << " (in reconfigurable '" << this->rc_get_name() << "')");
    } else if (dyn_if_switch == NULL) {
        assert(dyn_if != NULL);
        switch_obj.register_reconfigurable(*this, *dyn_if);
    } else if (dyn_if_switch != dyn_if) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "registration of switch '" << switch_obj.get_switch_name()
            << "' failed: interface mismatch"
            << " (in reconfigurable '" << this->rc_get_name() << "')");
    }
    p_switch_commobj_index.insert(
        switch_commobj_pair(&switch_obj, bound_obj));
}

bool rc_reconfigurable::rc_is_registered(rc_switch& switch_obj) const
{
    return (p_switch_commobj_index.find(&switch_obj)
        != p_switch_commobj_index.end());
}

void rc_reconfigurable::rc_register_resettable(rc_resettable& resettable)
{
    p_resettable_set.insert(&resettable);
}

bool rc_reconfigurable::rc_is_registered(rc_resettable& resettable) const
{
    return (p_resettable_set.find(&resettable) != p_resettable_set.end());
}

void rc_reconfigurable::rc_possible_deactivation()
{
    // check whether deactivation of this reconfigurable is requested
    //    and the transaction count equals zero
    if ((p_transaction_count == 0 && rc_is_deactivation_requested())
        || rc_get_state() != ACTIVE) {
        const rc_process_handle hproc = rc_get_current_process_handle();
        // THREADs can be reset or suspended until reactivation
        if (hproc.proc_kind() == SC_THREAD_PROC_) {
            // wait for deactivation
            rc_wait(p_pctrl.get_deactivation_event());
            // (reconfigurable processes won't reach this line)
            if (!p_pctrl.is_active()) { // in case of non-resettable procs
                // wait for reactivation
                rc_wait(p_pctrl.get_activation_event());
            }
        }
    }
}

void rc_reconfigurable::rc_possible_deactivation_delta()
{
    // check whether deactivation of this reconfigurable is requested
    if (rc_is_deactivation_requested() || rc_get_state() != ACTIVE) {
        const rc_process_handle hproc = rc_get_current_process_handle();
        // THREADs can be reset or suspended until reactivation
        if (hproc.proc_kind() == SC_THREAD_PROC_) {
            // wait one delta cycle (to give the deactivation a chance)
            rc_wait(SC_ZERO_TIME);
        }
    }
}

void rc_reconfigurable::rc_set_delay(action_type a, sc_time t)
{
    p_action_delays[a] = t;
    if (p_control == NULL) {
        p_action_default_delays[a] = t;
    }
}

void rc_reconfigurable::rc_add_filter(
    const rc_object_handle& commobj_, rc_interface_filter& filter_)
{
    // check if filter is added for the first time
    if (this->rc_has_filter(filter_)) {
        RC_REPORT_ERROR(RC_ID_DUPLICATE_ENTRY_,
            "filter already added"
            " (in reconfigurable '" << this->rc_get_name() << "')");
    }

    // insert filter
    p_filter_set.insert(&filter_);
    p_commobj_filters_map[commobj_].push_back(&filter_);
}

int rc_reconfigurable::rc_get_filter_count(
    const rc_object_handle& commobj_) const
{
    commobj_filters_map::const_iterator it =
        p_commobj_filters_map.find(commobj_);
    if (it != p_commobj_filters_map.end()) {
        return it->second.size();
    } else {
        return 0;
    }
}

rc_reconfigurable::filter_chain
rc_reconfigurable::rc_get_filter_chain(
    const rc_object_handle& commobj_) const
{
    commobj_filters_map::const_iterator it =
        p_commobj_filters_map.find(commobj_);
    return (it != p_commobj_filters_map.end()
        ? it->second : filter_chain());
}

bool rc_reconfigurable::rc_has_filter(rc_interface_filter& filter_) const
{
    return (p_filter_set.find(&filter_) != p_filter_set.end());
}

void rc_reconfigurable::rc_refresh_notify(
    const rc_object_handle& commobj_)
{
    commobj_switch_range range =
        p_commobj_switch_index.equal_range(commobj_);
    for (commobj_switch_index::iterator it = range.first;
        it != range.second;
        ++it)
    {
        it->first->refresh_notify();
    }
}

void rc_reconfigurable::rc_refresh_notify_all()
{
    for (switch_commobj_index::iterator it =
            p_switch_commobj_index.begin();
        it != p_switch_commobj_index.end();
        ++it)
    {
        it->first->refresh_notify();
    }
}

void rc_reconfigurable::start_of_simulation()
{
    // initialise all registered resettables
    for (resettable_set::const_iterator it = p_resettable_set.begin();
        it != p_resettable_set.end();
        ++it)
    {
        (*it)->rc_on_init_resettable();
    }

    // initialise switch states
    switch(p_state) {
    case INACTIVE:
        change_switch_state(rc_switch::CLOSED);
        break;
    case ACTIVE:
        change_switch_state(rc_switch::OPEN);
        break;
    default:
        break;
    }

    // if active, activate the resettable processes
    if (rc_is_active()) {
        p_pctrl.activate();
    }
}

void rc_reconfigurable::reconfigure(state_type new_state)
{
    const bool has_lock_share_ = has_lock_share();
    if (!has_lock_share_) {
        p_mutex.lock();
    }
    try {
        switch(p_state) {
        case UNLOADED:
            switch(new_state) {
            case INACTIVE:
                _rc_load();
                break;
            case ACTIVE:
                _rc_load();
                _rc_activate();
                break;
            default:
                break;
            }
            break;
        case INACTIVE:
            switch(new_state) {
            case UNLOADED:
                _rc_unload();
                break;
            case ACTIVE:
                _rc_activate();
                break;
            default:
                break;
            }
            break;
        case ACTIVE:
            switch(new_state) {
            case UNLOADED:
                _rc_deactivate();
                _rc_unload();
                break;
            case INACTIVE:
                _rc_deactivate();
                break;
            default:
                break;
            }
            break;
        }
    } catch(...) {
        if (!has_lock_share_) {
            p_mutex.unlock();
        }
        throw;
    }
    if (!has_lock_share_) {
        p_mutex.unlock();
    }
}

void rc_reconfigurable::change_switch_state(
    rc_switch::state_type new_state)
{
    for (switch_commobj_index::iterator it =
            p_switch_commobj_index.begin();
        it != p_switch_commobj_index.end();
        ++it)
    {
        rc_switch& switch_ = *(it->first);

        switch(new_state) {
        case rc_switch::OPEN:
        {
            // search for registered filters
            commobj_filters_map::const_iterator it2 =
                p_commobj_filters_map.find(it->second);

            if (it2 != p_commobj_filters_map.end()) {
                // open switch (and apply filters)
                switch_.open(*this, it2->second);
            } else {
                // open switch (without filters)
                switch_.open(*this);
            }
            break;
        }
        case rc_switch::CLOSED:
            // close switch
            switch_.close();
            break;
        case rc_switch::UNDEF:
            // set switch to undefined state
            switch_.set_undefined();
            break;
        }
    }
}

void rc_reconfigurable::move(switch_conn_type& target)
{
    assert(!(rc_is_loaded() || rc_is_state_changing()));

    rc_portmap_base* const portmap = _rc_get_compatible_portmap(target);
    if (portmap == NULL) {
        RC_REPORT_ERROR(RC_ID_SWITCH_CONNECTOR_INCOMPATIBLE_ERROR_,
            "switch connector '" << target.name() << "' is incompatible"
            << " (in reconfigurable '" << this->rc_get_name() << "')");
    }
    if (p_curr_switch_conn != &target) {
        p_curr_switch_conn = NULL;
        const int count = portmap->size();
        for (int i=0; i < count; i++) {
            // get the commobj at index i
            rc_object_handle commobj = portmap->get(i);
            // unregister commobj at its old switch
            commobj_switch_index::iterator it =
                p_commobj_switch_index.find(commobj);
            if (it != p_commobj_switch_index.end()) {
                rc_switch& switch_ = *it->first;
                switch_.unregister_reconfigurable(*this);
                p_commobj_switch_index.erase(it);
            }
            // register commobj with the new switch
            rc_switch& switch_ = target[i];
            sc_interface* const if_ = commobj.get_interface();
            assert(if_ != NULL);
            switch_.register_reconfigurable(*this, *if_);
            p_commobj_switch_index.insert(
                commobj_switch_index::value_type(&switch_, commobj));
        }
        p_curr_switch_conn = &target;
    }
}

bool rc_reconfigurable::unlock()
{
    const bool has_unlocked = p_mutex.unlock();
    if (has_unlocked && p_lock_share.valid()) {
        p_lock_share = sc_process_handle();
    }
    return has_unlocked;
}

bool rc_reconfigurable::share_lock(sc_process_handle proc)
{
    if (p_mutex.has_lock()) {
        p_lock_share = proc;
        return true;
    } else {
        return false;
    }
}

bool rc_reconfigurable::reset_lock_share()
{
    if (p_mutex.has_lock()) {
        p_lock_share = sc_process_handle();
        return true;
    } else {
        return false;
    }
}

bool rc_reconfigurable::lock_switches(bool report_error)
{
    int unlocked_count = 0;
    // check whether all of the switches can be locked
    for (switch_commobj_index::iterator it =
            p_switch_commobj_index.begin();
        it != p_switch_commobj_index.end();
        ++it)
    {
        rc_switch& switch_ = *(it->first);

        if (!switch_.is_lock_owner(*this)) {
            if (!switch_.is_locked()) {
                ++unlocked_count;
            } else {
                // failure, lock hold by another module
                if (report_error) {
                    RC_REPORT_ERROR(RC_ID_SWITCH_CONFLICT_,
                        "switch '" << switch_.get_switch_name()
                        << "' is already in use by another reconfigurable object"
                        << " (in reconfigurable '" << this->rc_get_name()
                        << "')");
                } else {
                    return false;
                }
            }
        }
    }
    // lock the switches (if necessary)
    if (unlocked_count > 0) {
        bool succ = false;
        for (switch_commobj_index::iterator it =
                p_switch_commobj_index.begin();
            it != p_switch_commobj_index.end();
            ++it)
        {
            rc_switch& switch_ = *(it->first);

            // the previous check loop should guarantee a success
            succ = switch_.set_locked(*this, true);
            assert(succ == true);
        }
    }
    return true;
}

void rc_reconfigurable::unlock_switches()
{
    // unlock the switches locked by this reconfigurable object
    for (switch_commobj_index::iterator it =
            p_switch_commobj_index.begin();
        it != p_switch_commobj_index.end();
        ++it)
    {
        rc_switch& switch_ = *(it->first);
        switch_.set_locked(*this, false);
    }
}

void rc_reconfigurable::reset_transaction_count()
{
    p_transaction_count = 0;
    if (rc_is_deactivation_requested()) {
        p_delta_sync.request_update();
    }
}

void rc_reconfigurable::rc_add_portmap(rc_portmap_base& portmap)
{
    sc_object* const this_obj = this->rc_get_object();
    if (dynamic_cast<sc_module*>(this_obj) == NULL) {
        RC_REPORT_ERROR(RC_ID_INVALID_USAGE_,
            "this reconfigurable is incapable of attaching a port map"
            " (in reconfigurable '" << this->rc_get_name() << "')");
    }
    const unsigned int count = portmap.size();
    for(unsigned int i=0; i < count; ++i) {
        sc_object& obj = portmap.get_object(i);
        if (obj.get_parent_object() != this_obj) {
            RC_REPORT_ERROR(RC_ID_PORTMAP_INCOMPATIBLE_ERROR_,
                "port map contains an object from a different parent"
                " (in reconfigurable '" << this->rc_get_name()
                << "', at index: " << i <<")");
        }
    }
    const int found_index = this->_rc_is_compatible(portmap);
    if (found_index >= 0) {
        if (p_portmap_vector[found_index] != &portmap) {
            RC_REPORT_ERROR(RC_ID_DUPLICATE_ENTRY_,
            "port map with identical type already registered"
            " (in reconfigurable '" << this->rc_get_name() << "')");
        }
        return;
    }

    p_portmap_vector.push_back(&portmap);
}

void rc_reconfigurable::rc_clear_portmaps()
{
    p_portmap_vector.clear();
}

rc_portmap_base& rc_reconfigurable::get_portmap(int index)
{
    assert(index < 0 || index >= (int)p_portmap_vector.size());
    return *(p_portmap_vector[index]);
}

int rc_reconfigurable::_rc_is_compatible(
    const rc_portmap_base& portmap) const
{
    const int count = (int)p_portmap_vector.size();
    for (int i=0; i < count; ++i) {
        if (portmap.is_compatible(*p_portmap_vector[i])) {
            return i;
        }
    }
    return -1;
}

rc_portmap_base* rc_reconfigurable::_rc_get_compatible_portmap(
        const rc_switch_connector_base& switch_conn) const
{
    const int count = (int)p_portmap_vector.size();
    for (int i=0; i < count; ++i) {
        rc_portmap_base* const portmap = p_portmap_vector[i];
        if (switch_conn.is_compatible(*portmap)) {
            return portmap;
        }
    }
    return NULL;
}

void rc_reconfigurable::_rc_set_sc_object(sc_object& reconf)
{
    if (p_sc_object == NULL) {
        p_sc_object = &reconf;
        p_is_no_sc_object = false;
    }
}

const sc_object* rc_reconfigurable::rc_get_object() const
{
    sc_object* obj = p_sc_object;
    if (obj == NULL) {
        if (!p_is_no_sc_object) {
            obj = dynamic_cast<sc_object*>(
                    const_cast<rc_reconfigurable*>(this));
            if (obj != NULL) {
                p_sc_object = obj;
            } else {
                p_is_no_sc_object = true;
            }
        }
    }
    return obj;
}

sc_object* rc_reconfigurable::rc_get_object()
{
    sc_object* obj = p_sc_object;
    if (obj == NULL) {
        if (!p_is_no_sc_object) {
            obj = dynamic_cast<sc_object*>(this);
            if (obj != NULL) {
                p_sc_object = obj;
            } else {
                p_is_no_sc_object = true;
            }
        }
    }
    return obj;
}

void rc_reconfigurable::_rc_delta_sync_state_change(
    rc_delta_sync_object& delta_sync)
{
    assert(&delta_sync == &p_delta_sync);

    if (p_state == INACTIVE && p_next_state == ACTIVE) {
        // activate reconfigurable object
        p_state = ACTIVE;
        p_delta_sync.set_enabled(false);
        p_pctrl.activate();
        change_switch_state(rc_switch::OPEN);
    } else if (p_state == ACTIVE && p_next_state == INACTIVE) {
        // check whether reconfigurable object is ready to be deactivated
        if (p_transaction_count == 0) {
            // deactivate reconfigurable object
            p_state = INACTIVE;
            p_delta_sync.set_enabled(false);
            p_pctrl.deactivate();
            change_switch_state(rc_switch::CLOSED);
        }
    } else {
        RC_REPORT_ERROR(RC_ID_INTERNAL_ERROR_, "internal error");
    }
}

void rc_reconfigurable::_rc_unload()
{
    p_next_state = UNLOADED;
    if (sc_is_running()) {
        sc_time delay = rc_get_delay(UNLOAD);
        if (delay != SC_ZERO_TIME) {
            ::sc_core::wait(delay);
        }
        p_state = UNLOADED;
        unlock_switches();
        rc_on_unload();
    } else {
        p_state = UNLOADED;
        unlock_switches();
    }
}

void rc_reconfigurable::_rc_load()
{
    p_next_state = INACTIVE;
    lock_switches(true);
    if (sc_is_running()) {
        sc_time delay = rc_get_delay(LOAD);
        if (delay != SC_ZERO_TIME) {
            ::sc_core::wait(delay);
        }
        p_state = INACTIVE;
        rc_on_load();
    } else {
        p_state = INACTIVE;
    }
}

void rc_reconfigurable::_rc_activate()
{
    p_next_state = ACTIVE;
    if (sc_is_running()) {
        // reset all registered resettables
        _rc_reset();
        // wait activation time (wait at least for one delta cycle)
        ::sc_core::wait(rc_get_delay(ACTIVATE));

        p_delta_sync.set_enabled(true);
        p_delta_sync.request_update();
        ::sc_core::wait(p_pctrl.get_activation_event());
        assert(p_state == ACTIVE);
        rc_on_activate();
    } else {
        p_state = ACTIVE;
    }
}

void rc_reconfigurable::_rc_deactivate()
{
    p_next_state = INACTIVE;
    if (sc_is_running()) {
        p_delta_sync.set_enabled(true);
        p_delta_sync.request_update();
        ::sc_core::wait(p_pctrl.get_deactivation_event());
        assert(p_state == INACTIVE);
        sc_time delay = rc_get_delay(DEACTIVATE);
        if (delay != SC_ZERO_TIME) {
            ::sc_core::wait(delay);
        }
        rc_on_deactivate();
        _rc_reset(); // reset all registered resettables
    } else {
        p_state = INACTIVE;
    }
}

void rc_reconfigurable::_rc_reset()
{
    const sc_dt::uint64 curr_delta = sc_delta_count();
    const sc_time start_time = sc_time_stamp();
    bool has_consumed_deltas = false;

    // reset all registered resettables
    for (resettable_set::const_iterator it = p_resettable_set.begin();
        it != p_resettable_set.end();
        ++it)
    {
        // reset the resettable
        (*it)->rc_on_reset();

        // check whether any delta cycles were consumed
        if (has_consumed_deltas == false
        && sc_delta_count() != curr_delta)
        {
            has_consumed_deltas = true;
        }
    }
    // report a warning if at least one of the resettables consumed time
    if (has_consumed_deltas) {
        const sc_time time_consumed = sc_time_stamp() - start_time;
        if (time_consumed > SC_ZERO_TIME) {
            RC_REPORT_WARNING(
                RC_ID_TIME_CONSUMED_WARNING_,
                "resetting the resettable objects"
                << " consumed time (in reconfigurable '"
                << this->rc_get_name() << "')");
        } else {
            RC_REPORT_WARNING(
                RC_ID_DELTAS_CONSUMED_WARNING_,
                "resetting the resettable objects"
                << " consumed one or more delta cycles"
                << " (in reconfigurable '"
                << this->rc_get_name() << "')");
        }
    }
}

rc_transaction::~rc_transaction()
{
    if (!p_has_ended && reconf != NULL) {
        reconf->rc_end_transaction();
    }
}

rc_reconfigurable* rc_get_reconfigurable_context()
{
    using namespace internals::reconfigurable;

    if (begin_construction::s_current != NULL) {
        return begin_construction::s_current;
    }

    sc_object* start_search = NULL;
    if (!sc_is_running()) {
        std::auto_ptr<temporary_object> tmp_obj(
            new temporary_object());
        start_search = tmp_obj->get_parent_object();
    } else {
        start_search =
            sc_get_current_process_handle().get_parent_object();
    }
    return rc_get_reconfigurable_context(start_search);
}

rc_reconfigurable* rc_get_reconfigurable_context(sc_object* start_search)
{
    using namespace internals::reconfigurable;

    static sc_object*         s_cached_search = NULL;
    static rc_reconfigurable* s_cached_result = NULL;

    if (begin_construction::s_current != NULL) {
        return begin_construction::s_current;
    }

    if (start_search != NULL) {
        if (start_search == s_cached_search) {
            return s_cached_result;
        }
        rc_reconfigurable* reconf =
            dynamic_cast<rc_reconfigurable*>(start_search);
        if (reconf == NULL) {
            if (start_search->get_parent_object() == s_cached_search) {
                return s_cached_result;
            }
            while(start_search != NULL
                && (reconf = dynamic_cast<rc_reconfigurable*>(
                    start_search = start_search->get_parent_object()))
                   == NULL);
        }
        s_cached_search = start_search;
        s_cached_result = reconf;
        return reconf;
    } else {
        return NULL;
    }
}

const rc_reconfigurable* rc_register_resettable(
    rc_resettable& resettable, sc_object* search_start)
{
    rc_reconfigurable* const reconf =
        rc_get_reconfigurable_context(search_start);
    if (reconf != NULL) {
        reconf->rc_register_resettable(resettable);
    }
    return reconf;
}

} // namespace ReChannel

//
// $Id: rc_reconfigurable.cpp,v 1.22 2008/01/01 13:34:00 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_reconfigurable.cpp,v $
//
