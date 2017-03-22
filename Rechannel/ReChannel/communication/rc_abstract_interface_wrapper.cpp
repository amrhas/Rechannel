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
 * @file    rc_abstract_interface_wrapper.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2008/01/01 13:47:37 $
 * $Revision: 1.12 $
 */

#include "rc_abstract_interface_wrapper.h"

namespace ReChannel {

rc_abstract_interface_wrapper_b::event_ef_map
    rc_abstract_interface_wrapper_b::s_event_ef_map;

rc_abstract_interface_wrapper_b::rc_abstract_interface_wrapper_b(void* if_)
    : p_interface(if_)
{ }

rc_abstract_interface_wrapper_b::~rc_abstract_interface_wrapper_b()
{
    event_forwarder_base_map::iterator it;
    for(it = p_event_forwarder_base_map.begin();
        it != p_event_forwarder_base_map.end(); it++) {
        delete it->second;
    }
    p_event_forwarder_base_map.clear();
}

void rc_abstract_interface_wrapper_b::missing_fallback_if_error() const
{
    RC_REPORT_ERROR(RC_ID_FALLBACK_INTERFACE_MISSING_,
        "a fallback interface doesn't exist (in interface wrapper '"
        << get_interface_wrapper_name() << "')");
}

void rc_abstract_interface_wrapper_b::set_wrapped_if(void* if_)
{
    p_interface = if_;

    // update blocking driver's channel reference
    for (vector::iterator it =
        p_drv_obj_vector.begin();
        it != p_drv_obj_vector.end();
        it++)
    {
        void* drv = *it;
        if (drv != NULL) {
            this->_rc_set_driver_channel(drv, if_);
        }
    }
    // update non-blocking driver's channel reference
    for (vector::iterator it =
        p_nb_drv_obj_vector.begin();
        it != p_nb_drv_obj_vector.end();
        it++)
    {
        void* nb_drv = *it;
        if (nb_drv != NULL) {
            this->_rc_set_nb_driver_channel(nb_drv, if_);
        }
    }
}

void* rc_abstract_interface_wrapper_b::create_driver_object(int index)
{
    if (index >= RC_INTERFACE_WRAPPER_MAX_DRIVER_COUNT) {
        RC_REPORT_ERROR(RC_ID_OUT_OF_BOUNDS_,
            "RC_INTERFACE_WRAPPER_MAX_DRIVER_COUNT has been exceeded"
            " (in interface wrapper '"
            << this->get_interface_wrapper_name() << "')");
    } else if (index < 0) {
        RC_REPORT_ERROR(RC_ID_OUT_OF_BOUNDS_,
            "driver index out of bounds"
            " (in interface wrapper '"
            << this->get_interface_wrapper_name() << "')");
    }
    if (index >= (int)p_drv_obj_vector.size()) {
        p_drv_obj_vector.resize(index + 1);
    }
    return (p_drv_obj_vector[index] =
        this->_rc_create_driver_object(p_interface));
}

void* rc_abstract_interface_wrapper_b::create_nb_driver_object(int index)
{
    if (index >= RC_INTERFACE_WRAPPER_MAX_DRIVER_COUNT) {
        RC_REPORT_ERROR(RC_ID_OUT_OF_BOUNDS_,
            "RC_INTERFACE_WRAPPER_MAX_DRIVER_COUNT has been exceeded"
            " (in interface wrapper '"
            << this->get_interface_wrapper_name() << "')");
    } else if (index < 0) {
        RC_REPORT_ERROR(RC_ID_OUT_OF_BOUNDS_,
            "driver index out of bounds "
            " (in interface wrapper '"
            << this->get_interface_wrapper_name() << "')");
    }
    if (index >= (int)p_nb_drv_obj_vector.size()) {
        p_nb_drv_obj_vector.resize(index + 1);
    }
    return (p_nb_drv_obj_vector[index] =
        this->_rc_create_nb_driver_object(p_interface));
}

void rc_abstract_interface_wrapper_b::delete_all_drivers()
{
    for(vector::iterator it = p_drv_obj_vector.begin();
        it != p_drv_obj_vector.end(); it++) {
        this->_rc_delete_driver_object(*it);
    }
    p_drv_obj_vector.clear();

    for(vector::iterator it = p_nb_drv_obj_vector.begin();
        it != p_nb_drv_obj_vector.end(); it++) {
        this->_rc_delete_nb_driver_object(*it);
    }
    p_nb_drv_obj_vector.clear();
}

void rc_abstract_interface_wrapper_b::delete_all_event_forwarders()
{
    event_forwarder_base_map::iterator it;
    for(it = p_event_forwarder_base_map.begin();
        it != p_event_forwarder_base_map.end();
        it++)
    {
        delete it->second;
        // note: deleting the associated spawned process is not possible
    }
    p_event_forwarder_base_map.clear();
}

void rc_abstract_interface_wrapper_b::add_event_forwarder(
    rc_event_forwarder_base& forwarder, const sc_event& e,
    const std::string& event_name)
{
    std::string _event_name(event_name);
    if (_event_name.empty()) {
        _event_name = sc_gen_unique_name("event");
    }
    rc_event_forwarder_base*& item_ref = 
        p_event_forwarder_base_map[_event_name];
    if (item_ref != NULL) {
        RC_REPORT_ERROR(RC_ID_DUPLICATE_ENTRY_,
            "event forwarder for event '" << _event_name
            << "' already exists"
            " (in interface wrapper '"
            << this->get_interface_wrapper_name() << "')");
    }
    item_ref = &forwarder;
    this->_rc_spawn_event_forwarder_process(forwarder, e);
}

void rc_abstract_interface_wrapper_b::add_event_forwarder_target(
    sc_interface& if_, rc_event_filter* filter)
{
    event_forwarder_base_map::iterator it;
    for(it = p_event_forwarder_base_map.begin();
        it != p_event_forwarder_base_map.end();
        it++)
    { it->second->add_target(if_, filter); }
}

bool rc_abstract_interface_wrapper_b::add_event_forwarder_target(
    const std::string& event_name, sc_interface& if_,
    rc_event_filter* filter)
{
    event_forwarder_base_map::iterator it =
        p_event_forwarder_base_map.find(event_name);
    if (it != p_event_forwarder_base_map.end()) {
        return it->second->add_target(if_, filter);
    } else {
        return false;
    }
}

void rc_abstract_interface_wrapper_b::clear_event_forwarder_targets()
{
    event_forwarder_base_map::iterator it;
    for(it = p_event_forwarder_base_map.begin();
        it != p_event_forwarder_base_map.end();
        it++)
    { it->second->clear_targets(); }
}

void rc_abstract_interface_wrapper_b::clear_event_forwarder_targets(
    const std::string& event_name)
{
    event_forwarder_base_map::iterator it =
        p_event_forwarder_base_map.find(event_name);
    if (it != p_event_forwarder_base_map.end()) {
        return it->second->clear_targets();
    }
}

rc_abstract_interface_wrapper_b::string_vector
rc_abstract_interface_wrapper_b::get_event_forwarder_names() const
{
    string_vector names;
    names.reserve(p_event_forwarder_base_map.size());
    event_forwarder_base_map::const_iterator it;
    for(it = p_event_forwarder_base_map.begin();
        it != p_event_forwarder_base_map.end();
        it++)
    { names.push_back(it->first); }
    return names;
}

void rc_abstract_interface_wrapper_b::notify_event(
    const std::string& event_name)
{
    rc_event_forwarder_base& event_forwarder =
        this->_rc_get_event_forwarder(event_name);
    event_forwarder.trigger_event();
}

rc_event_forwarder_base&
rc_abstract_interface_wrapper_b::_rc_get_event_forwarder(
    const std::string& event_name)
{
    event_forwarder_base_map::iterator it =
        p_event_forwarder_base_map.find(event_name);
    if (it == p_event_forwarder_base_map.end()) {
        RC_REPORT_ERROR(RC_ID_DUPLICATE_ENTRY_,
            "event '" << event_name << "' does not exist"
            " (in interface wrapper '"
            << this->get_interface_wrapper_name() << "')");
    }
    return *(it->second);
}

void rc_abstract_interface_wrapper_b::_rc_spawn_event_forwarder_process(
    rc_event_forwarder_base& forwarder, const sc_event& e)
{
    event_ef_map::iterator it = s_event_ef_map.find(&e);
    if (it == s_event_ef_map.end()) {
        event_ef_map_entry& new_map_entry_ = s_event_ef_map[&e];
        new_map_entry_.first = &forwarder;

        sc_spawn_options opt;
        {
            opt.spawn_method();
            opt.set_sensitivity(&e);
            opt.dont_initialize();
        }
        sc_spawn(
            sc_bind(
                &rc_abstract_interface_wrapper_b::_rc_event_forwarder_proc,
                sc_ref(new_map_entry_)),
            sc_gen_unique_name("_rc_event_forwarder_proc"), &opt);
    } else {
        event_ef_map_entry& map_entry_ = it->second;
        ef_vector& ef_vector_ = map_entry_.second;
        ef_vector_.push_back(&forwarder);
    }
}

void rc_abstract_interface_wrapper_b::_rc_event_forwarder_proc(
    event_ef_map_entry& map_entry_)
{
    map_entry_.first->trigger_event();
    ef_vector& ef_vector_ = map_entry_.second;
    if (!ef_vector_.empty()) {
        for (ef_vector::iterator it = ef_vector_.begin();
            it != ef_vector_.end();
            ++it)
        {
            (*it)->trigger_event();
        }
    }
}

} // namespace ReChannel
//
// $Id: rc_abstract_interface_wrapper.cpp,v 1.12 2008/01/01 13:47:37 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/rc_abstract_interface_wrapper.cpp,v $
//
