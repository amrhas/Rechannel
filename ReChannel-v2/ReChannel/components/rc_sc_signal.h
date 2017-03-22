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
 * @file    rc_sc_signal.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.10 $
 */

#ifndef RC_SC_SIGNAL_H_
#define RC_SC_SIGNAL_H_

#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/core/rc_resettable.h"
#include "ReChannel/communication/exportals/rc_signal_exportals.h"
#include "ReChannel/core/rc_reset_channel_if.h"

namespace ReChannel {

/**
 * @brief Base class of a resettable signal (wrapping a SystemC signal).
 */
template<class T, class SIGNAL=sc_signal<T> >
class rc_sc_signal_b
    : public sc_channel,
      virtual public sc_signal_inout_if<T>,
      virtual public rc_resettable
{
protected:
    typedef sc_signal_inout_if<T> if_type;
    typedef SIGNAL                signal_type;

protected:

/* constructor */

    explicit rc_sc_signal_b(const sc_module_name& name_);

public:

/* interface methods */

    virtual void register_port(sc_port_base& port_, const char* if_name_)
        { return m_signal.register_port(port_, if_name_); }

    virtual const T& read() const
        { return get_if().read(); }

    virtual const T& get_data_ref() const
        { return get_if().get_data_ref(); }

    virtual void write(const T& value)
    {
        if (!p_constr_done) { m_reset_value = value; }
        get_if().write(value);
    }

    virtual const sc_event& default_event() const
        { return get_exportal_if().default_event(); }

    virtual const sc_event& value_changed_event() const
        { return get_exportal_if().value_changed_event(); }

/* other channel methods */

    virtual bool event() const
        { return m_signal.event(); }

    virtual void print(std::ostream& sout=std::cout) const
        { m_signal.print(sout); }

    virtual void dump(std::ostream& sout=std::cout) const
        { m_signal.dump(sout); }

    virtual const char* kind() const
        { return m_signal.kind(); }

protected:

    virtual void end_of_elaboration()
    {
        //p_if = &((if_type&)p_exportal.static_export());
    }

    virtual void start_of_simulation()
    {
        p_constr_done = true;
    }

    RC_ON_RESET()
    {
        driver_write(m_reset_value);
    }

    RC_ON_INIT_RESETTABLE() { }

protected:

    inline if_type& get_if();

    inline if_type& get_exportal_if() const
        { return *p_if; }

    inline const if_type& get_if() const;

    inline void driver_write(const T& value, int index=0);

    inline int get_driver_count() const
        { return p_interface_wrapper->get_nb_driver_count(); }

protected:
    signal_type                    m_signal;
    T                              m_reset_value;

private:
    rc_exportal<if_type>           p_exportal;
    rc_interface_wrapper<if_type>* p_interface_wrapper;
    const rc_reconfigurable*       p_reconf;
    if_type*                       p_if;
    bool                           p_constr_done;
};

/**
 * @brief Resettable signal (wrapping class sc_signal).
 */
template<class T>
class rc_sc_signal
    : public rc_sc_signal_b<T>
{
private:
    typedef rc_sc_signal_b<T> base_type;

public:
    explicit rc_sc_signal(
        const sc_module_name& name_=sc_gen_unique_name("signal"))
        : base_type(name_)
    { }

    operator const T&() const
        { return this->read(); }

    rc_sc_signal<T>& operator= (const T& value)
        { this->write(value); return *this; }

    rc_sc_signal<T>& operator= (const rc_sc_signal<T>& signal_)
        { this->write(signal_); return *this; }
};

/**
 * @brief Resettable signal (wrapping class sc_signal<bool>).
 */
template<>
class rc_sc_signal<bool>
    : public rc_sc_signal_b<bool, internals::reset_signal>,
      virtual public rc_reset_channel_if
{
private:
    typedef rc_sc_signal_b<bool, internals::reset_signal>
        base_type;

public:
    explicit rc_sc_signal(
        const sc_module_name& name_=sc_gen_unique_name("signal"))
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

    rc_sc_signal<bool>& operator= (const bool& value)
        { this->write(value); return *this; }

    rc_sc_signal<bool>& operator= (const rc_sc_signal<bool>& signal_)
        { this->write(signal_); return *this; }
};

/**
 * @brief Resettable signal (wrapping class sc_signal<sc_logic>).
 */
template<>
class rc_sc_signal<sc_logic>
    : public rc_sc_signal_b<sc_logic>
{
private:
    typedef rc_sc_signal_b<sc_logic> base_type;

public:
    explicit rc_sc_signal(
        const sc_module_name& name_=sc_gen_unique_name("signal"))
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

    rc_sc_signal<sc_logic>& operator= (const sc_logic& value)
        { this->write(value); return *this; }

    rc_sc_signal<sc_logic>& operator= (const rc_sc_signal<sc_logic>& signal_)
        { this->write(signal_); return *this; }
};

/* inline code */

template<class T, class SIGNAL>
inline
typename rc_sc_signal_b<T, SIGNAL>::if_type&
rc_sc_signal_b<T, SIGNAL>::get_if()
{
    if (p_reconf != NULL && p_constr_done) {
        return *p_if;
    } else {
        return m_signal;
    }
}

template<class T, class SIGNAL>
inline
const typename rc_sc_signal_b<T, SIGNAL>::if_type&
rc_sc_signal_b<T, SIGNAL>::get_if() const
{
    if (p_reconf != NULL && p_constr_done) {
        return *p_if;
    } else {
        return m_signal;
    }
}

template<class T, class SIGNAL>
inline
void rc_sc_signal_b<T, SIGNAL>::driver_write(const T& value, int index)
{
    p_interface_wrapper->get_nb_driver_access(index)->call(
        &if_type::write, value);
}

/* template code */

template<class T, class SIGNAL>
rc_sc_signal_b<T, SIGNAL>::rc_sc_signal_b(const sc_module_name& name_)
    : sc_channel(name_),
      m_signal(name_), m_reset_value(m_signal.read()),
      p_exportal("_rc_exportal"), p_interface_wrapper(NULL), p_if(NULL),
      p_constr_done(false)
{
    p_interface_wrapper = &p_exportal.bind_exclusively(m_signal);
    p_reconf = rc_register_resettable(*this, this->get_parent_object());
    p_if = &((if_type&)p_exportal.static_export());
}

} // namespace ReChannel

#endif // RC_SC_SIGNAL_H_

//
// $Id: rc_sc_signal.h,v 1.10 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_sc_signal.h,v $
//
