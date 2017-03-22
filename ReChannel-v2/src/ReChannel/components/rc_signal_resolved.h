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
 * @file    rc_signal_resolved.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.7 $
 */

#ifndef RC_SIGNAL_RESOLVED_H_
#define RC_SIGNAL_RESOLVED_H_

#include "ReChannel/components/rc_signal.h"

namespace ReChannel {

/**
 * @brief Resettable representation of sc_signal_resolved.
 */
class rc_signal_resolved
    : public rc_signal<sc_logic>
{
private:
    typedef sc_dt::sc_logic_value_t data_type;
    enum logic_values {
        Log_0 = 0,
        Log_1,
        Log_Z,
        Log_X
    };
    struct written
    {
        written(const sc_process_handle& hproc_, data_type value_)
            : hproc(hproc_), value(value_)
            { }
        sc_process_handle hproc;
        data_type         value;
    };

private:
    typedef rc_signal_resolved  this_type;
    typedef rc_signal<sc_logic> base_type;

    typedef std::vector<written> written_vector;

    using base_type::m_new_value;

public:
    explicit rc_signal_resolved(
        const sc_module_name& name_=sc_gen_unique_name("signal_resolved"));

    virtual const char* kind() const
        { return "rc_signal_resolved"; }

    virtual void register_port(sc_port_base& port_, const char* if_name_)
        { }

    virtual void write(const sc_logic& value);

    operator const sc_logic&() const
        { return this->read(); }

    this_type& operator=(const sc_logic& value)
        { this->write(value); return *this; }

    this_type& operator=(const this_type& signal_)
        { this->write(signal_); return *this; }

protected:

    RC_ON_INIT_RESETTABLE()
    {
        base_type::rc_on_init_resettable();
        if (!m_written_vector.empty()) {
            m_reset_written_vector = m_written_vector;
        }
    }

    RC_ON_RESET()
    {
        base_type::rc_on_reset();
        for (int i=0; i < 4; ++i) {
            m_value_count[i] = 0;
        }
        if (m_reset_written_vector.empty()) {
            m_written_vector.clear();
        } else {
            m_written_vector = m_reset_written_vector;
            for (written_vector::iterator it = m_written_vector.begin();
                it != m_written_vector.end();
                ++it)
            {
                int value = (*it).value;
                if (value != Log_Z) {
                    m_value_count[value] += 1;
                }
            }
        }   
    }

protected:

    written_vector m_reset_written_vector;
    written_vector m_written_vector;
    int            m_value_count[4];

private:
    // diabled
    rc_signal_resolved(const this_type& signal_);
};

} // namespace ReChannel

#endif // RC_SIGNAL_RESOLVED_H_

//
// $Id: rc_signal_resolved.h,v 1.7 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_signal_resolved.h,v $
//
