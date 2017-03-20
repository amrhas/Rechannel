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
 * @file    rc_signal_accessors.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Defines the accessors for the sc_signal type channels.
 *
 * This file contains the accessors for the sc_signal type interfaces
 * as well as the implementation of the respective rc_fallback_interface.
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.8 $
 */

#ifndef RC_SIGNAL_ACCESSORS_H_
#define RC_SIGNAL_ACCESSORS_H_

#include "ReChannel/communication/accessors/rc_accessor.h"
#include "ReChannel/core/rc_reset_channel_if.h"

namespace ReChannel {

/**
 * @brief Fallback interface for a signal (input).
 */
template<class T>
class rc_fallback_interface<sc_signal_in_if<T> >
: virtual public sc_signal_in_if<T>
{
public:
    rc_fallback_interface()
        : p_value(rc_undefined_value<T>())
    { }
    virtual const T& read() const
        { return p_value; }
    virtual const T& get_data_ref() const
        { return p_value; }
    virtual bool event() const
        { return false; }
    virtual bool posedge() const
        { return false; }
    virtual bool negedge() const
        { return false; }
#if !defined(RC_USE_NON_OSCI_KERNEL)
    virtual sc_reset* is_reset() const
    {
        RC_REPORT_ERROR(RC_ID_NOT_IMPLEMENTED_,
            "'sc_reset* ReChannel::rc_fallback_interface"
            "<sc_signal_inout_if<T> >::is_reset() const'"
            " not implemented");
    }
#endif // !defined(RC_USE_NON_OSCI_KERNEL)
    virtual const sc_event& default_event() const
    {
        SC_REPORT_WARNING(RC_ID_UNDEFINED_EVENT_WARNING_, 0);
        return p_undef;
    }
    virtual const sc_event& value_changed_event() const
        { return default_event(); }
    virtual const sc_event& posedge_event() const
        { return default_event(); }
    virtual const sc_event& negedge_event() const
        { return default_event(); }
private:
    T        p_value;
    sc_event p_undef;
};

/**
 * @brief Fallback interface for a signal (output).
 */
template<class T>
class rc_fallback_interface<sc_signal_inout_if<T> >
: virtual public sc_signal_inout_if<T>
{
public:
    rc_fallback_interface()
        : p_value(rc_undefined_value<T>())
    { }
    virtual const T& read() const
        { return p_value; }
    virtual const T& get_data_ref() const
        { return p_value; }
    virtual bool event() const
        { return false; }
    virtual void write(const T& value)
        { }
    virtual bool posedge() const
        { return false; }
    virtual bool negedge() const
        { return false; }
#if !defined(RC_USE_NON_OSCI_KERNEL)
    virtual sc_reset* is_reset() const
    {
        RC_REPORT_ERROR(RC_ID_NOT_IMPLEMENTED_,
            "'sc_reset* ReChannel::rc_fallback_interface"
            "<sc_signal_inout_if<T> >::is_reset() const'"
            " not implemented");
    }
#endif // !defined(RC_USE_NON_OSCI_KERNEL)
    virtual const sc_event& default_event() const
    {
        SC_REPORT_WARNING(RC_ID_UNDEFINED_EVENT_WARNING_, 0);
        return p_undef;
    }
    virtual const sc_event& value_changed_event() const
        { return default_event(); }
    virtual const sc_event& posedge_event() const
        { return default_event(); }
    virtual const sc_event& negedge_event() const
        { return default_event(); }
private:
    T        p_value;
    sc_event p_undef;
};

/**
 * @brief Base class of an accessor for a signal (input).
 */
template<class T>
class rc_signal_in_accessor_base
    : public rc_abstract_accessor<sc_signal_in_if<T> >
{
protected:
    typedef sc_signal_in_if<T> if_type;

    rc_signal_in_accessor_base() { }

public:
    RC_EVENT(default_event);
    RC_EVENT_ALIAS(default_event, value_changed_event);

    virtual const T& read() const
    {
        return this->rc_nb_forward(&if_type::read);
    }
    virtual const T& get_data_ref() const
    {
        return this->rc_nb_forward(&if_type::get_data_ref);
    }
    virtual bool event() const
    {
        return this->rc_nb_forward(&if_type::event);
    }
};

/**
 * @brief Accessor for a signal (input).
 */
template<class T>
class rc_accessor<sc_signal_in_if<T> >
    : public rc_signal_in_accessor_base<T>
{
public:
    typedef sc_signal_in_if<T> if_type;

    rc_accessor() { }
};

/**
 * @brief Base class of an accessor for a signal (input/edged).
 */
template<class T>
class rc_signal_in_accessor_edged_base
    : public rc_signal_in_accessor_base<T>
{
protected:
    typedef sc_signal_in_if<T> if_type;

    rc_signal_in_accessor_edged_base() { }

public:
    RC_EVENT(posedge_event);
    RC_EVENT(negedge_event);

    virtual bool posedge() const
    {
        return this->rc_nb_forward(&if_type::posedge);
    }
    virtual bool negedge() const
    {
        return this->rc_nb_forward(&if_type::negedge);
    }
};

/**
 * @brief Accessor for signal<bool> (input).
 */
template<>
class rc_accessor<sc_signal_in_if<bool> >
    : public rc_signal_in_accessor_edged_base<bool>,
      virtual public rc_reset_channel_if
{
private:
    typedef rc_accessor<sc_signal_in_if<bool> > this_type;
    typedef internals::reset_signal             reset_signal_type;

public:
    typedef sc_signal_in_if<bool> if_type;

    rc_accessor()
        : p_reset_signal(NULL)
    { }

#if !defined(RC_USE_NON_OSCI_KERNEL)
    virtual sc_reset* is_reset() const
    {
#       if !defined(RC_DISABLE_CTHREAD_MOBILITY)
            // workaround to allow mobility of CTHREADs with the OSCI Kernel
            return _rc_get_reset_signal().is_reset();
#       else // defined(RC_DISABLE_CTHREAD_MOBILITY)
            return this->rc_nb_forward(&if_type::is_reset);
#       endif // !defined(RC_DISABLE_CTHREAD_MOBILITY)
    }
#endif // !defined(RC_USE_NON_OSCI_KERNEL)

protected:
    virtual void register_process_control(
        rc_process_control& pctrl, bool active_level) const;

    virtual void unregister_process_control(
        rc_process_control& pctrl) const;

    virtual bool get_current_level() const
    { return (p_reset_signal != NULL ? p_reset_signal->read() : false); }

    virtual const sc_signal<bool>* get_underlying_reset_signal() const
        { return &_rc_get_reset_signal(); }

private:

    reset_signal_type& _rc_get_reset_signal() const;

    void _rc_reset_updater_proc();

private:
    mutable reset_signal_type* p_reset_signal;
};

/**
 * @brief Accessor for signal<sc_logic> (input).
 */
template<>
class rc_accessor<sc_signal_in_if<sc_dt::sc_logic> >
    : public rc_signal_in_accessor_edged_base<sc_dt::sc_logic>
{
public:
    typedef sc_signal_in_if<sc_dt::sc_logic> if_type;

    rc_accessor() { }
};

/**
 * @brief Base class of an accessor for a signal (input+output).
 */
template<class T>
class rc_signal_accessor_base
    : public rc_abstract_accessor<sc_signal_inout_if<T> >
{
protected:
    typedef sc_signal_inout_if<T> if_type;

    rc_signal_accessor_base() { }

public:
    RC_EVENT(default_event);
    RC_EVENT_ALIAS(default_event, value_changed_event);

    virtual const T& read() const
    {
        return this->rc_nb_forward(&if_type::read);
    }
    virtual const T& get_data_ref() const
    {
        return this->rc_nb_forward(&if_type::get_data_ref);
    }
    virtual bool event() const
    {
        return this->rc_nb_forward(&if_type::event);
    }
    virtual void write(const T& value)
    {
#       if !defined(RC_SIGNAL_WRITE_CHECK_DISABLED)
            this->rc_nb_forward_driver(&if_type::write, value);
#       else
            this->rc_nb_forward(&if_type::write, value);
#       endif
    }
};

/**
 * @brief Accessor for a signal (input+output).
 */
template<class T>
class rc_accessor<sc_signal_inout_if<T> >
    : public rc_signal_accessor_base<T>
{
public:
    typedef sc_signal_inout_if<T> if_type;

    rc_accessor() { }
};

/**
 * @brief Base class of an accessor for a signal (input+output/edged).
 */
template<class T>
class rc_signal_accessor_edged_base
    : public rc_signal_accessor_base<T>
{
protected:
    typedef sc_signal_inout_if<T> if_type;

    rc_signal_accessor_edged_base() { }

public:
    RC_EVENT(posedge_event);
    RC_EVENT(negedge_event);

    virtual bool posedge() const
    {
        return this->rc_nb_forward(&if_type::posedge);
    }
    virtual bool negedge() const
    {
        return this->rc_nb_forward(&if_type::negedge);
    }
};

/**
 * @brief Accessor for signal<bool> (input+output).
 */
template<>
class rc_accessor<sc_signal_inout_if<bool> >
    : public rc_signal_accessor_edged_base<bool>,
      virtual public rc_reset_channel_if
{
private:
    typedef rc_accessor<sc_signal_inout_if<bool> > this_type;
    typedef internals::reset_signal                reset_signal_type;

public:
    typedef sc_signal_inout_if<bool> if_type;

    rc_accessor()
        : p_reset_signal(NULL)
    { }

#if !defined(RC_USE_NON_OSCI_KERNEL)
    virtual sc_reset* is_reset() const
    {
#       if !defined(RC_DISABLE_CTHREAD_MOBILITY)
            // workaround to allow mobility of CTHREADs with the OSCI Kernel
            return _rc_get_reset_signal().is_reset();
#       else // defined(RC_DISABLE_CTHREAD_MOBILITY)
            return this->rc_nb_forward(&if_type::is_reset);
#       endif // !defined(RC_DISABLE_CTHREAD_MOBILITY)
    }
#endif // !defined(RC_USE_NON_OSCI_KERNEL)

protected:
    virtual void register_process_control(
        rc_process_control& pctrl, bool active_level) const;

    virtual void unregister_process_control(
        rc_process_control& pctrl) const;

    virtual bool get_current_level() const
    { return (p_reset_signal != NULL ? p_reset_signal->read() : false); }

    virtual const sc_signal<bool>* get_underlying_reset_signal() const
        { return &_rc_get_reset_signal(); }

private:

    reset_signal_type& _rc_get_reset_signal() const;

    void _rc_reset_updater_proc();

private:
    mutable reset_signal_type* p_reset_signal;
};

/**
 * @brief Accessor for signal<sc_logic> (input+output).
 */
template<>
class rc_accessor<sc_signal_inout_if<sc_dt::sc_logic> >
    : public rc_signal_accessor_edged_base<sc_dt::sc_logic>
{
public:
    typedef sc_signal_inout_if<sc_dt::sc_logic> if_type;

    rc_accessor() { }
};

} // namespace ReChannel

#endif // RC_SIGNAL_ACCESSORS_H_
//
// $Id: rc_signal_accessors.h,v 1.8 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/accessors/rc_signal_accessors.h,v $
//
