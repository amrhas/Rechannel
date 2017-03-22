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
 * @file    rc_abstract_exportal.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Code of class rc_abstract_exportal.
 *
 *
 * $Date: 2007/12/27 00:25:44 $
 * $Revision: 1.16 $
 */

#include "rc_abstract_exportal.h"

namespace ReChannel {

rc_exportal_base::rc_exportal_base(
    const sc_module_name& module_name,
    state_type& state, unsigned int& transaction_count,
    wrapper_pool& s_wrapper_pool_)
    : sc_module(module_name), p_state(state),
      p_transaction_count(transaction_count),
      s_wrapper_pool(s_wrapper_pool_),
      p_first_dyn_filter(NULL), p_dyn_module(NULL),
      p_exclusive_wrapper(NULL)
{ }

sc_interface* rc_exportal_base::get_dynamic_interface() const
{
    if (p_dyn_wrapper_handle.valid()) {
        return &p_dyn_wrapper_handle->get_wrapped_interface();
    } else {
        return NULL;
    }
}

void rc_exportal_base::bind_dynamic(rc_export_handle export_)
{
    if (!export_.valid()) {
        RC_REPORT_ERROR(RC_ID_NULL_POINTER_,
            "binding failed: invalid export handle given (in exportal '"
            << this->name() << "')");
    }
    if (sc_is_running()) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "binding attempt with export '" << export_->name()
            << "' at simulation time (in exportal '"
            << this->name() << "')");
    }

    rc_reconfigurable* module =
        rc_get_reconfigurable_context(export_->get_parent_object());
    if (module == NULL) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "binding attempt with non-reconfigurable export '"
            << export_->name()
            << "' (in exportal '" << this->name() << "')");
    }
    sc_interface* dyn_if = export_.get_interface();
    if (dyn_if == NULL) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "export '" << export_->name()
            << "' is unbound: interface expected"
            "(in exportal '" << this->name() << "')");
    }
    this->register_reconfigurable(*module, *dyn_if);
    module->rc_register_switch(*this, export_);
}

void rc_exportal_base::bind_dynamic(sc_interface& dynamic_if_)
{
    if (sc_is_running()) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "binding attempt with an interface"
            << " at simulation time (in exportal '"
            << this->name() << "')");
    }
    sc_object* channel_ = dynamic_cast<sc_object*>(&dynamic_if_);
    if (channel_ == NULL) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "binding attempt with an interface"
            << " that is no sc_object (in exportal '"
            << this->name() << "')");
    }
    rc_reconfigurable* module = rc_get_reconfigurable_context(channel_);
    if (module == NULL) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "binding attempt with an interface"
            << " from a non-reconfigurable context (in exportal '"
            << this->name() << "')");
    }
    this->register_reconfigurable(*module, dynamic_if_);
    module->rc_register_switch(*this, *channel_);
}

void rc_exportal_base::bind_exclusively(rc_interface_wrapper_base& wrapper)
{
    if (sc_is_running()) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "binding attempt with an interface"
            << " at simulation time (in exportal '"
            << this->name() << "')");
    }
    if (p_exclusive_wrapper != NULL) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "only one exclusively bound interface"
            " can be registered (in exportal '"
            << this->name() << "')");
    }
    sc_interface& dynamic_if_ = wrapper.get_wrapped_interface();
    sc_object* channel_ = dynamic_cast<sc_object*>(&dynamic_if_);
    if (channel_ == NULL) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "binding attempt with an interface"
            << " that is no sc_object (in exportal '"
            << this->name() << "')");
    }
    rc_reconfigurable* module = rc_get_reconfigurable_context(channel_);
    if (module == NULL) {
        module = &rc_non_reconfigurable::get_instance();
    }
    this->register_reconfigurable(*module, dynamic_if_);
    module->rc_register_switch(*this, *channel_);
    p_exclusive_wrapper = &wrapper;
}

