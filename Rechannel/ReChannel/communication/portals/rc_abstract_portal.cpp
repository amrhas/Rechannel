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
 * @file    rc_portal.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Code of class rc_portal.
 *
 *
 * $Date: 2007/12/20 20:27:30 $
 * $Revision: 1.9 $
 */

#include "rc_abstract_portal.h"

namespace ReChannel {

rc_portal_base::rc_portal_base(
    const sc_module_name& module_name,
    state_type& state, unsigned int& transaction_count)
    : sc_module(module_name), p_state(state),
      p_transaction_count(transaction_count),
      p_interface_wrapper(NULL), p_dyn_accessor(NULL),
      p_last_dyn_filter(NULL), p_dyn_module(NULL)
{ }

void rc_portal_base::_rc_init(rc_interface_wrapper_base& interface_wrapper)
{
    // check correct usage
    assert(p_interface_wrapper == NULL);

    // initialize (if necessary)
    if (p_interface_wrapper == NULL) {
        p_interface_wrapper = &interface_wrapper;
    }
}

void rc_portal_base::register_port(rc_port_handle port_)
{
    if (!port_.valid()) {
        RC_REPORT_ERROR(RC_ID_NULL_POINTER_,
            "binding failed: invalid port handle given (in portal '"
            << this->name() << "')");
    }
    if (sc_is_running()) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "binding attempt with port '" << port_->name()
            << "' at simulation time (in portal '"
            << this->name() << "')");
    }

    rc_reconfigurable* module =
        rc_get_reconfigurable_context(port_->get_parent_object());
    if (module == NULL) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "binding attempt with non-reconfigurable port '"
            << port_->name()
            << "' (in portal '" << this->name() << "')");
    }
    sc_interface* dyn_if = port_.get_interface();
    if (dyn_if == NULL) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "port '" << port_->name()
            << "' is not bound: accessor expected"
            " (in portal '" << this->name() << "')");
    }
    this->register_reconfigurable(*module, *dyn_if);
    module->rc_register_switch(*this, port_);
}

void rc_portal_base::open()
{
    if (p_state == rc_switch::CLOSED) {
        // check whether module is active
        if (p_dyn_module != NULL && p_dyn_module->rc_is_active()) {
            // open portal
            this->open(*p_dyn_module, p_dyn_filter_chain);
        } else {
            this->set_undefined();
        }
    }
}

