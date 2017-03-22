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
 * @file    rc_transaction_counter.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class rc_transaction_counter
 *
 *
 * $Date: 2007/12/20 20:42:13 $
 * $Revision: 1.6 $
 */

#include "rc_transaction_counter.h"

namespace ReChannel {

rc_transaction_counter::rc_transaction_counter()
    : sc_object(sc_gen_unique_name("_rc_transaction_counter")), p_count(0)
{
    p_reconf = rc_get_reconfigurable_context(this->get_parent_object());
}

rc_transaction_counter::rc_transaction_counter(
    std::vector<this_type*>& children)
    : sc_object(sc_gen_unique_name("_rc_transaction_counter")), p_count(0)
{
    p_reconf = rc_get_reconfigurable_context(this->get_parent_object());

    for (std::vector<this_type*>::iterator it = children.begin();
        it != children.end();
        ++it)
    {
        rc_transaction_counter* const child = *it;
        if (child != NULL) {
            this->_rc_add_child(*child);
        }
    }
}

void rc_transaction_counter::modify(int amount)
{
    if (p_children.empty()) {
        const int prev_p_count = p_count;
        p_count += amount;
        if (prev_p_count >= 0) {
            if (p_count < 0) {
                amount -= p_count;
            }
        } else { //(prev_p_count < 0)
            amount = (p_count > 0 ? p_count : 0);
        }
        if (p_reconf != NULL && amount != 0) {
            if (p_reconf->p_transaction_count > -amount) {
                p_reconf->p_transaction_count += amount;
            } else {
                p_reconf->reset_transaction_count();
            }
        }
    } else {
        this->_rc_relay(amount);
    }
}

void rc_transaction_counter::reset()
{
    if (p_count != 0 && p_children.empty()) {
        this->modify(-p_count);
    }
}

int rc_transaction_counter::get_relay_factor(int index) const
{
    if (index >= 0 && index < (int)p_children.size()) {
        return p_children[index].second;
    } else {
        return 0;
    }
}

void rc_transaction_counter::set_relay_factor(int index, int factor)
{
    if (index >= 0 && index < (int)p_children.size()) {
        p_children[index].second = factor;
    }
}

void rc_transaction_counter::set_relay_factor(
    rc_transaction_counter& child, int factor)
{
    for (std::vector<std::pair<this_type*, int> >::iterator it =
            p_children.begin();
        it != p_children.end();
        ++it)
    {
        if (it->first == &child) {
            it->second = factor;
        }
    }
}

void rc_transaction_counter::_rc_relay(int amount)
{
    if (amount != 0) {
        for (std::vector<std::pair<this_type*, int> >::iterator it =
                p_children.begin();
            it != p_children.end();
            ++it)
        {
            const int factor = it->second;
            if (factor != 0) {
                it->first->modify(amount * factor);
            }
        }
    }
}

void rc_transaction_counter::_rc_add_child(
    rc_transaction_counter& child, int factor)
{
    p_children.push_back(std::pair<this_type*, int>(&child, factor));
}

/* convenience constructors */

rc_transaction_counter::rc_transaction_counter(this_type& tc1, int factor1)
    : sc_object(sc_gen_unique_name("_rc_transaction_counter")), p_count(0)
{
    p_reconf = rc_get_reconfigurable_context(this->get_parent_object());
    this->_rc_add_child(tc1, factor1);
}

rc_transaction_counter::rc_transaction_counter(
    this_type& tc1, int factor1, this_type& tc2, int factor2)
    : sc_object(sc_gen_unique_name("_rc_transaction_counter")), p_count(0)
{
    p_reconf = rc_get_reconfigurable_context(this->get_parent_object());
    this->_rc_add_child(tc1, factor1);
    this->_rc_add_child(tc2, factor2);
}

rc_transaction_counter::rc_transaction_counter(
    this_type& tc1, int factor1, this_type& tc2, int factor2,
    this_type& tc3, int factor3)
    : sc_object(sc_gen_unique_name("_rc_transaction_counter")), p_count(0)
{
    p_reconf = rc_get_reconfigurable_context(this->get_parent_object());
    this->_rc_add_child(tc1, factor1);
    this->_rc_add_child(tc2, factor2);
    this->_rc_add_child(tc3, factor3);
}

rc_transaction_counter::rc_transaction_counter(
    this_type& tc1, int factor1, this_type& tc2, int factor2,
    this_type& tc3, int factor3, this_type& tc4, int factor4)
    : sc_object(sc_gen_unique_name("_rc_transaction_counter")), p_count(0)
{
    p_reconf = rc_get_reconfigurable_context(this->get_parent_object());
    this->_rc_add_child(tc1, factor1);
    this->_rc_add_child(tc2, factor2);
    this->_rc_add_child(tc3, factor3);
    this->_rc_add_child(tc4, factor4);
}

rc_transaction_counter::rc_transaction_counter(
    this_type& tc1, int factor1, this_type& tc2, int factor2,
    this_type& tc3, int factor3, this_type& tc4, int factor4,
    this_type& tc5, int factor5)
    : sc_object(sc_gen_unique_name("_rc_transaction_counter")), p_count(0)
{
    p_reconf = rc_get_reconfigurable_context(this->get_parent_object());
    this->_rc_add_child(tc1, factor1);
    this->_rc_add_child(tc2, factor2);
    this->_rc_add_child(tc3, factor3);
    this->_rc_add_child(tc4, factor4);
    this->_rc_add_child(tc5, factor5);
}

rc_transaction_counter::rc_transaction_counter(
    this_type& tc1, int factor1, this_type& tc2, int factor2,
    this_type& tc3, int factor3, this_type& tc4, int factor4,
    this_type& tc5, int factor5, this_type& tc6, int factor6)
    : sc_object(sc_gen_unique_name("_rc_transaction_counter")), p_count(0)
{
    p_reconf = rc_get_reconfigurable_context(this->get_parent_object());
    this->_rc_add_child(tc1, factor1);
    this->_rc_add_child(tc2, factor2);
    this->_rc_add_child(tc3, factor3);
    this->_rc_add_child(tc4, factor4);
    this->_rc_add_child(tc5, factor5);
    this->_rc_add_child(tc6, factor6);
}

rc_transaction_counter::rc_transaction_counter(
    this_type& tc1, int factor1, this_type& tc2, int factor2,
    this_type& tc3, int factor3, this_type& tc4, int factor4,
    this_type& tc5, int factor5, this_type& tc6, int factor6,
    this_type& tc7, int factor7)
    : sc_object(sc_gen_unique_name("_rc_transaction_counter")), p_count(0)
{
    p_reconf = rc_get_reconfigurable_context(this->get_parent_object());
    this->_rc_add_child(tc1, factor1);
    this->_rc_add_child(tc2, factor2);
    this->_rc_add_child(tc3, factor3);
    this->_rc_add_child(tc4, factor4);
    this->_rc_add_child(tc5, factor5);
    this->_rc_add_child(tc6, factor6);
    this->_rc_add_child(tc7, factor7);
}

rc_transaction_counter::rc_transaction_counter(
    this_type& tc1, int factor1, this_type& tc2, int factor2,
    this_type& tc3, int factor3, this_type& tc4, int factor4,
    this_type& tc5, int factor5, this_type& tc6, int factor6,
    this_type& tc7, int factor7, this_type& tc8, int factor8)
    : sc_object(sc_gen_unique_name("_rc_transaction_counter")), p_count(0)
{
    p_reconf = rc_get_reconfigurable_context(this->get_parent_object());
    this->_rc_add_child(tc1, factor1);
    this->_rc_add_child(tc2, factor2);
    this->_rc_add_child(tc3, factor3);
    this->_rc_add_child(tc4, factor4);
    this->_rc_add_child(tc5, factor5);
    this->_rc_add_child(tc6, factor6);
    this->_rc_add_child(tc7, factor7);
    this->_rc_add_child(tc8, factor8);
}

rc_transaction_counter::rc_transaction_counter(
    this_type& tc1, int factor1, this_type& tc2, int factor2,
    this_type& tc3, int factor3, this_type& tc4, int factor4,
    this_type& tc5, int factor5, this_type& tc6, int factor6,
    this_type& tc7, int factor7, this_type& tc8, int factor8,
    this_type& tc9, int factor9)
    : sc_object(sc_gen_unique_name("_rc_transaction_counter")), p_count(0)
{
    p_reconf = rc_get_reconfigurable_context(this->get_parent_object());
    this->_rc_add_child(tc1, factor1);
    this->_rc_add_child(tc2, factor2);
    this->_rc_add_child(tc3, factor3);
    this->_rc_add_child(tc4, factor4);
    this->_rc_add_child(tc5, factor5);
    this->_rc_add_child(tc6, factor6);
    this->_rc_add_child(tc7, factor7);
    this->_rc_add_child(tc8, factor8);
    this->_rc_add_child(tc9, factor9);
}

rc_transaction_counter::rc_transaction_counter(
    this_type& tc1, int factor1, this_type& tc2, int factor2,
    this_type& tc3, int factor3, this_type& tc4, int factor4,
    this_type& tc5, int factor5, this_type& tc6, int factor6,
    this_type& tc7, int factor7, this_type& tc8, int factor8,
    this_type& tc9, int factor9, this_type& tc10, int factor10)
    : sc_object(sc_gen_unique_name("_rc_transaction_counter")), p_count(0)
{
    p_reconf = rc_get_reconfigurable_context(this->get_parent_object());
    this->_rc_add_child(tc1, factor1);
    this->_rc_add_child(tc2, factor2);
    this->_rc_add_child(tc3, factor3);
    this->_rc_add_child(tc4, factor4);
    this->_rc_add_child(tc5, factor5);
    this->_rc_add_child(tc6, factor6);
    this->_rc_add_child(tc7, factor7);
    this->_rc_add_child(tc8, factor8);
    this->_rc_add_child(tc9, factor9);
    this->_rc_add_child(tc10, factor10);
}

} // namespace ReChannel

//
// $Id: rc_transaction_counter.cpp,v 1.6 2007/12/20 20:42:13 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_transaction_counter.cpp,v $
//