void rc_exportal_base::open()
{
    if (p_state == rc_switch::CLOSED) {
        // check whether module is active
        if (p_dyn_module != NULL && p_dyn_module->rc_is_active()) {
            this->open(*p_dyn_module, p_dyn_filter_chain);
        } else {
            this->set_undefined();
        }
    }
}

void rc_exportal_base::open(
    rc_reconfigurable& module, const filter_chain& filters)
{
    // check whether module is bound to this exportal
    sc_interface* dyn_if = this->get_registered_interface(module);
    if (dyn_if == NULL) {
        // binding error, module is not registered at this exportal
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "dynamic module '" << module.rc_get_name()
            << "' is not registered (in exportal '"
            << this->name() << "')");
    }
    // is exportal open?
    if (p_state == rc_switch::OPEN) {
        if (&module == p_dyn_module) {
            if (filters != p_dyn_filter_chain) {
                // report "change of filter chain while open" error
                RC_REPORT_ERROR(RC_ID_SWITCH_CONFLICT_,
                    "cannot change interface filter chain while"
                    " exportal is open (in exportal '"
                    << this->name() << "')");
            } else {
                // exportal already open for this module (no changes)
                return;
            }
        } else {
            // report conflict with another module
            RC_REPORT_ERROR(RC_ID_SWITCH_CONFLICT_,
                "opening exportal for dynamic module '"
                << module.rc_get_name()
                << "' conflicts with currently active dynamic module"
                " '" << p_dyn_module->rc_get_name() << "'"
                << " (in exportal '" << this->name() << "')");
        }
    } else {
        // check if the previous wrapper handle was released
        assert(p_dyn_wrapper_handle.valid() == false);
    }

    // check whether module is active
    if (!module.rc_is_active()) {
        this->set_undefined();
        return;
    }

    // check whether given interface is compatible
    if (this->_rc_check_interface_type(dyn_if) == false) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "an invalid interface type '"  << typeid(dyn_if).name()
            << "' has been registered for dynamic module '"
            << module.rc_get_name()
            << " (in exportal '" << this->name() << "')");
    }

    // acquire the dynamic wrapper
    if (p_reserved_wrapper_handle.valid()
    && &p_reserved_wrapper_handle->get_wrapped_interface() == dyn_if) {
        // fetch the wrapper from the reserve slot
        p_dyn_wrapper_handle = p_reserved_wrapper_handle;
        // check if the reserve slot was invalidated correctly
        assert(p_reserved_wrapper_handle.valid() == false);
    } else {
        // check if we own an exclusive wrapper for this interface
        if (p_exclusive_wrapper != NULL
        && &p_exclusive_wrapper->get_wrapped_interface() == dyn_if) {
            // the exclusive wrapper is the new dynamic wrapper
            p_dyn_wrapper_handle = p_exclusive_wrapper;
        } else {
            // get the dynamic wrapper from the pool
            wrapper_handle handle = 
                rc_exportal_base::s_wrapper_pool.get(*dyn_if, this);
            p_dyn_wrapper_handle = handle;

            // set this exportal as the new owner of the interface wrapper
            // (this requires the interface wrapper to be of the internal
            //    interface wrapper type)
            this->_rc_set_interface_wrapper_owner(
                p_dyn_wrapper_handle.get_object());
        }
        // clear the restore slot
        p_reserved_wrapper_handle.release();
    }

    // get the static accessor
    accessor_base_type& static_accessor = this->_rc_get_static_accessor();

    // check if an interface filter shall be applied or not
    if (filters.empty()) {
        // connect the accessor with the wrapper
        static_accessor.rc_set_target(*p_dyn_wrapper_handle);
    } else {
        accessor_base_type* first_dyn_filter = NULL;
        rc_interface_filter* const last_dyn_filter = filters.back();

        // check if filter chain is different from the stored filter chain
        //   (may be the same if opened from within this exportal)
        if (&filters != &p_dyn_filter_chain) {
            // check if filters are compatible
            for (filter_chain::const_reverse_iterator it = filters.rbegin();
                it != filters.rend();
                ++it)
            {
                rc_interface_filter* filter_ = *it;
                first_dyn_filter =
                    dynamic_cast<accessor_base_type*>(filter_);
                if (first_dyn_filter == NULL
                || _rc_check_accessor_if_type(first_dyn_filter) == false)
                {
                    RC_REPORT_ERROR(RC_ID_SWITCH_INVALID_FILTER_,
                        "imcompatible filter object type '"
                        << typeid(filter_).name()
                        << "' supplied by dynamic module '"
                        << module.rc_get_name()
                        << "': accessor type expected"
                           " (in portal '" << this->name() << "')");
                }
            }
            // store a copy of the active filter chain
            p_dyn_filter_chain = filters;
        } else {
            // the filter chain has not changed
            // and thus the first filter is already known
            first_dyn_filter = p_first_dyn_filter;
        }

        // connect the first filter with the interface wrapper
        first_dyn_filter->rc_set_target(*p_dyn_wrapper_handle);
        // store the pointer of the applied filter
        p_first_dyn_filter = first_dyn_filter;
        // let the event forwarder target at the first filter
        this->add_event_forwarder_target(
            *first_dyn_filter, first_dyn_filter->rc_get_event_filter());
        // get the filter count
        const unsigned int count = filters.size();
        // connect the rest of the filter chain
        for (unsigned int i=1; i < count; ++i) {
            if (filters[i]->rc_set_target(*filters[i-1]) == false) {
                RC_REPORT_ERROR(RC_ID_SWITCH_INVALID_FILTER_,
                    "connecting the filter chain for "
                    " dynamic module '" << module.rc_get_name()
                    << "' failed (in exportal '" << this->name() << "')");
            }
            // let the event forwarder target at the filter
            this->add_event_forwarder_target(
                *filters[i], filters[i]->rc_get_event_filter());
        }
        // connect the accessor with the last filter
        if (static_accessor.rc_set_target(
            *last_dyn_filter, module.rc_get_process_control()) == false)
        {
            RC_REPORT_ERROR(RC_ID_SWITCH_INVALID_FILTER_,
                "connecting the accessor of "
                " dynamic module '" << module.rc_get_name()
                << "' failed (in exportal '" << this->name() << "')");
        }
    }
    // let the event forwarder target at the accessor
    this->add_event_forwarder_target(
        static_accessor, static_accessor.rc_get_event_filter());
    // store the pointer of the module
    p_dyn_module = &module;
    // set portal to opened state
    p_state = rc_switch::OPEN;
    // call user-defined event handler (if simulation is running)
    if (sc_is_running()) {
        this->rc_on_open();
    }
}

