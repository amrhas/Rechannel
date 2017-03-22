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
 * @file    rc_fifo_accessors.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Defines accessors for the sc_fifo channel.
 *
 * This file contains the accessors for both FIFO read and write interfaces
 * as well as the implementation of the respective rc_fallback_interfaces.
 *
 * $Date: 2008/01/01 13:34:25 $
 * $Revision: 1.8 $
 */

#ifndef RC_FIFO_ACCESSORS_H_
#define RC_FIFO_ACCESSORS_H_

#include "ReChannel/communication/accessors/rc_accessor.h"

namespace ReChannel {

/**
 * @brief Fallback interface for a FIFO (input).
 */
template<class T>
class rc_fallback_interface<sc_fifo_in_if<T> >
: virtual public sc_fifo_in_if<T>
{
public:
    rc_fallback_interface()
    : p_value(rc_undefined_value<T>())
    { }
    virtual bool nb_read(T& value)
        { value = p_value; return false; }
    virtual const sc_event& data_written_event() const
    {
        SC_REPORT_WARNING(RC_ID_UNDEFINED_EVENT_WARNING_, 0);
        return p_undef;
    }
    virtual void read(T& value)
        { ::sc_core::wait(p_undef); }
    virtual T read()
    {
        SC_REPORT_WARNING(RC_ID_UNDEFINED_BLOCKING_ACCESS_WARNING_, 0);
        ::sc_core::wait(p_undef); return p_value;
    }
    virtual int num_available() const
        { return 0; }
private:
    const T  p_value;
    sc_event p_undef;
};

/**
 * @brief Fallback interface for a FIFO (output).
 */
template<class T>
class rc_fallback_interface<sc_fifo_out_if<T> >
: virtual public sc_fifo_out_if<T>
{
public:
    rc_fallback_interface() { }
    virtual bool nb_write(const T& value)
        { return false; }
    virtual const sc_event& data_read_event() const
    {
        SC_REPORT_WARNING(RC_ID_UNDEFINED_EVENT_WARNING_, 0);
        return p_undef;
    }
    virtual void write(const T& value)
    {
        SC_REPORT_WARNING(RC_ID_UNDEFINED_BLOCKING_ACCESS_WARNING_, 0);
        ::sc_core::wait(p_undef);
    }
    virtual int num_free() const
        { return 0; }
private:
    sc_event p_undef;
};

/**
 * @brief Accessor for a FIFO (input).
 */
template<class T>
RC_ACCESSOR_TEMPLATE(sc_fifo_in_if<T>)
{
    RC_ACCESSOR_TEMPLATE_CTOR(sc_fifo_in_if<T>) { }

    RC_EVENT(data_written_event);

    virtual bool nb_read(T& value)
    {
        return this->rc_nb_forward(&if_type::nb_read, rc_ref(value));
    }
    virtual void read(T& value)
    {
        // prevent deadlocks in empty FIFOs
        while (this->num_available() <= 0) {
            this->wait(this->data_written_event());
        }
        this->rc_forward(&if_type::read, rc_ref(value));
    }
    virtual T read()
    {
        // prevent deadlocks in empty FIFOs
        while (this->num_available() <= 0) {
            this->wait(this->data_written_event());
        }
        return this->rc_forward(&if_type::read);
    }
    virtual int num_available() const
    {
        return this->rc_nb_forward(&if_type::num_available);
    }
};

/**
 * @brief Accessor for a FIFO (output).
 */
template<class T>
RC_ACCESSOR_TEMPLATE(sc_fifo_out_if<T>)
{
    RC_ACCESSOR_TEMPLATE_CTOR(sc_fifo_out_if<T>) { }

    RC_EVENT(data_read_event);

    virtual bool nb_write(const T& value)
    {
        return this->rc_nb_forward(&if_type::nb_write, rc_cref(value));
    }
    virtual void write(const T& value)
    {
        // prevent deadlocks in empty FIFOs
        while (this->num_free() <= 0) {
            this->wait(this->data_read_event());
        }
        this->rc_forward(&if_type::write, rc_cref(value));
    }
    virtual int num_free() const
    {
        return this->rc_nb_forward(&if_type::num_free);
    }
};

} // namespace ReChannel

#endif // RC_FIFO_ACCESSORS_H_

//
// $Id: rc_fifo_accessors.h,v 1.8 2008/01/01 13:34:25 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/accessors/rc_fifo_accessors.h,v $
//