void rc_portal_base::open(
    rc_reconfigurable& module, const filter_chain& filters)
{
    // check valid initialization
    assert(p_interface_wrapper != NULL);

    // check whether module is bound to this portal
    sc_interface* dyn_if = this->get_registered_interface(module);
    if (dyn_if == NULL) {
        // binding error, module is not registered at this portal
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "reconfigurable module '" << module.rc_get_name()
            << "' is not registered (in portal '"
            << this->name() << "')");
    }
    // is portal open?
    if (p_state == rc_switch::OPEN) {
        if (&module == p_dyn_module) {
            if (filters != p_dyn_filter_chain) {
                // report "change of filter chain while open" error
                RC_REPORT_ERROR(RC_ID_SWITCH_CONFLICT_,
                    "cannot change interface filter chain while"
                    " portal is open (in portal '"
                    << this->name() << "')");
            } else {
                // portal already open for this module (no changes)
                return;
            }
        } else {
            // report conflict with another module
            RC_REPORT_ERROR(RC_ID_SWITCH_CONFLICT_,
                "opening portal for reconfigurable module '"
                << module.rc_get_name()
                << "' conflicts with currently active reconfigurable module"
                " '" << p_dyn_module->rc_get_name() << "'"
                << " (in portal '" << this->name() << "')");
        }
    }
    // check whether module is active
    if (!module.rc_is_active()) {
        this->set_undefined();
        return;
    }

    // check whether given interface is a compatible accessor
    accessor_base_type* dyn_accessor =
        dynamic_cast<accessor_base_type*>(dyn_if);
    if (dyn_accessor == NULL
       || _rc_check_accessor_if_type(dyn_accessor) == false)
    {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "an invalid accessor type '"  << typeid(dyn_if).name()
            << "' has been registered for reconfigurable module '"
            << module.rc_get_name()
            << " (in portal '" << this->name() << "')");
    }
    // check if an interface filter shall be applied or not
    if (filters.empty()) {
        // connect accessor with portal
        dyn_accessor->rc_set_target(*p_interface_wrapper);
    } else {
        rc_interface_filter* const first_dyn_filter = filters.front();
        accessor_base_type* last_dyn_filter = NULL;

        // check if filter chain is different from the stored filter chain
        //   (may be the same if opened from within this portal)
        if (&filters != &p_dyn_filter_chain) {
            // check if filters are compatible
            for (filter_chain::const_iterator it = filters.begin();
                it != filters.end();
                ++it)
            {
                rc_interface_filter* filter_ = *it;
                last_dyn_filter =
                    dynamic_cast<accessor_base_type*>(filter_);
                if (last_dyn_filter == NULL
                || _rc_check_accessor_if_type(last_dyn_filter) == false)
                {
                    RC_REPORT_ERROR(RC_ID_SWITCH_INVALID_FILTER_,
                        "imcompatible filter object type '"
                        << typeid(filter_).name()
                        << "' supplied by reconfigurable module '"
                        << module.rc_get_name()
                        << "': accessor type expected"
                           " (in portal '" << this->name() << "')");
                }
            }
            // store a copy of the active filter chain
            p_dyn_filter_chain = filters;

        } else {
            // the filter chain has not changed
            // and thus the last filter is already known
            last_dyn_filter = p_last_dyn_filter;
        }

        // connect the last filter with the interface wrapper
        last_dyn_filter->rc_set_target(*p_interface_wrapper);
        // store the pointer of the applied filter
        p_last_dyn_filter = last_dyn_filter;
        // firstly, let the event forwarder target at the last filter
        //    (the order of the event forwarders is essential!)
        this->add_event_forwarder_target(
            *last_dyn_filter, last_dyn_filter->rc_get_event_filter());
        // connect the rest of the filter chain (reverse direction!)
        for (int i=filters.size()-2; i >= 0; --i) {
            if (filters[i]->rc_set_target(*filters[i+1]) == false) {
                RC_REPORT_ERROR(RC_ID_SWITCH_INVALID_FILTER_,
                    "connecting the filter chain for "
                    " reconfigurable module '" << module.rc_get_name()
                    << "' failed (in portal '" << this->name() << "')");
            }
            // let the event forwarder target at the filter
            //    (the order of the event forwarders is essential!)
            this->add_event_forwarder_target(
                *filters[i], filters[i]->rc_get_event_filter());
        }
        // connect the accessor with the first filter
        if (dyn_accessor->rc_set_target(
            *first_dyn_filter, module.rc_get_process_control()) == false)
        {
            RC_REPORT_ERROR(RC_ID_SWITCH_INVALID_FILTER_,
                "connecting the accessor of "
                " reconfigurable module '" << module.rc_get_name()
                << "' failed (in portal '" << this->name() << "')");
        }
    }
    // store the pointer of the accessor
    p_dyn_accessor = dyn_accessor;
    // let the event forwarder target at the accessor
    this->add_event_forwarder_target(
        *dyn_accessor, dyn_accessor->rc_get_event_filter());
    // store the pointer of the module
    p_dyn_module = &module;
    // set portal to opened state
    p_state = rc_switch::OPEN;
    // call user-defined event handler (if simulation is running)
    if (sc_is_running()) {
        this->rc_on_open();
    }
}

void rc_portal_base::close()
{
    // set portal to closed state
    if (p_state != rc_switch::CLOSED) {
        // check whether communication is currently taking place
        if (p_state == rc_switch::OPEN && p_transaction_count > 0) {
            RC_REPORT_ERROR(RC_ID_SWITCH_CONFLICT_,
                "closing portal while communication in progress"
                " (in portal '" << this->name() << "')");
        }
        //note: p_dyn_module and p_last_dyn_filter_chain are kept
        //      (in contrast to method set_undefined()), because the
        //      module is still in place
        p_dyn_accessor->rc_clear_target();
        p_dyn_accessor = NULL;
        for (int i=p_dyn_filter_chain.size()-1; i >= 1; --i) {
            p_dyn_filter_chain[i]->rc_clear_target();
        }
        // cease event forwarding
        this->clear_event_forwarder_targets();
        // set state to CLOSED
        p_state = rc_switch::CLOSED;
        // call user-defined event handler (if simulation is running)
        if (sc_is_running()) {
            this->rc_on_close();
        }
    }
}