void rc_exportal_base::close()
{
    // check whether communication is currently taking place
    if (p_transaction_count > 0) {
        RC_REPORT_ERROR(RC_ID_SWITCH_CONFLICT_,
            "closing portal while communication in progress"
            " (in portal '" << this->name() << "')");
    }
    // set portal to closed state
    if (p_state != rc_switch::CLOSED) {
        //note: p_dyn_module and p_dyn_filter are kept (in contrast to
        //      method set_undefined()), because the module is still in
        //      place
        this->_rc_get_static_accessor().rc_clear_target();
        for (int i=p_dyn_filter_chain.size()-1; i >= 1; --i) {
            p_dyn_filter_chain[i]->rc_clear_target();
        }
        // cease event forwarding
        this->clear_event_forwarder_targets();
        // move wrapper to the reserve slot (equals a swapping)
        p_reserved_wrapper_handle = p_dyn_wrapper_handle;
        // set state to CLOSED
        p_state = rc_switch::CLOSED;
        // call user-defined event handler (if simulation is running)
        if (sc_is_running()) {
            this->rc_on_close();
        }
    }
}

void rc_exportal_base::set_undefined()
{
    // check whether communication is currently taking place
    if (p_transaction_count > 0) {
        RC_REPORT_ERROR(RC_ID_SWITCH_CONFLICT_,
            "changing portal to undefined state while communication"
            " in progress (in portal '" << this->name() << "')");
    }
    // set portal to undefined state
    if (p_state != rc_switch::UNDEF) {
        p_dyn_module = NULL;
        this->_rc_get_static_accessor().rc_clear_target();
        for (int i=p_dyn_filter_chain.size()-1; i >= 1; --i) {
            p_dyn_filter_chain[i]->rc_clear_target();
        }
        p_dyn_filter_chain.clear();
        p_first_dyn_filter = NULL;
        // cease event forwarding
        this->clear_event_forwarder_targets();
        // move wrapper to the reserve slot (equals a swapping)
        p_reserved_wrapper_handle = p_dyn_wrapper_handle;
        // set state to UNDEF
        p_state = rc_switch::UNDEF;
        // call user-defined event handler (if simulation is running)
        if (sc_is_running()) {
            // exportals do not have a special callback for undefined state
            this->rc_on_close();
        }
    }
}

