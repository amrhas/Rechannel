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
 * @file    rc_control.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:28:34 $
 * $Revision: 1.15 $
 */

#ifndef RC_CONTROL_H_
#define RC_CONTROL_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_control_if.h"
#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/core/rc_reconfigurable_set.h"
#include "ReChannel/core/rc_switch_connector.h"
#include "ReChannel/core/rc_report_ids.h"
#include "ReChannel/util/rc_mutex_object.h"

namespace ReChannel {

/**
 * @brief This class represents ReChannel's reconfiguration control.
 */
class rc_control
    : public sc_channel,
      virtual public rc_control_if
{
private:
    typedef std::map<sc_object*, rc_reconfigurable*> obj_reconf_map;

public:
    rc_control(sc_module_name name_=sc_gen_unique_name("rc_control"))
    { }

    virtual bool has_control(
        const rc_reconfigurable_set& reconf_set) const;

    virtual void add(const rc_reconfigurable_set& reconf_set);

    virtual void remove(const rc_reconfigurable_set& reconf_set);

    virtual void load(const rc_reconfigurable_set& reconf_set);

    virtual void unload(const rc_reconfigurable_set& reconf_set);

    virtual void activate(const rc_reconfigurable_set& reconf_set);

    virtual void deactivate(const rc_reconfigurable_set& reconf_set);

    virtual void lock(const rc_reconfigurable_set& reconf_set);

    virtual bool trylock(const rc_reconfigurable_set& reconf_set);

    virtual void unlock(const rc_reconfigurable_set& reconf_set);

    virtual bool is_locked(const rc_reconfigurable_set& reconf_set) const;

    virtual void move(
        const rc_reconfigurable_set& reconf_set,
        rc_switch_connector_base& target);

protected:

    virtual sc_time takes_time(
        const rc_reconfigurable& reconf,
        rc_reconfigurable::action_type action) const;

    void update_delays(rc_reconfigurable::action_type action);

    void get_controlled_set(rc_reconfigurable_set& copy_dest) const;

    virtual void start_of_simulation();

private:
    /* for internal use only */

    /**
     * @brief Unlocks just a range of a reconfigurable set (internal only)
     */
    void _rc_unlock(
        rc_reconfigurable_set::const_iterator start_,
        rc_reconfigurable_set::const_iterator end_) const;

    void _rc_do_action(
        const rc_reconfigurable_set& reconf_set,
        rc_reconfigurable::state_type new_state);

    void _rc_do_action(
        const rc_reconfigurable_set& reconf_set,
        rc_reconfigurable::state_type new_state,
        rc_reconfigurable::state_type only_change_state);

    void _rc_reconfigure(
        const rc_reconfigurable_set& reconf_set,
        rc_reconfigurable::state_type new_state);

private:
    rc_reconfigurable_set p_reconfigurable_set;
    obj_reconf_map        p_obj_reconf_map;

private:
    // disabled
    rc_control(const rc_control& orig);
    rc_control& operator=(const rc_control& orig);
};

} // namespace ReChannel

#endif //RC_CONTROL_H_

//
// $Id: rc_control.h,v 1.15 2007/12/20 20:28:34 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_control.h,v $
//
