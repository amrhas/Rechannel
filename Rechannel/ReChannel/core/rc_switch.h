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
 * @file    rc_switch.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   ABI rc_switch.
 *
 *
 * $Date: 2007/12/20 20:40:53 $
 * $Revision: 1.14 $
 */

#ifndef RC_SWITCH_H_
#define RC_SWITCH_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/util/rc_object_handle.h"

namespace ReChannel {

class rc_reconfigurable;
class rc_switch_connector_base;
class rc_interface_filter;

/**
 * @brief The pure virtual interface of a switch.
 *
 * <b>The Concept</b>
 *
 * A class that implements the pure virtual interface rc_switch is regarded to
 * be a "switch" (as specified in \ref sec_rechannel_concept).
 *
 * A switch is a helper component that serves as a connector of the dynamic
 * and the static side. The basic principle of a switch is similar to that of a
 * multiplexer (or demultiplexer, respectively). The switch's task is to
 * control communication between the static side and an arbitrary number of
 * reconfigurable modules on the dynamic side. The communication has to be
 * channeled from a static interface to a dynamic interface, or vice versa.
 *
 * <b>Binding</b>
 *
 * The binding of a switch to a communication object of a reconfigurable
 * module includes registering the reconfigurable module with the switch and
 * registering the switch with the reconfigurable module.
 * (see also: \link rc_switch::register_reconfigurable()\endlink,
 *      \link rc_reconfigurable::rc_register_switch()\endlink)
 *
 * Binding may only be performed during the elaboration phase.
 * Whereas registering or unregistering of reconfigurable modules is allowed
 * during elaboration and simulation.
 * 
 * The single communication object of the static side, that has has to be bound
 * to this switch, is fixed during simulation.
 * Whereas, the bound reconfigurable module on the dynamic side may vary over
 * time. The reconfigurable modules that represent the dynamic side have to be
 * registered with the switch.
 *
 * Among all registered reconfigurable module at most one shall be loaded.
 * A switch shall enforce the rule that only one reconfigurable can be loaded
 * at a time. In case of a conflict an error shall be reported.
 *
 * <b>Control</b>
 *
 * The switch is a passive object that receives commands from
 * a rc_reconfigurable instance (in most cases this will be the currently
 * active one which is holding an exclusive lock on this switch).
 * Most getter members of rc_switch are publicly accessible.
 * Whereas all control methods are declared as "protected" and may only be
 * accessed by friend classes, i.e. primarily by rc_reconfigurable.
 *
 * <b>Switch States</b>
 *
 * Switches possess a basic "switch behaviour", as they can be opened and
 * closed just like ordinary switches. Hence, the valid switch states are UNDEF,
 * OPEN and CLOSED.
 * A state transition to OPEN shall only be performed if a registered
 * reconfigurable module actually is loaded.
 * UNDEF will be the current state if a switch is opened but currently no
 * registered module exists that is loaded. A switch may be closed at every time
 * when no blocking transactions are pending on this switch.
 *
 * <b>Communication</b>
 *
 * The switch has to ensure that only the currently active module is allowed to
 * communicate. Unloaded modules shall be blocked from any external events
 * and access attempts. If the switch is CLOSED or UNDEF all communication has
 * to be blocked.
 *
 * The communication may be filtered by a chain of interface filters, which can
 * be set when the switch is opened up for a particular reconfigurable module
 * instance.
 *
 * Events shall be forwarded to the interface of the opposite
 * side they arised.
 *
 * The switch implementation is solely responsible for a correct communication
 * flow. A particular switch implementation may impose additional restrictions
 * or constraints on its usage.
 *
 * \see \ref sec_sim_sem, \ref sec_rechannel_concept, \ref sec_conn_switch,
 *      rc_reconfigurable, rc_interface_filter
 */
class rc_switch
{
    /**
     * @brief Grant rc_reconfigurable full access to control the switch.
     */
    friend class rc_reconfigurable;
    /**
     * @brief Allow rc_switch_connector_base to call the binding methods.
     */
    friend class rc_switch_connector_base;

public:

    /**
     * @brief The three possible states of a switch.
     */
    enum state_type { UNDEF=0, OPEN, CLOSED };

    /**
     * @brief The number of possible switch states.
     */
    enum { STATE_COUNT=3 };

    /**
     * @brief Type of the filter chain.
     *
     * A filter chain is a vector of interface filters.
     */
    typedef std::vector<rc_interface_filter*> filter_chain;

public:

    /**
     * @brief The kind of the switch.
     */
    virtual std::string get_switch_kind() const = 0;

    /**
     * @brief The name of the switch.
     */
    virtual std::string get_switch_name() const = 0;

