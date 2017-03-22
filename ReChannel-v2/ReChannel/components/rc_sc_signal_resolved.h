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
 * @file    rc_sc_signal_resolved.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.4 $
 */

#ifndef RC_SC_SIGNAL_RESOLVED_H_
#define RC_SC_SIGNAL_RESOLVED_H_

#include "ReChannel/components/rc_sc_signal.h"

namespace ReChannel {

/**
 * @brief Resettable resolved signal (wrapping class sc_signal_resolved).
 */
class rc_sc_signal_resolved
    : public rc_sc_signal_b<sc_logic, sc_signal_resolved >
{
private:
    typedef rc_sc_signal_b<sc_logic, sc_signal_resolved > base_type;

public:
    explicit rc_sc_signal_resolved(
        const sc_module_name& name_=sc_gen_unique_name("signal_resolved"))
        : base_type(name_)
    { }

    operator const sc_logic&() const
        { return this->read(); }

    rc_sc_signal_resolved& operator=(const sc_logic& value)
        { this->write(value); return *this; }

    rc_sc_signal_resolved& operator=(const rc_sc_signal_resolved& signal_)
        { this->write(signal_); return *this; }

    virtual bool posedge() const
        { return get_if().posedge(); }

    virtual bool negedge() const
        { return get_if().negedge(); }

    virtual const sc_event& posedge_event() const
        { return get_exportal_if().posedge_event(); }

    virtual const sc_event& negedge_event() const
        { return get_exportal_if().negedge_event(); }

protected:

    RC_ON_RESET()
    {
        int count = this->get_driver_count();
        for (int i=0; i < count; ++i) {
            driver_write(SC_LOGIC_Z, i);
        }
    }
};

} // namespace ReChannel

#endif // RC_SC_SIGNAL_RESOLVED_H_

//
// $Id: rc_sc_signal_resolved.h,v 1.4 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_sc_signal_resolved.h,v $
//
