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
 * @file    rc_signal.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:33:51 $
 * $Revision: 1.11 $
 */

#ifndef RC_SIGNAL_H_
#define RC_SIGNAL_H_

#include "ReChannel/components/rc_prim_channel.h"
#include "ReChannel/components/rc_report_ids.h"
#include "ReChannel/core/rc_reset_channel_if.h"
#include "ReChannel/communication/rc_fallback_interface.h"

namespace ReChannel {

namespace internals {
namespace signals {

void report_binding_error(
    const sc_object& target, const sc_object& port1,
    const sc_object& port2);

void report_driver_conflict(
    const sc_object& target, const sc_object& driver1,
    const sc_object& driver2);

} // namespace internals
} // namespace signals

/**
 * @brief Base class of a resettable representation of sc_signal.
 */
template<class T>
class rc_signal_b
    : public sc_signal_inout_if<T>,
      public rc_prim_channel
{
protected:

/* constructor */

    explicit rc_signal_b(const char* name_)
        : rc_prim_channel(name_),
          m_current_value(rc_undefined_value<T>()),
          m_delta(~0), m_new_value(m_current_value),
          m_reset_value(m_current_value),
          m_notify_value_changed_event(false),
          m_notify_posedge_event(false), m_notify_negedge_event(false),
          m_output_port(0), m_driver_proc(0)
        { }

public:

/* interface methods */

    virtual const char* kind() const
        { return "rc_signal"; }

    virtual void register_port(sc_port_base& port_, const char* if_name_);

    virtual const sc_event& default_event() const
    { m_notify_value_changed_event = true; return m_value_changed_event; }

    virtual const sc_event& value_changed_event() const
    { m_notify_value_changed_event = true; return m_value_changed_event; }

    virtual const T& read() const
        { return m_current_value; }

    virtual const T& get_data_ref() const
        { return m_current_value; }

    virtual bool event() const
        { return (m_delta == sc_delta_count()); }

    virtual void write(const T& value);

    virtual void print(std::ostream& os=std::cout) const
        { os << m_current_value; }

    virtual void dump(std::ostream& os=std::cout) const;

protected:

    virtual void update();

    RC_ON_INIT_RESETTABLE()
    {
        // store signal's initial value
        m_reset_value = m_new_value;
    }

    RC_ON_RESET()
    {
        // reset the signal
        m_delta = ~0;
        m_new_value = m_reset_value;
        m_current_value = m_reset_value;
        if (m_notify_value_changed_event) {
            m_value_changed_event.cancel();
        }
    }

protected:
    T             m_current_value;
    sc_dt::uint64 m_delta;
    T             m_new_value;
    T             m_reset_value;

    mutable bool  m_notify_value_changed_event : 1;
    mutable bool  m_notify_posedge_event       : 1;
    mutable bool  m_notify_negedge_event       : 1;
    sc_event      m_value_changed_event;

    sc_port_base* m_output_port;
    sc_object*    m_driver_proc;

private:
    // disabled
    rc_signal_b(const rc_signal_b<T>& other);
    rc_signal_b<T>& operator=(const rc_signal_b<T>& value);
};

/**
 * @brief Resettable representation of sc_signal.
 */
template<class T>
class rc_signal
    : public rc_signal_b<T>
{
private:
    typedef rc_signal<T>   this_type;
    typedef rc_signal_b<T> base_type;

public:
    explicit rc_signal(
        const sc_module_name& name_=sc_gen_unique_name("signal"))
        : base_type(name_)
        { }

    operator const T&() const
        { return this->read(); }

    this_type& operator=(const T& value)
        { this->write(value); return *this; }

    this_type& operator=(const this_type& signal_)
        { this->write(signal_.read()); return *this; }
};

/**
 * @brief Resettable representation of sc_signal<bool>.
 */
template<>
class rc_signal<bool>
    : public rc_signal_b<bool>,
      virtual public rc_reset_channel_if
{
private:
    typedef rc_signal<bool>   this_type;
    typedef rc_signal_b<bool> base_type;

    typedef std::set<rc_process_control*> pctrl_set;

public:
    explicit rc_signal(
        const sc_module_name& name_=sc_gen_unique_name("signal"))
        : base_type(name_), p_reset_signal(NULL)
        { }

/* interface methods */

    virtual const sc_event& posedge_event() const
        { m_notify_posedge_event = true; return m_posedge_event; }

    virtual const sc_event& negedge_event() const
        { m_notify_negedge_event = true; return m_negedge_event; }

    virtual bool posedge() const
        { return (this->event() && m_current_value); }

    virtual bool negedge() const
        { return (this->event() && !m_current_value); }

#if !defined(RC_USE_NON_OSCI_KERNEL)
    // workaround to allow reset of CTHREADs with the OSCI Kernel
    virtual sc_reset* is_reset() const
    {
        return _rc_get_reset_signal().is_reset();
    }
#endif // !defined(RC_USE_NON_OSCI_KERNEL)

/* other methods */

    operator const bool&() const
        { return this->read(); }

    this_type& operator=(const bool& value)
        { this->write(value); return *this; }

    this_type& operator=(const this_type& signal_)
        { this->write(signal_.read()); return *this; }

protected:

    virtual void register_process_control(
        rc_process_control& pctrl, bool active_level) const;

    virtual void unregister_process_control(
        rc_process_control& pctrl) const;

    virtual bool get_current_level() const;

    virtual const sc_signal<bool>* get_underlying_reset_signal() const;

    virtual void update();

    RC_ON_RESET()
    {
        base_type::rc_on_reset();

        // reset the signal
        if (m_notify_negedge_event) {
            m_negedge_event.cancel();
        }
        if (m_notify_posedge_event) {
            m_posedge_event.cancel();
        }
    }

private:
    sc_signal<bool>& _rc_get_reset_signal() const;

    void _rc_reset_updater_proc();

protected:
    sc_event m_posedge_event;
    sc_event m_negedge_event;

private:
    mutable pctrl_set        p_pctrl_set[2];
    mutable sc_signal<bool>* p_reset_signal;
};

/**
 * @brief Resettable representation of sc_signal<sc_logic>.
 */
template<>
class rc_signal<sc_dt::sc_logic>
    : public rc_signal_b<sc_dt::sc_logic>
{
private:
    typedef rc_signal<sc_dt::sc_logic>   this_type;
    typedef rc_signal_b<sc_dt::sc_logic> base_type;

public:
    explicit rc_signal(
        const sc_module_name& name_=sc_gen_unique_name("signal"))
        : base_type(name_)
        { }

/* interface methods */

    virtual const sc_event& posedge_event() const
        { m_notify_posedge_event = true; return m_posedge_event; }

    virtual const sc_event& negedge_event() const
        { m_notify_negedge_event = true; return m_negedge_event; }

    virtual bool posedge() const
    { return (this->event() && m_current_value == sc_dt::SC_LOGIC_1); }

    virtual bool negedge() const
    { return (this->event() && m_current_value == sc_dt::SC_LOGIC_0); }

/* other methods */

    operator const sc_dt::sc_logic&() const
        { return this->read(); }

    this_type& operator=(const sc_dt::sc_logic& value)
        { this->write(value); return *this; }

    this_type& operator=(const this_type& signal_)
        { this->write(signal_.read()); return *this; }

protected:

    virtual void update();

    RC_ON_RESET()
    {
        base_type::rc_on_reset();

        // reset the signal
        if (m_notify_negedge_event) {
            m_negedge_event.cancel();
        }
        if (m_notify_posedge_event) {
            m_posedge_event.cancel();
        }
    }

protected:
    sc_event m_posedge_event;
    sc_event m_negedge_event;
};

/* template code */

template<class T>
void rc_signal_b<T>::register_port(
    sc_port_base& port_, const char* if_name_)
{
    std::string if_name(if_name_);
    if (if_name == typeid(sc_signal_inout_if<T>).name()) {
        if (m_output_port != NULL) {
            internals::signals::report_binding_error(
                *this, *m_output_port, port_);
        }
        m_output_port = &port_;
    }
}

template<class T>
void rc_signal_b<T>::write(const T& value)
{
    if (this->rc_is_active() || !sc_is_running()) {
        if (this->rc_is_constr_done()) {
            sc_object* driver_proc =
                sc_get_current_process_handle().get_process_object();
            if (m_driver_proc == NULL) {
                m_driver_proc = driver_proc;
            } else if (m_driver_proc != driver_proc && driver_proc != NULL) {
                internals::signals::report_driver_conflict(
                    *this, *m_driver_proc, *driver_proc);
            }
        }

        m_new_value = value;
        if (!(m_new_value == m_current_value)) {
            this->request_update();
        }
    }
}

template<class T>
void rc_signal_b<T>::dump(std::ostream& os) const
{
    os << "     name = " << this->name() << std::endl;
    os << "    value = " << m_current_value << std::endl;
    os << "new value = " << m_new_value << std::endl;
}

template<class T>
void rc_signal_b<T>::update()
{
    if (this->rc_is_active() && !(m_new_value == m_current_value)) {
        m_current_value = m_new_value;
        if (m_notify_value_changed_event) {
            m_value_changed_event.notify(SC_ZERO_TIME);
        }
        m_delta = sc_delta_count();
    }
}

} // namespace ReChannel

#endif // RC_SIGNAL_H_

//
// $Id: rc_signal.h,v 1.11 2007/12/20 20:33:51 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_signal.h,v $
//
