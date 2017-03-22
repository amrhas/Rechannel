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
 * @file    rc_sc_event.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.7 $
 */

#ifndef RC_SC_EVENT_H_
#define RC_SC_EVENT_H_

#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/core/rc_resettable.h"

namespace ReChannel {

/**
 * @brief Resettable event (wrapping class sc_event).
 */
class rc_sc_event
    : protected sc_prim_channel, virtual public rc_resettable
{
public:
    rc_sc_event()
        : sc_prim_channel(sc_gen_unique_name("_rc_sc_event"))
    {
        rc_register_resettable(*this, this->get_parent_object());
    }

    RC_ON_INIT_RESETTABLE() { }

    RC_ON_RESET()
        { p_event.cancel(); }

    inline void notify()
        { p_event.notify(); }

    inline void notify(const sc_time& t)
        { p_event.notify(t); }

    inline void notify(double t, sc_time_unit tu)
        { p_event.notify(t, tu); }

    inline void cancel()
        { p_event.cancel(); }

    inline sc_event_or_list& operator|(const sc_event& e) const
        { return p_event.operator|(e); }

    inline sc_event_and_list& operator&(const sc_event& e) const
        { return p_event.operator&(e); }

    operator sc_event&()
        { return p_event; }

    operator const sc_event&() const
        { return p_event; }

private:
    sc_event p_event;
};

} // namespace ReChannel

#endif //RC_SC_EVENT_H_

//
// $Id: rc_sc_event.h,v 1.7 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_sc_event.h,v $
//
