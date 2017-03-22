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
 * @file    rc_module.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:24:55 $
 * $Revision: 1.20 $
 */

#ifndef RC_MODULE_H_
#define RC_MODULE_H_

#include "ReChannel/core/rc_process_support.h"

namespace ReChannel {

struct rc_module;

typedef sc_module_name rc_module_name;
typedef rc_module      rc_channel;
typedef rc_module      rc_behavior;

/**
 * @brief ReChannel's representation of sc_module.
 */
struct rc_module
    : public sc_module,
      virtual public rc_resettable
{
protected:
    rc_module()
        { rc_register_resettable(*this, this); }

    rc_module(rc_module_name name_)
        { rc_register_resettable(*this, this); }

protected:

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

    /* wait(..) redirects */

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

    virtual void rc_on_reset() {}

    virtual void rc_on_init_resettable() {}

    virtual ~rc_module() {}
};

} // namespace ReChannel

#define RC_MODULE(user_module_name) \
    struct user_module_name \
        : public ::ReChannel::rc_module

#define RC_CTOR(user_module_name) \
    RC_HAS_PROCESS(user_module_name); \
    user_module_name(sc_module_name)

#endif //RC_MODULE_H_

//
// $Id: rc_module.h,v 1.20 2007/11/23 13:24:55 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_module.h,v $
//
