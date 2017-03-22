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
 * @file    rc_prim_channel.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:33:51 $
 * $Revision: 1.8 $
 */

#ifndef RC_PRIM_CHANNEL_H_
#define RC_PRIM_CHANNEL_H_

#include "ReChannel/core/rc_reconfigurable.h"

namespace ReChannel {

/**
 * @brief Resettable representation of sc_prim_channel.
 */
class rc_prim_channel
    : public sc_prim_channel,
      virtual public rc_resettable
{
public:

    rc_prim_channel()
        : sc_prim_channel(), p_constr_done(false)
    {
        p_reconf =
            rc_register_resettable(*this, this->get_parent_object());
    }

    explicit rc_prim_channel(const char* name_)
        : sc_prim_channel(name_), p_constr_done(false)
    {
        p_reconf =
            rc_register_resettable(*this, this->get_parent_object());
    }

protected:

    inline bool rc_has_reconfigurable() const
        { return (p_reconf != NULL); }

    inline const rc_reconfigurable* rc_get_reconfigurable() const
        { return p_reconf; }

    inline bool rc_is_constr_done() const
        { return p_constr_done; }

    inline rc_reconfigurable::state_type rc_get_state() const
    {
        return (p_reconf != NULL ?
            p_reconf->rc_get_state() : rc_reconfigurable::ACTIVE);
    }

    inline rc_reconfigurable::state_type rc_get_next_state() const
    {
        return (p_reconf != NULL ?
            p_reconf->rc_get_next_state() : rc_reconfigurable::ACTIVE);
    }

    inline bool rc_is_loaded() const
        { return (this->rc_get_state() != rc_reconfigurable::UNLOADED); }

    inline bool rc_is_active() const
        { return (this->rc_get_state() == rc_reconfigurable::ACTIVE); }

    /* next_trigger(..) redirects */

    inline void next_trigger()
        { rc_next_trigger(); }

    inline void next_trigger(const sc_event& e)
        { rc_next_trigger(e); }

    inline void next_trigger(sc_event_or_list& el)
        { rc_next_trigger(el); }

    inline void next_trigger(sc_event_and_list& el)
        { rc_next_trigger(el); }

    inline void next_trigger(const sc_time& t)
        { rc_next_trigger(t); }

    inline void next_trigger(double v, sc_time_unit tu)
        { rc_next_trigger(v, tu); }

    inline void next_trigger(const sc_time& t, const sc_event& e)
        { rc_next_trigger(t, e); }

    inline void next_trigger(double v, sc_time_unit tu, const sc_event& e)
        { rc_next_trigger(v, tu, e); }

    inline void next_trigger(const sc_time& t, sc_event_or_list& el)
        { rc_next_trigger(t, el); }

    inline void next_trigger(
        double v, sc_time_unit t, sc_event_or_list& el)
        { rc_next_trigger(v, t, el); }

    inline void next_trigger(const sc_time& t, sc_event_and_list& el)
        { rc_next_trigger(t, el); }

    inline void next_trigger(
        double v, sc_time_unit tu, sc_event_and_list& el)
        { rc_next_trigger(v, tu, el); }

/* rc_wait() redirects */

    inline void wait()
        { rc_wait(); }

    inline void wait(int n)
        { rc_wait(n); }

    inline void wait(const sc_event& e)
        { rc_wait(e); }

    inline void wait(sc_event_or_list& el)
        { rc_wait(el); }

    inline void wait(sc_event_and_list& el)
        { rc_wait(el); }

    inline void wait(const sc_time& t)
        { rc_wait(t); }

    inline void wait(double v, sc_time_unit tu)
        { rc_wait(v, tu); }

    inline void wait(const sc_time& t, const sc_event& e)
        { rc_wait(t, e); }

    inline void wait(double v, sc_time_unit tu, const sc_event& e)
        { rc_wait(v, tu, e); }

    inline void wait(const sc_time& t, sc_event_or_list& el)
        { rc_wait(t, el); }

    inline void wait(double v, sc_time_unit t, sc_event_or_list& el)
        { rc_wait(v, t, el); }

    inline void wait(const sc_time& t, sc_event_and_list& el)
        { rc_wait(t, el); }

    inline void wait(double v, sc_time_unit tu, sc_event_and_list& el)
        { rc_wait(v, tu, el); }

    virtual void start_of_simulation()
    {
        p_constr_done = true;
    }

    RC_ON_INIT_RESETTABLE() {}

    RC_ON_RESET() {}

private:
    const rc_reconfigurable* p_reconf;
    bool                     p_constr_done;

private:
    // disabled
    rc_prim_channel(const rc_prim_channel& other);
    rc_prim_channel& operator=(const rc_prim_channel& other);
};

} // namespace ReChannel

#endif //RC_PRIM_CHANNEL_H_

//
// $Id: rc_prim_channel.h,v 1.8 2007/12/20 20:33:51 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_prim_channel.h,v $
//
