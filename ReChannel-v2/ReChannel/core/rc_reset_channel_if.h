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
 * @file    rc_reset_channel_if.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Interface rc_reset_channel_if and implementations of reset signals.
 *
 * \see ReChannel::rc_reset_channel_if, \ref sec_reset_proc
 *
 * $Date: 2007/10/09 21:47:01 $
 * $Revision: 1.5 $
 */

#ifndef RC_RESET_CHANNEL_IF_H_
#define RC_RESET_CHANNEL_IF_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_process.h"

namespace ReChannel {

namespace internals {
class process_support;
} // namespace internals

/**
 * @brief The pure virtual interface of a reset channel.
 * 
 * Reset channels implement the interface rc_reset_channel_if to enable them
 * to be used as a reset trigger for %ReChannel's resettable processes.
 * 
 * A reset channel has an internal state that is called the current level
 * ("high" or "low"). The way this level is constituted, is subject to the
 * respective channel implementation.
 *
 * The reset condition of a registered process is met if the the current level
 * of the channel equals the process's so-called active level.
 * When a channel's current level changes, the reset shall be triggered by
 * deactivating all processes whose reset condition is met.
 * 
 * \remarks Processes are not registered directly, instead a process control
 *          object is passed that grants control of a particular process.
 *
 * \see \ref sec_reset_proc
 */
class rc_reset_channel_if
    : virtual public sc_interface
{
    /**
     * @brief Grants the process functionality access to the protected
     *        interface methods.
     */
    friend class internals::process_support;

protected:
    /**
	 * @brief Registers a process control at the channel's reset list
	 * @param pctrl The process control object that shall be deactivated
     *              in case of a reset.
     * @param active_level The reset trigger level (high or low).
     *
     * \see rc_process_control
	 */
	virtual void register_process_control(
        rc_process_control& pctrl, bool active_level) const = 0;

    /**
	 * @brief Unregisters a process control from the channel's reset list
	 * @param pctrl The process control object that shall be unregistered
     *
     * The opposite of register_process_control().
     *
     * \see rc_process_control
	 */
    virtual void unregister_process_control(
        rc_process_control& pctrl) const = 0;

    /**
	 * @brief Returns the reset channel's current level.
	 * @return the current level of the channel.
	 */
	virtual bool get_current_level() const = 0;

    /**
	 * @brief Returns the underlying SystemC signal (if such exists)
	 * @return a SystemC signal this channel is based on.
     *
     * \note SystemC's reset mechanism is not standardised.
     * Hence for some SystemC implementations (e.g. OSCI SystemC) it may
     * be useful to have access to a native channel.
     *
     * \see sc_core::sc_signal_inout_if::reset_is()
	 */
    virtual const sc_signal<bool>* get_underlying_reset_signal() const = 0;
};

namespace internals {

/**
 * @brief Implementation of a reset signal (used internally).
 *
 * \see ReChannel::rc_reset_channel_if
 */
class reset_signal
    : public sc_signal<bool>,
      virtual public rc_reset_channel_if
{
private:
    typedef sc_signal<bool> base_type;
    typedef std::set<rc_process_control*> pctrl_set;

public:
    reset_signal(const char* name_=sc_gen_unique_name("signal"))
        : base_type(name_)
    {}

    virtual void register_process_control(
        rc_process_control& pctrl, bool active_level) const;

    virtual void unregister_process_control(
        rc_process_control& pctrl) const;

    operator const bool&() const
        { return this->read(); }

    reset_signal& operator= (const bool& value)
        { this->write(value); return *this; }

    reset_signal& operator= (const reset_signal& buffer_)
        { this->write(buffer_); return *this; }

protected:
    virtual bool get_current_level() const
        { return this->read(); }

    virtual const sc_signal<bool>* get_underlying_reset_signal() const
        { return this; }

    virtual void update();

private:
    mutable pctrl_set p_pctrl_set[2];
};

/**
 * @brief Implementation of a reset buffer signal (used internally).
 *
 * \see ReChannel::rc_reset_channel_if
 */
class reset_buffer
    : public sc_buffer<bool>,
      virtual public rc_reset_channel_if
{
private:
    typedef sc_buffer<bool> base_type;
    typedef std::set<rc_process_control*> pctrl_set;

public:
    reset_buffer(const char* name_=sc_gen_unique_name("buffer"))
        : base_type(name_)
    {}

    virtual void register_process_control(
        rc_process_control& pctrl, bool active_level) const;

    virtual void unregister_process_control(
        rc_process_control& pctrl) const;

    operator const bool&() const
        { return this->read(); }

    reset_buffer& operator= (const bool& value)
        { this->write(value); return *this; }

    reset_buffer& operator= (const reset_buffer& buffer_)
        { this->write(buffer_); return *this; }

protected:
    virtual bool get_current_level() const
        { return this->read(); }

    virtual const sc_signal<bool>* get_underlying_reset_signal() const
        { return this; }

    virtual void update();

private:
    mutable pctrl_set p_pctrl_set[2];
};

} // namespace internals

} // namespace ReChannel

#endif // RC_RESET_CHANNEL_IF_H_

//
// $Id: rc_reset_channel_if.h,v 1.5 2007/10/09 21:47:01 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_reset_channel_if.h,v $
//