void rc_portal_base::set_undefined()
{
    // set portal to undefined state
    if (p_state != rc_switch::UNDEF) {
        // check whether communication is currently taking place
        if (p_state == rc_switch::OPEN && p_transaction_count > 0) {
            RC_REPORT_ERROR(RC_ID_SWITCH_CONFLICT_,
                "changing portal to undefined state while communication"
                " in progress (in portal '" << this->name() << "')");
        }

        p_dyn_module = NULL;
        p_dyn_accessor->rc_clear_target();
        p_dyn_accessor = NULL;
        for (int i=p_dyn_filter_chain.size()-1; i >= 1; --i) {
            p_dyn_filter_chain[i]->rc_clear_target();
        }
        p_dyn_filter_chain.clear();
        p_last_dyn_filter = NULL;
        // cease event forwarding
        this->clear_event_forwarder_targets();
        // set state to UNDEF
        p_state = rc_switch::UNDEF;
        // call user-defined event handler (if simulation is running)
        if (sc_is_running()) {
            this->rc_on_undef();
        }
    }
}

void rc_portal_base::register_reconfigurable(
        rc_reconfigurable& module, sc_interface& dyn_if)
{
    // check whether the dynamic interface is a compatible accessor
    accessor_base_type* dyn_accessor =
        dynamic_cast<accessor_base_type*>(&dyn_if);
    if (dyn_accessor == NULL
    || _rc_check_accessor_if_type(dyn_accessor) == false) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "registering reconfigurable module '" << module.rc_get_name()
            << "' failed: compatible accessor object expected"
               " (in portal '" << this->name() << "')");
    }
    // set the accessor initially disabled
    dyn_accessor->rc_clear_target();
}

void rc_portal_base::unregister_reconfigurable(rc_reconfigurable& module)
{
    // there is only something to do if module is currently in use
    if (p_dyn_module == &module) {
        if (p_state == rc_switch::OPEN) {
            RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
                "unregistering reconfigurable module '" << module.rc_get_name()
                << "' while portal is open"
                   " (in portal '" << this->name() << "')");
        }
        assert(
            p_state == rc_switch::CLOSED
            || p_state == rc_switch::UNDEF);
        // reset members that are still storing module specific data
        //   (note: event forwarding has already been suspended)
        p_last_dyn_filter = NULL;
        p_dyn_filter_chain.clear();
        p_dyn_module = NULL;
    }
}

void rc_portal_base::add_event_forwarder_target(
    sc_interface& target_if, rc_event_filter* filter)
{
    // check valid initialization
    assert(p_interface_wrapper != NULL);

    rc_event_forwarder_host* event_forwarder_host =
        p_interface_wrapper->get_event_forwarder_host();
    if (event_forwarder_host == NULL) {
        RC_REPORT_ERROR(RC_ID_INTERNAL_ERROR_,
            "interface wrapper doesn't support event forarding"
               " (in portal '" << this->name() << "')");
    }
    event_forwarder_host->add_event_forwarder_target(target_if, filter);
}

void rc_portal_base::clear_event_forwarder_targets()
{
    // check valid initialization
    assert(p_interface_wrapper != NULL);

    rc_event_forwarder_host* event_forwarder_host =
        p_interface_wrapper->get_event_forwarder_host();
    if (event_forwarder_host != NULL) {
        event_forwarder_host->clear_event_forwarder_targets();
    }
}

} // namespace ReChannel
//
// $Id: rc_abstract_portal.cpp,v 1.9 2007/12/20 20:27:30 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/portals/rc_abstract_portal.cpp,v $
//
