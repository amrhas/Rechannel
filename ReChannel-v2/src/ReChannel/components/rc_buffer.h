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
 * @file    rc_buffer.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:22:57 $
 * $Revision: 1.8 $
 */

#ifndef RC_BUFFER_H_
#define RC_BUFFER_H_

#include "ReChannel/components/rc_signal.h"

namespace ReChannel {

/**
 * @brief Resettable representation of sc_buffer.
 */
template<class T>
class rc_buffer
    : public rc_signal<T>
{
private:
    typedef rc_buffer<T> this_type;
    typedef rc_signal<T> base_type;

    using base_type::m_current_value;
    using base_type::m_new_value;
    using base_type::m_notify_value_changed_event;
    using base_type::m_value_changed_event;
    using base_type::m_delta;
    using base_type::m_driver_proc;

public:
    explicit rc_buffer(
        const sc_module_name& name_=sc_gen_unique_name("buffer"))
        : base_type(name_)
    { }

    virtual const char* kind() const
        { return "rc_buffer"; }

    virtual void write(const T& value);

    operator const T&() const
        { return this->read(); }

    this_type& operator=(const T& value)
        { this->write(value); return *this; }

    this_type& operator=(const this_type& buffer_)
        { this->write(buffer_); return *this; }

protected:

    virtual void update();
};

/* template code */

template<class T>
void rc_buffer<T>::write(const T& value)
{
    if (this->rc_is_active() || !sc_is_running())
    {
        if (this->rc_is_constr_done()) {
            sc_object* const driver_proc =
                sc_get_current_process_handle().get_process_object();
            if (m_driver_proc == NULL) {
                m_driver_proc = driver_proc;
            } else if (m_driver_proc != driver_proc && driver_proc != NULL) {
                internals::signals::report_driver_conflict(
                    *this, *m_driver_proc, *driver_proc);
            }
        }
        m_new_value = value;
        this->request_update();
    }
}

template<class T>
void rc_buffer<T>::update()
{
    if (this->rc_is_active()) {
        m_current_value = m_new_value;
        if (m_notify_value_changed_event) {
            m_value_changed_event.notify(SC_ZERO_TIME);
        }
        m_delta = sc_delta_count();
    }
}

} // namespace ReChannel

#endif // RC_BUFFER_H_

//
// $Id: rc_buffer.h,v 1.8 2007/12/20 20:22:57 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_buffer.h,v $
//
