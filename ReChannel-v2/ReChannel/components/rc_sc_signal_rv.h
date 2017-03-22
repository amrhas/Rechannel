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
 * @file    rc_sc_signal_rv.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.4 $
 */

#ifndef RC_SC_SIGNAL_RV_H_
#define RC_SC_SIGNAL_RV_H_

#include "ReChannel/components/rc_sc_signal.h"

namespace ReChannel {

/**
 * @brief Resettable rv signal (wrapping class sc_signal_rv<W>).
 */
template<int W>
class rc_sc_signal_rv
    : public rc_sc_signal_b<sc_lv<W>, sc_signal_rv<W> >
{
private:
    typedef rc_sc_signal_b<sc_lv<W>, sc_signal_rv<W> > base_type;

public:
    explicit rc_sc_signal_rv(
        const sc_module_name& name_=sc_gen_unique_name("signal_rv"))
        : base_type(name_)
    { }

    operator const sc_lv<W>&() const
        { return this->read(); }

    rc_sc_signal_rv<W>& operator= (const sc_lv<W>& value)
        { this->write(value); return *this; }

    rc_sc_signal_rv<W>& operator= (const rc_sc_signal_rv<W>& signal_)
        { this->write(signal_); return *this; }

protected:

    RC_ON_RESET()
    {
        int count = this->get_driver_count();
        for (int i=0; i < count; ++i) {
            driver_write(s_ZZZ, i);
        }
    }

private:
    static const sc_dt::sc_lv<W> s_ZZZ;
};

template<int W>
const typename sc_dt::sc_lv<W>
rc_sc_signal_rv<W>::s_ZZZ = sc_dt::sc_lv<W>('Z');

} // namespace ReChannel

#endif // RC_SC_SIGNAL_RV_H_

//
// $Id: rc_sc_signal_rv.h,v 1.4 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_sc_signal_rv.h,v $
//