void rc_exportal_base::register_reconfigurable(
        rc_reconfigurable& module, sc_interface& dyn_if)
{
    // check whether the dynamic interface is compatible
    if (this->_rc_check_interface_type(&dyn_if) == false) {
        RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
            "registering dynamic module '" << module.rc_get_name()
            << "' failed: compatible interface expected"
               " (in portal '" << this->name() << "')");
    }
}

void rc_exportal_base::unregister_reconfigurable(
    rc_reconfigurable& module)
{
    // there is only something to do if module is currently in use
    if (p_dyn_module == &module) {
        if (p_state == rc_switch::OPEN) {
            RC_REPORT_ERROR(RC_ID_SWITCH_BINDING_ERROR_,
                "unregistering dynamic module '" << module.rc_get_name()
                << "' while portal is open"
                   " (in portal '" << this->name() << "')");
        }
        assert(
            p_state == rc_switch::CLOSED
            || p_state == rc_switch::UNDEF);
        // reset members that are still storing module specific data
        p_reserved_wrapper_handle.release();
        p_first_dyn_filter = NULL;
        p_dyn_filter_chain.clear();
        p_dyn_module = NULL;
    }
}

void rc_exportal_base::notify_event(const std::string& event_name)
{
    if (p_dyn_wrapper_handle.valid()) {
        rc_event_forwarder_host* const event_forwarder_host =
            p_dyn_wrapper_handle->get_event_forwarder_host();
        if (event_forwarder_host != NULL) {
            event_forwarder_host->notify_event(event_name);
        }
    }
}

rc_interface_wrapper_base*
rc_exportal_base::get_interface_wrapper_base() const
{
    if (p_dyn_wrapper_handle.valid()) {
        return p_dyn_wrapper_handle.get_object();
    } else if (p_reserved_wrapper_handle.valid()) {
        return p_reserved_wrapper_handle.get_object();
    } else {
        return NULL;
    }
}

void rc_exportal_base::add_event_forwarder_target(
    sc_interface& target_if, rc_event_filter* filter)
{
    rc_event_forwarder_host* const event_finder_host =
        p_dyn_wrapper_handle->get_event_forwarder_host();
    if (event_finder_host == NULL) {
        RC_REPORT_ERROR(RC_ID_INTERNAL_ERROR_,
            "interface wrapper doesn't support event forarding"
               " (in exportal '" << this->name() << "')");
    }
    event_finder_host->add_event_forwarder_target(target_if, filter);
}

void rc_exportal_base::clear_event_forwarder_targets()
{
    rc_event_forwarder_host* const event_finder_host =
        p_dyn_wrapper_handle->get_event_forwarder_host();
    if (event_finder_host != NULL) {
        event_finder_host->clear_event_forwarder_targets();
    }
}

} // namespace ReChannel

//
// $Id: rc_abstract_exportal.cpp,v 1.16 2007/12/27 00:25:44 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/exportals/rc_abstract_exportal.cpp,v $
//
