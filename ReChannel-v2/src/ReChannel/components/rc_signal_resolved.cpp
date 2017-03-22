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
 * @file    rc_signal_resolved.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:33:51 $
 * $Revision: 1.6 $
 */

#include "rc_signal_resolved.h"

namespace ReChannel {

rc_signal_resolved::rc_signal_resolved(const sc_module_name& name_)
    : base_type(name_)
{
    for (int i=0; i < 4; ++i) {
        m_value_count[i] = 0;
    }
}

void rc_signal_resolved::write(const sc_logic& value)
{
    if (this->rc_is_active() || !sc_is_running()) {
        sc_process_handle hproc = sc_get_current_process_handle();

        data_type value_ = value.value();

        bool found = false;

        for(written_vector::iterator it = m_written_vector.begin();
            it != m_written_vector.end();
            ++it)
        {
            written& w = *it;
            if (hproc.get_process_object()
            == w.hproc.get_process_object())
            {
                data_type& w_value_ = w.value;
                if (value_ != w_value_) {
                    --m_value_count[w_value_];
                    if (value_ != (data_type)Log_Z) {
                        ++m_value_count[value_];
                        w_value_ = value_;
                    } else {
                        if (&w != &m_written_vector.back()) {
                            w = m_written_vector.back();
                        }
                        m_written_vector.pop_back();
                    }
                    found = true;
                    break;
                }
                return;
            }
        }

        if (!found && value_ != (data_type)Log_Z) {
            m_written_vector.push_back(written(hproc, value_));
            ++m_value_count[value_];
        }

        if (m_value_count[Log_X] > 0) {
            m_new_value = Log_X;
        } else if (m_value_count[Log_0] > 0) {
            m_new_value =
                (m_value_count[Log_1] == 0
                    ? Log_0 : Log_X);
        } else if (m_value_count[Log_1] > 0) {
            m_new_value =
                (m_value_count[Log_0] == 0
                    ? Log_1 : Log_X);
        } else {
            m_new_value = Log_Z;
        }
        request_update();
    }
}

} // namespace ReChannel

//
// $Id: rc_signal_resolved.cpp,v 1.6 2007/12/20 20:33:51 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_signal_resolved.cpp,v $
//
