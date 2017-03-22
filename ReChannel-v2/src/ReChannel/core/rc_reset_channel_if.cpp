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
 * @file    rc_reset_channel_if.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Implementations of reset signals.
 *
 *
 * $Date: 2007/10/09 21:47:01 $
 * $Revision: 1.5 $
 */

#include "rc_reset_channel_if.h"

namespace ReChannel {

namespace internals {

void reset_signal::register_process_control(
    rc_process_control& pctrl, bool active_level) const
{
    p_pctrl_set[(active_level ? 1 : 0)].insert(&pctrl);
}

void reset_signal::unregister_process_control(
    rc_process_control& pctrl) const
{
    p_pctrl_set[0].erase(&pctrl);
    p_pctrl_set[1].erase(&pctrl);
}

void reset_signal::update()
{
    base_type::update();
    pctrl_set& set_ = p_pctrl_set[(this->read() ? 1 : 0)];
    for (pctrl_set::iterator it = set_.begin();
        it != set_.end();
        ++it)
    {
        (*it)->deactivate();
    }
}

void reset_buffer::register_process_control(
    rc_process_control& pctrl, bool active_level) const
{
    p_pctrl_set[(active_level ? 1 : 0)].insert(&pctrl);
}

void reset_buffer::unregister_process_control(
    rc_process_control& pctrl) const
{
    p_pctrl_set[0].erase(&pctrl);
    p_pctrl_set[1].erase(&pctrl);
}

void reset_buffer::update()
{
    base_type::update();
    pctrl_set& set_ = p_pctrl_set[(this->read() ? 1 : 0)];
    for (pctrl_set::iterator it = set_.begin();
        it != set_.end();
        ++it)
    {
        (*it)->deactivate();
    }
}

} // namespace internals

} // namespace ReChannel
//
// $Id: rc_reset_channel_if.cpp,v 1.5 2007/10/09 21:47:01 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_reset_channel_if.cpp,v $
//
