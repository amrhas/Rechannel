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
 * @file    rc_sc_buffer.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.6 $
 */

#ifndef RC_SC_BUFFER_H_
#define RC_SC_BUFFER_H_

#include "ReChannel/components/rc_sc_signal.h"

namespace ReChannel {

/**
 * @brief Resettable buffer (wrapping class sc_buffer).
 */
template<class T>
class rc_sc_buffer
    : public rc_sc_signal_b<T, sc_buffer<T> >
{
private:
    typedef rc_sc_signal_b<T, sc_buffer<T> > base_type;

public:
    explicit rc_sc_buffer(
        const sc_module_name& name_=sc_gen_unique_name("buffer"))
        : base_type(name_)
    { }

    operator const T&() const
        { return this->read(); }

    rc_sc_buffer<T>& operator= (const T& value)
        { this->write(value); return *this; }

    rc_sc_buffer<T>& operator= (const rc_sc_buffer<T>& buffer_)
        { this->write(buffer_); return *this; }
};

/**
 * @brief Resettable buffer (wrapping class sc_buffer<bool>).
 */
template<>
class rc_sc_buffer<bool>
    : public rc_sc_signal_b<bool, internals::reset_buffer>,
      virtual public rc_reset_channel_if
{
private:
    typedef rc_sc_signal_b<bool, internals::reset_buffer>
        base_type;

public:
    explicit rc_sc_buffer(
        const sc_module_name& name_=sc_gen_unique_name("buffer"))
        : base_type(name_)
    { }

/* interface methods */

    virtual bool posedge() const
        { return get_if().posedge(); }

    virtual bool negedge() const
        { return get_if().negedge(); }

    virtual const sc_event& posedge_event() const
        { return get_exportal_if().posedge_event(); }

    virtual const sc_event& negedge_event() const
        { return get_exportal_if().negedge_event(); }

#if !defined(RC_USE_NON_OSCI_KERNEL)
    virtual sc_reset* is_reset() const
        { return m_signal.is_reset(); }
#endif // !defined(RC_USE_NON_OSCI_KERNEL)

protected:
    virtual void register_process_control(
        rc_process_control& pctrl, bool reset_level) const
    {
        m_signal.register_process_control(pctrl, reset_level);
    }

    virtual void unregister_process_control(
        rc_process_control& pctrl) const
    {
        m_signal.unregister_process_control(pctrl);
    }

    virtual bool get_current_level() const
        { return m_signal.read(); }

    virtual const sc_signal<bool>* get_underlying_reset_signal() const
        { return &m_signal; }

/* other channel methods */

public:
    operator const bool&() const
        { return this->read(); }

    rc_sc_buffer<bool>& operator= (const bool& value)
        { this->write(value); return *this; }

    rc_sc_buffer<bool>& operator= (const rc_sc_buffer<bool>& buffer_)
        { this->write(buffer_); return *this; }
};

/**
 * @brief Resettable buffer (wrapping class sc_buffer<sc_logic>).
 */
template<>
class rc_sc_buffer<sc_logic>
    : public rc_sc_signal_b<sc_logic, sc_buffer<sc_logic> >
{
private:
    typedef rc_sc_signal_b<sc_logic, sc_buffer<sc_logic> > base_type;

public:
    explicit rc_sc_buffer(
        const sc_module_name& name_=sc_gen_unique_name("buffer"))
        : base_type(name_)
    { }

/* interface methods */

    virtual bool posedge() const
        { return get_if().posedge(); }

    virtual bool negedge() const
        { return get_if().negedge(); }

    virtual const sc_event& posedge_event() const
        { return get_exportal_if().posedge_event(); }

    virtual const sc_event& negedge_event() const
        { return get_exportal_if().negedge_event(); }

/* other channel methods */

    operator const sc_logic&() const
        { return this->read(); }

    rc_sc_buffer<sc_logic>& operator= (const sc_logic& value)
        { this->write(value); return *this; }

    rc_sc_buffer<sc_logic>& operator= (const rc_sc_buffer<sc_logic>& buffer_)
        { this->write(buffer_); return *this; }
};

} // namespace ReChannel

#endif // RC_SC_BUFFER_H_

//
// $Id: rc_sc_buffer.h,v 1.6 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_sc_buffer.h,v $
//
