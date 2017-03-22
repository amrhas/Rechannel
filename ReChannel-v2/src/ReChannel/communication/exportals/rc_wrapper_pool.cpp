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
 * @file    rc_wrapper_pool.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Code of internal class wrapper_pool.
 *
 *
 * $Date: 2007/12/06 12:12:06 $
 * $Revision: 1.1 $
 */

#include "rc_wrapper_pool.h"

namespace ReChannel {
namespace internals {
namespace exportal {

wrapper_pool::~wrapper_pool()
{
    for(if_wrapper_multimap::iterator it = p_if_wrapper_multimap.begin();
        it != p_if_wrapper_multimap.end();
        ++it)
    {
        delete it->second;
    }
    p_if_wrapper_multimap.clear();
}

wrapper_handle wrapper_pool::get(
   sc_interface& wrapped_if, wrapper_factory* factory)
{
    rc_interface_wrapper_base* wrapper = NULL;

    if_wrapper_multimap::iterator it =
        p_if_wrapper_multimap.find(&wrapped_if);
    if (it != p_if_wrapper_multimap.end()) {
        wrapper = it->second;
        p_if_wrapper_multimap.erase(it);
    }
    if (wrapper == NULL && factory != NULL) {
        wrapper = factory->create_interface_wrapper(wrapped_if);
    }
    if (wrapper != NULL) {
        ++p_num_taken;
    }
    return wrapper_handle(*this, wrapper);
}

void wrapper_pool::add(
    rc_interface_wrapper_base& wrapper)
{
    sc_interface& wrapped_if = wrapper.get_wrapped_interface();
    p_if_wrapper_multimap.insert(
        if_wrapper_multimap::value_type(&wrapped_if, &wrapper));
}

void wrapper_pool::release(
    wrapper_handle& wrapper_handle_)
{
    assert(wrapper_handle_.p_wrapper_pool == this);

    rc_interface_wrapper_base* wrapper = wrapper_handle_.p_wrapper;
    if (wrapper != NULL) {
        sc_interface& wrapped_if = wrapper->get_wrapped_interface();
        p_if_wrapper_multimap.insert(
                if_wrapper_multimap::value_type(&wrapped_if, wrapper));
        if (p_num_taken > 0) {
            --p_num_taken;
        }
    }
    wrapper_handle_.p_wrapper_pool = NULL;
}

wrapper_handle& wrapper_handle::operator=(
    wrapper_handle& orig)
{
    this->release();
    if (orig.p_wrapper != NULL) {
        p_wrapper_pool = orig.p_wrapper_pool;
        p_wrapper = orig.p_wrapper;
        orig.p_wrapper = NULL;
    }
    return *this;
}

wrapper_handle& wrapper_handle::operator=(
    rc_interface_wrapper_base* wrapper)
{
    this->release();
    p_wrapper = wrapper;
    return *this;
}

} // namespace exportal
} // namespace internals
} // namespace ReChannel

//
// $Id: rc_wrapper_pool.cpp,v 1.1 2007/12/06 12:12:06 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/exportals/rc_wrapper_pool.cpp,v $
//
