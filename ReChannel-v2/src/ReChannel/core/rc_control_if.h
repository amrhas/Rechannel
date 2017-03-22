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
 * @file    rc_control_if.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:21:48 $
 * $Revision: 1.8 $
 */

#ifndef RC_CONTROL_IF_H_
#define RC_CONTROL_IF_H_

#include "ReChannel/core/rc_common_header.h"

namespace ReChannel {

class rc_control;
class rc_reconfigurable;
class rc_reconfigurable_set;
class rc_switch_connector_base;

/**
 * @brief ABI of a reconfiguration control used for simulation.
 */
class rc_control_sim_if
    : virtual public sc_interface
{
public:
    virtual bool has_control(
        const rc_reconfigurable_set& reconf_set) const = 0;

    virtual void load(const rc_reconfigurable_set& reconf_set) = 0;

    virtual void activate(const rc_reconfigurable_set& reconf_set) = 0;

    virtual void deactivate(const rc_reconfigurable_set& reconf_set) = 0;

    virtual void unload(const rc_reconfigurable_set& reconf_set) = 0;

    virtual void lock(const rc_reconfigurable_set& reconf_set) = 0;

    virtual bool trylock(const rc_reconfigurable_set& reconf_set) = 0;

    virtual void unlock(const rc_reconfigurable_set& reconf_set) = 0;

    virtual bool is_locked(
        const rc_reconfigurable_set& reconf_set) const = 0;

protected:
    virtual ~rc_control_sim_if() {}
};

/**
 * @brief ABI of a reconfiguration control used for elaboration.
 */
class rc_control_elab_if
    : virtual public sc_interface
{
public:
    virtual bool has_control(
        const rc_reconfigurable_set& reconf_set) const = 0;

    virtual void add(const rc_reconfigurable_set& reconf_set) = 0;

protected:
    virtual ~rc_control_elab_if() {}
};

/**
 * @brief ABI of the reconfiguration control.
 */
class rc_control_if
    : virtual public rc_control_sim_if,
      virtual public rc_control_elab_if
{
    virtual void move(
        const rc_reconfigurable_set& reconf_set,
        rc_switch_connector_base& target) = 0;

    virtual void remove(const rc_reconfigurable_set& reconf_set) = 0;
};

} // namespace ReChannel

#endif //RC_CONTROL_IF_H_

//
// $Id: rc_control_if.h,v 1.8 2007/11/23 13:21:48 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_control_if.h,v $
//
