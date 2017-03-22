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
 * @file    rc_signal_rv.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:33:51 $
 * $Revision: 1.9 $
 */

#ifndef RC_SIGNAL_RV_H_
#define RC_SIGNAL_RV_H_

#include "ReChannel/components/rc_signal.h"

namespace ReChannel {

/**
 * @brief Resettable representation of sc_signal_rv<W>.
 */
template<int W>
class rc_signal_rv
    : public rc_signal<sc_lv<W> >
{
private:
    typedef sc_dt::sc_logic_value_t data_type;
    typedef sc_dt::sc_lv<W>         value_type;
    enum logic_values {
        Log_0 = 0,
        Log_1,
        Log_Z,
        Log_X
    };
    struct written
    {
        written(const sc_process_handle& hproc_, const value_type& value_)
            : hproc(hproc_)
        {
            for (int i=0; i < W; ++i) {
                value[i] = sc_logic(value_[i]).value();
            }
        }
        sc_process_handle hproc;
        data_type         value[W];
    };

private:
    typedef rc_signal_rv         this_type;
    typedef rc_signal<sc_lv<W> > base_type;

    typedef typename std::vector<written> written_vector;

    using base_type::m_new_value;
    using base_type::m_reset_value;

public:
    explicit rc_signal_rv(
        const sc_module_name& name_=sc_gen_unique_name("signal_rv"));

    virtual const char* kind() const
        { return "rc_signal_rv"; }

    virtual void register_port(sc_port_base& port_, const char* if_name_)
        { }

    virtual void write(const value_type& value);

    this_type& operator=(const value_type& value)
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
        for (int i=0; i < W; ++i) {
            for (int j=0; j < 4; ++j) {
                m_value_count[i][j] = 0;
            }
        }
        if (m_reset_written_vector.empty()) {
            m_written_vector.clear();
        } else {
            m_written_vector = m_reset_written_vector;
            for (typename written_vector::iterator it =
                    m_written_vector.begin();
                it != m_written_vector.end();
                ++it)
            {
                data_type* written_value = (*it).value;
                for (int i=0; i < W; ++i) {
                    int value = sc_logic(written_value[i]).value();
                    if (value != Log_Z) {
                        m_value_count[i][value] += 1;
                    }
                }
            }
        }
    }

protected:

    written_vector m_reset_written_vector;
    written_vector m_written_vector;
    int            m_value_count[W][4];

private:
    // diabled
    rc_signal_rv(const this_type& signal_);
};

/* template code */

template<int W>
rc_signal_rv<W>::rc_signal_rv(const sc_module_name& name_)
    : base_type(name_)
{
    for (int i=0; i < W; ++i) {
        for (int j=0; j < 4; ++j) {
            m_value_count[i][j] = 0;
        }
    }
}

template<int W>
void rc_signal_rv<W>::write(const value_type& value)
{
    if (this->rc_is_active() || !sc_is_running()) {
        sc_process_handle hproc = sc_get_current_process_handle();
        written* found = NULL;

        for(typename written_vector::iterator it =
            m_written_vector.begin();
            it != m_written_vector.end();
            ++it)
        {
            if (hproc.get_process_object()
            == (*it).hproc.get_process_object())
            {
                found = &(*it);
                break;
            }
        }

        bool changed = false;

        if (found == NULL) {
            bool is_Z = true;
            for(int i=0; i < W; ++i) {
                if (value[i] != Log_Z) {
                    is_Z = false;
                    break;
                }
            }
            if (is_Z) {
                return;
            }
            m_written_vector.push_back(written(hproc, value));
            found = &m_written_vector.back();
            for (int i=0; i < W; ++i) {
                data_type value_ = sc_logic(value[i]).value();
                if (value_ != (data_type)Log_Z) {
                    ++m_value_count[i][value_];
                }
            }
            changed = true;
        } else {
            bool is_Z = true;
            for (int i=0; i < W; ++i) {
                data_type curr_value_ = sc_logic(found->value[i]).value();
                data_type value_ = sc_logic(value[i]).value();
                if (value_ != (data_type)Log_Z) {
                    is_Z = false;
                }
                if (value_ != curr_value_) {
                    changed = true;
                    --m_value_count[i][curr_value_];
                    if (value_ != (data_type)Log_Z) {
                        ++m_value_count[i][value_];
                        found->value[i] = value_;
                    }
                }
            }
            if (is_Z) {
                if (found != &m_written_vector.back()) {
                    *found = m_written_vector.back();
                }
                m_written_vector.pop_back();
            }
        }

        if (changed) {
            for (int i=0; i < W; ++i) {
                int* value_count = m_value_count[i];
                if (value_count[Log_X] > 0) {
                    m_new_value[i] = Log_X;
                } else if (value_count[Log_0] > 0) {
                    m_new_value[i] =
                        (value_count[Log_1] == 0
                            ? Log_0 : Log_X);
                } else if (value_count[Log_1] > 0) {
                    m_new_value[i] =
                        (value_count[Log_0] == 0
                            ? Log_1 : Log_X);
                } else {
                    m_new_value[i] = Log_Z;
                }
            }
            this->request_update();
        }
    }
}

} // namespace ReChannel

#endif // RC_SIGNAL_RV_H_

//
// $Id: rc_signal_rv.h,v 1.9 2007/12/20 20:33:51 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_signal_rv.h,v $
//
