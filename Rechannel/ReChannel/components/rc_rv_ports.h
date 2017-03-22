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
 * @file    rc_rv_ports.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.6 $
 */

#ifndef RC_RV_PORTS_H_
#define RC_RV_PORTS_H_

#include "ReChannel/core/rc_common_header.h"

namespace ReChannel {

/**
 * @brief Resettable representation of sc_in_rv<W>.
 */
template<int W>
class rc_in_rv
    : public ::sc_core::sc_in_rv<W>
{
private:
    typedef typename ::sc_core::sc_in_rv<W> base_type;

public:
    typedef sc_signal_in_if<sc_dt::sc_lv<W> > if_type;

public:
    rc_in_rv() { }

    explicit rc_in_rv(const char* name_)
        : base_type(name_)
        { }

    virtual void end_of_elaboration()
        { sc_in<sc_dt::sc_lv<W> >::end_of_elaboration(); }
};

/**
 * @brief Resettable representation of sc_inout_rv<W>.
 */
template<int W>
class rc_inout_rv
    : public ::sc_core::sc_inout_rv<W>
{
private:
    typedef rc_inout_rv<W>            this_type;
    typedef ::sc_core::sc_inout_rv<W> base_type;

public:
    typedef sc_signal_inout_if<sc_dt::sc_lv<W> > if_type;

public:
    rc_inout_rv() { }

    explicit rc_inout_rv(const char* name_)
        : base_type(name_)
        { }

    this_type& operator= (const sc_dt::sc_lv<W>& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const sc_signal_in_if<sc_dt::sc_lv<W> >& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const sc_port<sc_signal_in_if<sc_dt::sc_lv<W> >, 1>& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const sc_port<sc_signal_inout_if<sc_dt::sc_lv<W> >, 1>& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const rc_inout_rv<W>& v)
        { base_type::operator=(v); return *this; }

    virtual void end_of_elaboration()
        { sc_inout<sc_dt::sc_lv<W> >::end_of_elaboration(); }
};

/**
 * @brief Resettable representation of sc_out_rv<W>.
 */
template<int W>
class rc_out_rv
    : public ::sc_core::sc_out_rv<W>
{
private:
    typedef rc_out_rv<W>            this_type;
    typedef ::sc_core::sc_out_rv<W> base_type;

public:
    typedef sc_signal_out_if<sc_dt::sc_lv<W> > if_type;

public:
    rc_out_rv() { }

    explicit rc_out_rv(const char* name_)
        : base_type(name_)
        { }

    this_type& operator= (const sc_dt::sc_lv<W>& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const sc_signal_in_if<sc_dt::sc_lv<W> >& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const sc_port<sc_signal_in_if<sc_dt::sc_lv<W> >, 1>& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const sc_port<sc_signal_inout_if<sc_dt::sc_lv<W> >, 1>& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const rc_inout_rv<W>& v)
        { base_type::operator=(v); return *this; }

    virtual void end_of_elaboration()
        { sc_inout<sc_dt::sc_lv<W> >::end_of_elaboration(); }
};

} // namespace ReChannel

#endif // RC_RV_PORTS_H_

//
// $Id: rc_rv_ports.h,v 1.6 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_rv_ports.h,v $
//