    /**
     * @brief The current state
     */

    virtual state_type get_switch_state() const = 0;

    /**
     * @brief The number of pending transactions at this switch.
     */
    virtual unsigned int get_transaction_count() const = 0;

    /**
     * @brief Returns the static side's communication interface.
     *
     * This method is intended for informational purposes only. Invoking methods
     * of the interface returned by this method is not allowed from outside of
     * the portal.
     */
    virtual sc_interface* get_static_interface() const = 0;

    /**
     * @brief Returns the currently active reconfigurable side's interface.
     * @return the interface pointer if switch state equals OPEN, NULL otherwise
     *
     * This method is intended for informational purposes only. Invoking methods
     * of the interface returned by this method is not allowed from outside of
     * the portal.
     */
    virtual sc_interface* get_dynamic_interface() const = 0;

    /**
     * @brief Returns the current reconfigurable module.
     * @return the current reconfigurable if such exists, NULL otherwise
     */
    virtual rc_reconfigurable* get_current_reconfigurable() const = 0;

    /**
     * @brief Is the switch locked by a rc_reconfigurable?
     *
     * \note Locking is used to detect reconfiguration conflicts.
     */
    virtual bool is_locked() const = 0;

    /**
     * @brief Is the given rc_reconfigurable registered with this switch?
     */
    virtual bool is_registered(const rc_reconfigurable& reconf) const = 0;

    /**
     * @brief Is the given dynamic interface registered with this switch?
     */
    virtual bool is_registered(const sc_interface& dyn_if) const = 0;

protected:

    /**
     * @brief Instructs the switch to bind itself to the given static object.
     *
     * \remarks An error shall be reported if binding is impossible.
     */
    virtual void bind_static_object(
        const rc_object_handle& obj_to_bind) = 0;

    /**
     * @brief Instructs the switch to bind itself to the given dynamic object.
     *
     * \remarks An error shall be reported if binding is impossible.
     */
    virtual void bind_dynamic_object(
        const rc_object_handle& obj_to_bind) = 0;

    /**
     * @brief Opens up the switch.
     *
     * If the switch is connected to an active rc_reconfigurable, the state
     * is changed to OPEN, otherwise to UNDEF.
     */
    virtual void open() = 0;

    /**
     * @brief Opens up the switch for a particular reconfigurable module.
     *
     * If the switch is connected to an active rc_reconfigurable, the state
     * is changed to OPEN, otherwise to UNDEF.
     *
     * A filter chain can be specified that shall filter the communication.
     *
     * \remark If the reconfigurable has not previously been registered
     *         an error shall be reported.
     */
    virtual void open(
        rc_reconfigurable& reconf,
        const filter_chain& filters = filter_chain()) = 0;

    /**
     * @brief Closes the switch.
     *
     * Sets the switch to CLOSED state.
     */
    virtual void close() = 0;

    /**
     * @brief Sets the switch to UNDEF state.
     */
    virtual void set_undefined() = 0;

    /**
     * @brief Request a refresh of the event notifications.
     */
    virtual void refresh_notify() = 0;

    /**
     * @brief Registers a rc_reconfigurable instance.
     * @param reconf the rc_reconfigurable instance
     * @param dyn_if the connected dynamic interface
     */
    virtual void register_reconfigurable(
        rc_reconfigurable& reconf, sc_interface& dyn_if) = 0;

    /**
     * @brief Unregisters a previously registered rc_reconfigurable instance.
     *
     * \remark Unregistering of a loaded module shall result in
     *         an error.
     */
    virtual void unregister_reconfigurable(
        rc_reconfigurable& reconf) = 0;

    /**
     * @brief Returns the dynamic interface of a registered rc_reconfigurable.
     *
     * This method is intended for informational purposes only. Invoking methods
     * of the interface returned by this method is not allowed from outside of
     * the portal.
     */
    virtual sc_interface* get_registered_interface(
        rc_reconfigurable& reconf) const = 0;

    /**
     * @brief Has the switch been locked by the given rc_reconfigurable?
     *
     * \note Locking is used to detect reconfiguration conflicts.
     */
    virtual bool is_lock_owner(const rc_reconfigurable& reconf) const = 0;

    /**
     * @brief Sets/unsets the lock owner.
     *
     * \note Locking is used to detect reconfiguration conflicts.
     */
    virtual bool set_locked(rc_reconfigurable& lock_owner, bool lock) = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~rc_switch() {}
};

}

#endif //RC_SWITCH_H_

//
// $Id: rc_switch.h,v 1.14 2007/12/20 20:40:53 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_switch.h,v $
//
