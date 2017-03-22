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
 * @file    rc_resolved_ports.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.5 $
 */

#ifndef RC_RESOLVED_PORTS_H_
#define RC_RESOLVED_PORTS_H_

#include "ReChannel/core/rc_common_header.h"

namespace ReChannel {

/**
 * @brief ReChannel's representation of sc_in_resolved.
 */
class rc_in_resolved
    : public ::sc_core::sc_in_resolved
{
private:
    typedef ::sc_core::sc_in_resolved base_type;

public:
    typedef sc_signal_in_if<sc_dt::sc_logic> if_type;

public:
    rc_in_resolved() { }
    explicit rc_in_resolved(const char* name_)
    : base_type(name_)
    {}

    virtual void end_of_elaboration()
        { sc_in<sc_dt::sc_logic>::end_of_elaboration(); }
};

/**
 * @brief ReChannel's representation of sc_inout_resolved.
 */
class rc_inout_resolved
    : public ::sc_core::sc_inout_resolved
{
private:
    typedef rc_inout_resolved            this_type;
    typedef ::sc_core::sc_inout_resolved base_type;

public:
    typedef sc_signal_inout_if<sc_dt::sc_logic> if_type;

public:
    rc_inout_resolved() { }
    explicit rc_inout_resolved(const char* name_)
    : base_type(name_)
    {}

    this_type& operator= (const sc_dt::sc_logic& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const sc_signal_in_if<sc_dt::sc_logic>& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const sc_port<sc_signal_in_if<sc_dt::sc_logic>, 1>& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const sc_port<sc_signal_inout_if<sc_dt::sc_logic>, 1>& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const rc_inout_resolved& v)
        { base_type::operator=(v); return *this; }

    virtual void end_of_elaboration()
        { sc_inout<sc_dt::sc_logic>::end_of_elaboration(); }
};

/**
 * @brief ReChannel's representation of sc_out_resolved.
 */
class rc_out_resolved
    : public rc_inout_resolved
{
private:
    typedef rc_out_resolved   this_type;
    typedef rc_inout_resolved base_type;

public:
    typedef sc_signal_out_if<sc_dt::sc_logic> if_type;

public:
    rc_out_resolved() { }
    explicit rc_out_resolved(const char* name_)
    : base_type(name_)
    {}

    this_type& operator= (const sc_dt::sc_logic& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const sc_signal_in_if<sc_dt::sc_logic>& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const sc_port<sc_signal_in_if<sc_dt::sc_logic>, 1>& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const sc_port<sc_signal_inout_if<sc_dt::sc_logic>, 1>& v)
        { base_type::operator=(v); return *this; }
    this_type& operator= (const rc_inout_resolved& v)
        { base_type::operator=(v); return *this; }
};

} // namespace ReChannel

#endif // RC_RESOLVED_PORTS_H_

//
// $Id: rc_resolved_ports.h,v 1.5 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_resolved_ports.h,v $
//

