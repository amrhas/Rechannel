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
 * @file    rc_reconfigurable.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class rc_reconfigurable.
 *
 *
 * $Date: 2008/01/01 13:34:00 $
 * $Revision: 1.22 $
 */

#ifndef RC_RECONFIGURABLE_H_
#define RC_RECONFIGURABLE_H_

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_switch.h"
#include "ReChannel/core/rc_resettable.h"
#include "ReChannel/core/rc_process.h"
#include "ReChannel/core/rc_reconfigurable_set.h"
#include "ReChannel/core/rc_portmap.h"
#include "ReChannel/core/rc_switch_connector.h"

#include "ReChannel/util/rc_driver_object.h"
#include "ReChannel/util/rc_mutex_object.h"
#include "ReChannel/util/rc_delta_sync_object.h"
#include "ReChannel/util/rc_hash_map.h"
#include "ReChannel/util/rc_object_handle.h"
#include "ReChannel/util/rc_port_handle.h"
#include "ReChannel/util/rc_export_handle.h"

namespace ReChannel {

class rc_control;
class rc_transaction_counter;

// forward declarations
class rc_reconfigurable;
rc_reconfigurable* rc_get_reconfigurable_context();
rc_reconfigurable* rc_get_reconfigurable_context(sc_object* start_search);

namespace internals {
namespace reconfigurable {

/**
 * @brief A temporary object derived from sc_object.
 *
 * Can be temporarily created to determine the current reconfigurable context
 * from SystemC's object hierarchy.
 *
 * \see \ref sec_reconf_context
 */
class temporary_object : public sc_object {};

/**
 * @brief Helper class that frames the construction of a reconfigurable object.
 * (internally used)
 * 
 * A class that is derived from rc_reconfigurable can be additionally derived
 * from internals::reconfigurable::begin_contruction to allow the
 * identification of the current reconfigurable context during a subsequent
 * base class and member construction.
 * 
 * It helps function rc_get_reconfigurable_context() in providing the
 * correct context instance before its construction actually is complete.
 * The construction of the reconfigurable object's base classes and members is
 * framed by its creation and a final call to finish().
 *
 * \note For an example how this class is used, see documentation of
 * class rc_reconfigurable_module.
 */
class begin_construction
{
    friend void end_construction(begin_construction& constr);
    friend rc_reconfigurable* ::ReChannel::rc_get_reconfigurable_context();
    friend rc_reconfigurable* ::ReChannel::rc_get_reconfigurable_context(
        sc_object* start_search);

protected:
    /**
     * @brief Marks the begin of a reconfigurable object's construction.
     *
     * \attention finish() has to be called once at the end of construction!
     * Otherwise it will result in undefined behavior.
     */
    explicit begin_construction(rc_reconfigurable& reconf);

    /**
     * @brief Marks the end of a reconfigurable object's construction.
     *
     * \attention finish() has to be called once at the end of construction!
     * Otherwise it will result in undefined behavior.
     *
     * \see ::end_construction()
     */
    void finish();

     /**
     * @brief Marks the end of a reconfigurable object's construction.
     * @param reconf reference to the constructed sc_object
     *
     * If the reconfigurable object is derived from sc_object this is the best
     * point to initialise rc_reconfigurable with a reference to its sc_object.
     *
     * \attention finish() has to be called once at the end of construction!
     * Otherwise it will result in undefined behavior.
     */
    void finish(sc_object& reconf);

     /**
      * @brief Destructor.
      *
      * \note Destructor calls finish() (for continuity purposes).
      */
    ~begin_construction()
        { finish(); }

private:
    /**
	 * @brief A pointer to a possibly enclosing reconfigurable object that is
     * currently under construction, too.
	 */
	rc_reconfigurable* p_previous;
    /**
	 * @brief Flag that indicates whether finish() has been called.
	 */
    bool               p_finished;

private:
    /**
	 * @brief A pointer to the reconfigurable object that is
     * currently under construction.
	 */
    static rc_reconfigurable* s_current;

private:
    //disabled
    /** @brief (disabled) */
	begin_construction(const begin_construction& reconf);
    /** @brief (disabled) */
    begin_construction& operator=(const begin_construction& reconf);
};

/**
 * @brief Marks the end of a reconfigurable object's construction.
 * @param constr The construction observer
 *
 * A global function doing the same as calling finish() on the
 * begin_construction object.
 *
 * \see ::end_construction()
 */
inline void end_construction(begin_construction& constr)
    { constr.finish(); }

} // namespace reconfigurable
} // namespace internals

/**
 * @brief This class represents a reconfigurable object.
 * 
 * Theoretically, any object can be used with %ReChannel's reconfiguration
 * algorithm, as the only technical precondition is that it has to be derived
 * from class rc_reconfigurable. This class contains all the behaviour and
 * state information that is necessary for the simulation of reconfiguration in
 * %ReChannel to work.
 *
 * The reconfigurable has an internal transaction counter. Only if the number
 * of open transactions equals zero the reconfigurable can be
 * deactivated. If an operation is in progress that has to finish
 * safely before the reconfigurable object may be deactivated, a transaction
 * shall be begun. The transaction shall be ended immediately after the
 * operation has been done.
 *
 * \see \ref sec_derive_reconf_module, \ref sec_reconf_module,
 *      \ref sec_reconf_control, \ref sec_moving_reconf_module,
 *      rc_control
 */
class rc_reconfigurable
{
    /**
     * @brief Allow rc_control to control reconfigurable objects.
     */
    friend class rc_control;

    friend class internals::reconfigurable::begin_construction;

    /**
     * @brief Grant rc_transaction access to transaction related members.
     */
    friend class rc_transaction;

    /**
     * @brief Grant rc_transaction_counter access to transaction related
     * members.
     */
    friend class rc_transaction_counter;

public:
    /**
     * @brief Switch connector base type.
     * \see rc_switch_connector_base
     */
    typedef rc_switch_connector_base switch_conn_type;

    /**
     * @brief The type of a filter chain.
     * \see rc_switch
     */
    typedef rc_switch::filter_chain  filter_chain;

private:
    typedef std::pair<rc_switch*, rc_object_handle> switch_commobj_pair;

    /**
     * @brief The type of the multi-indexed (multi)map of communication objects
     * and switches.
     */
    typedef boost::multi_index_container<
              switch_commobj_pair,
              boost::multi_index::indexed_by<
                boost::multi_index::ordered_unique<
                  BOOST_MULTI_INDEX_MEMBER(
                    switch_commobj_pair, rc_switch*, first)
                >,
                boost::multi_index::ordered_non_unique<
                  BOOST_MULTI_INDEX_MEMBER(
                    switch_commobj_pair, rc_object_handle, second)
                >
              >
            > switch_commobj_map;

    /**
     * @brief The type of the map index: switch -> communication objects
     */
    typedef switch_commobj_map::nth_index<0>::type   switch_commobj_index;

    /**
     * @brief The type of the map index: communication objects -> switch
     */
    typedef switch_commobj_map::nth_index<1>::type   commobj_switch_index;

    /**
     * @brief The type of a range of the map index switch_commobj_index.
     */
    typedef std::pair<
                switch_commobj_index::iterator,
                switch_commobj_index::iterator>      switch_commobj_range;

    /**
     * @brief The type of a range of the map index commobj_switch_index.
     */
    typedef std::pair<
                commobj_switch_index::iterator,
                commobj_switch_index::iterator>      commobj_switch_range;

    /**
     * @brief The type of a set of filters.
     */
    typedef std::set<rc_interface_filter*>           filter_set;

    /**
     * @brief The type of the map: communication object -> filter chain.
     */
    typedef std::map<rc_object_handle, filter_chain> commobj_filters_map;

    /**
     * @brief The type of a set of resettable objects.
     */
    typedef std::set<rc_resettable*>                 resettable_set;

    /**
     * @brief The type of a set of resettable objects.
     */
    typedef std::vector<rc_portmap_base*>            portmap_vector;

public:
    /**
     * @brief The three possible reconfiguration states of a rc_reconfigurable.
     */
    enum state_type { UNLOADED=0, INACTIVE, ACTIVE };

    /**
     * @brief The number of possible reconfiguration states.
     */
    enum { STATE_COUNT=3 };

    /**
     * @brief The four possible reconfiguration actions.
     */
    enum action_type { UNLOAD=0, LOAD, ACTIVATE, DEACTIVATE };

    /**
     * @brief The number of possible reconfiguration actions.
     */
    enum { ACTION_COUNT=4 };

protected:

    /**
     * @brief The only available constructor.
     * @param this_ a pointer to the sc_object of this instance
     * (if already known before)
     */
    rc_reconfigurable(sc_object* this_=NULL);

public:

    /**
     * @brief Returns the name of this object.
     *
     * \remark If this object is derived from sc_object, the reconfigurable's
     * name equals the name of sc_object.
     */
    std::string rc_get_name() const;

    /**
     * @brief Returns the sc_object representation
     * @return pointer to sc_object if sc_object is a base class,
     *         NULL otherwise.
     */
    const sc_object* rc_get_object() const;

    /**
     * @brief Returns the sc_object representation
     * @return pointer to sc_object if sc_object is a base class,
     *         NULL otherwise.
     */
    sc_object* rc_get_object();

    /**
     * @brief Returns the current reconfiguration state
     */
    inline state_type rc_get_state() const
        { return p_state; }

    /**
     * @brief Determines whether the state is about to change due to an on-going
     * reconfiguration operation.
     */
    inline bool rc_is_state_changing() const
        { return (p_state != p_next_state); }

    /**
     * @brief Returns the next reconfiguration state.
     */
    inline state_type rc_get_next_state() const
        { return p_next_state; }

    /**
     * @brief Is the reconfigurable object currently loaded?
     */
    inline bool rc_is_loaded() const
        { return (p_state != UNLOADED); }

    /**
     * @brief Is the reconfigurable object currently active?
     */
    inline bool rc_is_active() const
        { return (p_state == ACTIVE); }

    /**
     * @brief Returns the number of on-going transactions.
     *
     * \remark While transactions exist, a reconfigurable cannot
     * be deactivated.
     */
    inline int rc_get_transaction_count() const
        { return p_transaction_count; }

    /**
     * @brief Bind this reconfigurable object to a switch connector.
     *
     * \attention This method may only be called during the elaboration phase.
     */
    void bind(rc_switch_connector_base& switch_connector);

    /**
     * @brief Registers a bound switch to this reconfigurable.
     *
     * \remark This method is called by a switch implementation
     * during the binding process.
     *
     * \see rc_switch
     */
    void rc_register_switch(
        rc_switch& switch_obj, const rc_object_handle& bound_obj);

    /**
     * @brief Is the given switch registered with this reconfigurable?
     */
    bool rc_is_registered(rc_switch& switch_obj) const;

    /**
     * @brief Registers a resettable object.
     *
     * Resettable components register itself to the reconfigurable context
     * to be reset if the context is deactivated.
     *
     * \see rc_resettable
     */
    void rc_register_resettable(rc_resettable& resettable);

    /**
     * @brief Is the given resettable object registered?
     */
    bool rc_is_registered(rc_resettable& resettable) const;

    /**
     * @brief Begin a transaction.
     *
     * Increases the internal transaction counter by one.
     *
     * \attention This method may only be called if can be guaranteed
     * that for every begun transaction is ended by rc_end_transaction().
     * Wrong usage of these methods will result in undefined behaviour.
     */
    inline void rc_begin_transaction()
        { ++p_transaction_count; }

    /**
     * @brief Ends a previously begun transaction.
     *
     * Decreases the internal transaction counter by one.
     *
     * \attention This method may only be called if can be guaranteed
     * that for every begun transaction is ended by rc_end_transaction().
     * Wrong usage of these methods will result in undefined behaviour.
     */
    inline void rc_end_transaction();

    /**
     * @brief Marks a point of possible deactivation for thread processes.
     *
     * A thread invoking rc_possible_deactivation() will be suspended
     * if the deactivation of the reconfigurable object was
     * requested and the transaction count equals zero.
     */
    void rc_possible_deactivation();

    /**
     * @brief Marks a point of possible deactivation for thread processes.
     *
     * If a deactivation is requested a thread invoking
     * rc_possible_deactivation_delta() will be suspended till the next
     * delta cycle. This may be useful on the UTF abstraction
     * level.
     *
     * \note The more restrictive alternative would be using
     *       method rc_possible_deactivation() instead.
     */
    void rc_possible_deactivation_delta();

    /**
     * @brief Is a deactivation operation currently in progress?
     */
    inline bool rc_is_deactivation_requested() const
        { return (p_next_state == INACTIVE && p_state == ACTIVE); }

    /**
     * @brief Set the time a specific reconfiguration action will take.
     *
     * \remarks Also overwrites the default time if the reconfigurable
     * object is not registered to a control.
     */
    void rc_set_delay(action_type a, sc_time t);

    /**
     * @brief Returns the time taken by a particular reconfiguration action.
     */
    inline sc_time rc_get_delay(action_type a) const
        { return p_action_delays[a]; }

    /**
     * @brief Set the default time a specific reconfiguration action will take.
     */
    inline void rc_set_default_delay(action_type a, sc_time t)
        { p_action_default_delays[a] = t; }

    /**
     * @brief Returns the default time taken by a particular reconfiguration
     * action.
     *
     * \remark The default time is the time set before the reconfigurable
     * was added to a control. It may also be directly set by method
     * rc_set_default_delay().
     */
    inline sc_time rc_get_default_delay(action_type a) const
        { return p_action_default_delays[a]; }

    /**
     * @brief Implicit cast to a set containing only this object.
     *
     * \see rc_reconfigurable_set
     */
    inline operator const rc_reconfigurable_set&() const
        { return p_self_set; }

    /**
     * @brief Returns the internal process control instance.
     *
     * Processes controlled by this process control instance
     * will be automatically reset in case of a deactivation.
     *
     * \see rc_process_control
     */
    inline const rc_process_control& rc_get_process_control() const
        { return p_pctrl; }

    /**
     * @brief Returns the internal process control instance.
     *
     * Processes controlled by this process control instance
     * will be automatically reset in case of a deactivation.
     */
    inline switch_conn_type* rc_get_current_switch_connector() const
        { return p_curr_switch_conn; }

    /**
     * @brief Destructor.
     */
    virtual ~rc_reconfigurable();

protected:

    /**
     * @brief A callback which is called after successful loading.
     *
     * This callback is invoked by an external controlling process.
     *
     * \remark Can be overloaded by a user for user-defined actions.
     * \note May contain waits.
     */
    virtual void rc_on_load() {}

    /**
     * @brief A callback which is called after successful activation.
     *
     * This callback is invoked by an external controlling process.
     *
     * \remark Can be overloaded by a user for user-defined actions.
     * \note May contain waits.
     */
    virtual void rc_on_activate() {}

    /**
     * @brief A callback which is called after successful deactivation.
     *
     * This callback is invoked by an external controlling process.
     *
     * \remark Can be overloaded by a user for user-defined actions.
     * \note May contain waits.
     */
    virtual void rc_on_deactivate() {}

    /**
     * @brief A callback which is called after successful unloading.
     *
     * This callback is invoked by an external controlling process.
     *
     * \remark Can be overloaded by a user for user-defined actions.
     * \note May contain waits.
     */
    virtual void rc_on_unload() {}

    /**
     * @brief Adds a port map this rc_reconfigurable is compatible with.
     *
     * \remarks The port map may only contain communication objects that
     * belong to this reconfigurable context.
     *
     * \see \ref sec_switch_connectors, \ref sec_moving_reconf_module
     */
    void rc_add_portmap(rc_portmap_base& portmap);

    /**
     * @brief Clears all previously added port maps.
     */
    void rc_clear_portmaps();

    /**
     * @brief Checks the compatibility with a given port map.
     *
     * \see \ref sec_switch_connectors, \ref sec_moving_reconf_module
     */
    bool rc_is_compatible(const rc_portmap_base& portmap) const
        { return (this->_rc_is_compatible(portmap) > -1); }

    /**
     * @brief Adds a filter to the filter chain of the given
     * communication object.
     * @param commobj_ the object the filter shall be added to
     * @param commobj_ a filter of compatible type
     */
    void rc_add_filter(
        const rc_object_handle& commobj_, rc_interface_filter& filter_);

    /**
     * @brief Returns the size of the filter chain of a given
     * communication object.
     */
    int rc_get_filter_count(const rc_object_handle& commobj_) const;

    /**
     * @brief Returns a copy of the filter chain of a given
     * communication object.
     */
    filter_chain rc_get_filter_chain(
        const rc_object_handle& commobj_) const;

    /**
     * @brief Is the given filter part of a filter chain.
     */
    bool rc_has_filter(rc_interface_filter& filter_) const;

    /**
     * @brief Instruct the switch connected to the given object to refresh its
     * event notifications.
     */
    void rc_refresh_notify(const rc_object_handle& commobj_);

    /**
     * @brief Instruct all registered switches to refresh their event
     * notifications.
     */
    void rc_refresh_notify_all();

private:

    /**
     * @brief Copies the vector that contains all existing
     * reconfigurable objects.
     */
    static void get_all_reconfigurables(
        std::vector<rc_reconfigurable*>& copy_dest)
        { copy_dest = s_reconfigurables; }

private:
    /**
     * @brief Returns the control (if exists).
     */
    inline rc_control* get_control() const
        { return p_control; }

    /**
     * @brief Sets/resets the control.
     */
    void set_control(rc_control* control)
        { p_control = control; }

    /**
     * @brief Completes initialization at the start of simulation.
     *
     * This method is called by rc_control to complete the initialisation
     * of the respective reconfigurable instance.
     * \note It is ensured that start_of_simulation() is invoked on every
     * reconfigurable object, even if it was not added to a control.
     * \attention This method is not called by the SystemC kernel.
     */
    void start_of_simulation();

    /**
     * @brief Performs a reconfiguration action.
     * \note Called by rc_control.
     *       This method may also be invoked during elaboration.
     */
    void reconfigure(state_type new_state);

    /**
     * @brief Changes the state of the registered switches.
     * \note This method may also be invoked during elaboration.
     */
    void change_switch_state(rc_switch::state_type new_state);

    /**
     * @brief Moves this reconfigurable instance to another switch connector.
     * \note Called by rc_control.
     */
    void move(switch_conn_type& target);

    /**
     * @brief Does the current process hold a lock on this instance?
     */
    inline bool has_lock() const
        { return p_mutex.has_lock(); }

    /**
     * @brief Is this instance locked by a controlling process?
     */
    inline bool is_locked() const
        { return p_mutex.is_locked(); }

    /**
     * @brief Returns the lock release event.
     */
    inline const sc_event& get_lock_release_event() const
        { return p_mutex.get_lock_release_event(); }

    /**
     * @brief Tries to obtain a lock on this reconfigurable (non-blocking).
     * @return true on success, false otherwise.
     */
    inline bool trylock()
        { return p_mutex.trylock(); }

    /**
     * @brief Obtain a lock on this reconfigurable (blocking).
     */
    inline void lock()
        { p_mutex.lock(); }

    /**
     * @brief Obtain a lock on this reconfigurable (blocking, with timeout).
     */
    inline bool lock(sc_time timeout)
        { return p_mutex.lock(timeout); }

    /**
     * @brief Release the lock hold by the current process.
     * @return true if lock was hold and is now released, false otherwise.
     */
    bool unlock();

    /**
     * @brief Register another process that shares current the lock with the
     *        calling process.
     * @return true on success, false if calling process did not have the lock.
     * \remark Only calling processes that hold a lock can share this lock.
     */
    bool share_lock(sc_process_handle proc);

    /**
     * @brief Release a previously set lock share.
     */
    bool reset_lock_share();

    /**
     * @brief Does the current process hold a shared lock?
     */
    inline bool has_lock_share() const
        { return (p_lock_share == sc_get_current_process_handle()); }

    /**
     * @brief Lock all registered switches for this reconfigurable.
     * \remark Is called before a reconfigurable is loaded.
     */
    bool lock_switches(bool report_error=false);

    /**
     * @brief Release the lock on all registered switches for this
     * reconfigurable.
     * \remark Is called after a reconfigurable is unloaded.
     */
    void unlock_switches();

    /**
     * @brief Set transaction count to zero.
     * \remark May only be used by ReChannel.
     * \attention Wrong usage will result in undefined behaviour.
     */
    void reset_transaction_count();

    /**
     * @brief Get the registered port map of given index.
     */
    rc_portmap_base& get_portmap(int index);

    /**
     * @brief Get the number of registered port maps.
     */
    int get_portmap_count() const
        { return p_portmap_vector.size(); }

private:

/* for internal use only */

    /**
     * @brief Initialises the sc_object reference. (internal only)
     */
    void _rc_set_sc_object(sc_object& reconf);

    /**
     * @brief Callback for rc_delta_sync_object. (internal only)
     * 
     * Is used to synchronise reconfiguration operations with delta
     * cycle boundaries.
     */
    void _rc_delta_sync_state_change(rc_delta_sync_object& delta_sync);

    /**
     * @brief Performes the unloading action. (internal only)
     */
    void _rc_unload();

    /**
     * @brief Performes the loading action. (internal only)
     */
    void _rc_load();

    /**
     * @brief Performes the activation action. (internal only)
     */
    void _rc_activate();

    /**
     * @brief Performes the deactivation action. (internal only)
     */
    void _rc_deactivate();

    /**
     * @brief Resets all registered resettable objects. (internal only)
     */
    void _rc_reset();

    /**
     * @brief Checks for compatibility with the given port map. (internal only)
     */
    int _rc_is_compatible(const rc_portmap_base& portmap) const;

    /**
     * @brief Searches for a compatible, registered port map. (internal only)
     */
    rc_portmap_base* _rc_get_compatible_portmap(
        const rc_switch_connector_base& switch_conn) const;

private:
    /**
     * @brief Vector containing all existing reconfigurable objects.
     */
    static std::vector<rc_reconfigurable*> s_reconfigurables;

private:
    /**
     * @brief The sc_object.
     *
     * Stores the pointer to a sc_object base class (if exists)
     */
    mutable sc_object*    p_sc_object;
    /**
     * Stores true, if no sc_object base class could be found.
     */
    mutable bool          p_is_no_sc_object;

    /**
     * @brief The current state.
     */
    state_type            p_state;
    /**
     * @brief The next state.
     *
     * Stores the scheduled successive state while a reconfiguration operation
     * is in progress.
     */
    state_type            p_next_state;
    /**
     * @brief The current transaction count.
     *
     * \note Is always greater than zero.
     */
    int                   p_transaction_count;

    /**
     * @brief The process control object.
     *
     * Resettable processes are canceled by this object in case of the
     * deactivation of this rc_reconfigurable.
     */
    rc_process_control    p_pctrl;
    /**
     * @brief Delta cycle synchroniser object for activation/deactivation.
     */
    rc_delta_sync_object  p_delta_sync;

    switch_commobj_map    p_switch_commobj_map;
    switch_commobj_index& p_switch_commobj_index;
    commobj_switch_index& p_commobj_switch_index;

    filter_set            p_filter_set;
    commobj_filters_map   p_commobj_filters_map;

    rc_control*           p_control;
    rc_reconfigurable_set p_self_set;
    rc_mutex_object       p_mutex;
    sc_process_handle     p_lock_share;
    sc_time               p_action_default_delays[ACTION_COUNT];
    sc_time               p_action_delays[ACTION_COUNT];

    portmap_vector        p_portmap_vector;
    switch_conn_type*     p_curr_switch_conn;

    resettable_set        p_resettable_set;
};

/**
 * @brief This class represents an open transaction.
 * 
 * The existence of this class represents an open transaction.
 * The associated rc_reconfigurable's transaction is increased by one.
 * The count is decreased in the destructor.
 * rc_transaction can be used as a temporary object.
 *
 * begin() and end() are provided to allow the use of rc_transaction
 * in cases where it is not used as a temporary object.
 *
 * \remark Objects of this class may also be used in a non-reconfigurable
 * context. The transaction will be simply ignored in this case and
 * therefore has no effect.
 *
 * \see rc_reconfigurable, rc_reconfigurable::rc_begin_transaction(),
 *      rc_reconfigurable::rc_end_transaction()
 */
class rc_transaction
{
public:
    /**
     * @brief Constructor.
     * @param reconf the reconfigurable the transaction shall be associated to
     */
    inline explicit rc_transaction(rc_reconfigurable* reconf);
    /**
     * @brief Begin transaction.
     * \remark Is not cumulative: At most one transaction can be begun.
     */
    inline void begin();
    /**
     * @brief End transaction.
     * \remark Is not cumulative: Only a transaction previously begun by this
     * object can be ended.
     */
    inline void end();
    /**
     * @brief Has the transaction already been ended?
     */
    inline bool has_ended() const
        { return p_has_ended; }
    /**
     * @brief Destructor.
     */
    ~rc_transaction();
private:
    // disabled
    /** @brief (disabled) */
    rc_transaction(const rc_transaction& orig);
    /** @brief (disabled) */
    rc_transaction& operator=(const rc_transaction& orig);
private:
    /**
     * @brief The associated reconfigurable context.
     */
    rc_reconfigurable* reconf;
    /**
     * @brief The transaction ended flag.
     */
    bool p_has_ended;
};

/* inline code */

inline void rc_reconfigurable::rc_end_transaction()
{
    if (p_transaction_count > 1) {
        --p_transaction_count;
    } else {
        this->reset_transaction_count();
    }
}

inline rc_transaction::rc_transaction(rc_reconfigurable* reconf)
    : reconf(reconf), p_has_ended(false)
{
    if (reconf != NULL) {
        reconf->rc_begin_transaction();
    }
    // p_has_ended must initially be set to false (see RC_TRANSACTION)
}

inline void rc_transaction::begin()
{
    if (p_has_ended && reconf != NULL) {
        reconf->rc_begin_transaction();
    }
    // this must always be set, to allow RC_TRANSACTION to work properly
    p_has_ended = false;
}

inline void rc_transaction::end()
{
    if (!p_has_ended && reconf != NULL) {
        reconf->rc_end_transaction();
        reconf->rc_possible_deactivation();
    }
    // this must always be set, to allow RC_TRANSACTION to work properly
    p_has_ended = true;
}

/* global functions */

/**
 * @brief Determines the current reconfigurable context.
 *
 * May be called during construction and simulation phase.
 * During construction phase the context will be the nearest enclosing
 * rc_reconfigurable or rc_reconfigurable_module respectively.
 * During simulation phase the context is determined starting the search at
 * the position of the calling process in the object hierarchy.
 *
 * \note There is also a variant of rc_get_reconfigurable_context()
 * with one parameter specifying the start of the search for the context.
 *
 * \see \ref sec_reconf_context, ReChannel::rc_reconfigurable,
 * ReChannel::internals::reconfigurable::begin_construction
 */
extern rc_reconfigurable* rc_get_reconfigurable_context();

/**
 * @brief Determines the current reconfigurable context.
 *
 * May be called during construction and simulation phase.
 * During construction phase the context will be the nearest enclosing
 * The start of the search is specified (parameter start_search), which allows
 * to find the reconfigurable context during the simulation phase.
 *
 * \remark The position of the calling process in the object hierarchy will
 * not be automatically taken as the start of the search. To achieve this
 * use the variant of rc_get_reconfigurable_context() without a parameter.
 *
 * \see \ref sec_reconf_context, ReChannel::rc_reconfigurable,
 *  rc_get_reconfigurable_context(),
 *  ReChannel::internals::reconfigurable::begin_construction
 */
extern rc_reconfigurable* rc_get_reconfigurable_context(
    sc_object* start_search);

/**
 * @brief Automatically registers a resettable component to
 *  with the current reconfigurable context.
 *
 * \remark The reconfigurable context is determined by the help of function
 * rc_get_reconfigurable_context().
 * \see ReChannel::rc_resettable, ReChannel::rc_reconfigurable
 */
extern const rc_reconfigurable* rc_register_resettable(
    rc_resettable& resettable, sc_object* search_start);

} // namespace ReChannel

/**
 * @brief Convenience macro. Equals rc_reconfigurable::LOAD.
 * \see ReChannel::rc_reconfigurable::action_type
 */
#define RC_LOAD       rc_reconfigurable::LOAD
/**
 * @brief Convenience macro. Equals rc_reconfigurable::ACTIVATE.
 * \see ReChannel::rc_reconfigurable::action_type
 */
#define RC_ACTIVATE   rc_reconfigurable::ACTIVATE
/**
 * @brief Convenience macro. Equals rc_reconfigurable::DEACTIVATE.
 * \see ReChannel::rc_reconfigurable::action_type
 */
#define RC_DEACTIVATE rc_reconfigurable::DEACTIVATE
/**
 * @brief Convenience macro. Equals rc_reconfigurable::UNLOAD.
 * \see ReChannel::rc_reconfigurable::action_type
 */
#define RC_UNLOAD     rc_reconfigurable::UNLOAD

/**
 * @brief Convenience macro. Equals rc_reconfigurable::ACTIVE.
 * \see ReChannel::rc_reconfigurable::state_type
 */
#define RC_ACTIVE   rc_reconfigurable::ACTIVE
/**
 * @brief Convenience macro. Equals rc_reconfigurable::INACTIVE.
 * \see ReChannel::rc_reconfigurable::state_type
 */
#define RC_INACTIVE rc_reconfigurable::INACTIVE
/**
 * @brief Convenience macro. Equals rc_reconfigurable::UNLOADED.
 * \see ReChannel::rc_reconfigurable::state_type
 */
#define RC_UNLOADED rc_reconfigurable::UNLOADED

/**
 * @brief Declares the rc_on_load() callback method.
 * 
 * This convenience macro can be invoked within classes that implement
 * interface rc_reconfigurable. It simplifies the notation of the virtual
 * rc_on_load() callback method within class declarations.
 *
 * Example:
 * \code
 * RC_ON_LOAD() {
 *     [...] // user-defined code (executed after loading finished)
 * }
 * \endcode
 *
 * \see ReChannel::rc_reconfigurable
 */
#define RC_ON_LOAD() \
        virtual void rc_on_load()
/**
 * @brief Declares the rc_on_activate() callback method.
 * 
 * This convenience macro can be invoked within classes that implement
 * interface rc_reconfigurable. It simplifies the notation of the virtual
 * rc_on_activate() callback method within class declarations.
 *
 * Example:
 * \code
 * RC_ON_ACTIVATE() {
 *     [...] // user-defined code (executed after activation)
 * }
 * \endcode
 *
 * \see ReChannel::rc_reconfigurable
 */
#define RC_ON_ACTIVATE() \
        virtual void rc_on_activate()
/**
 * @brief Declares the rc_on_deactivate() callback method.
 * 
 * This convenience macro can be invoked within classes that implement
 * interface rc_reconfigurable. It simplifies the notation of the virtual
 * rc_on_deactivate() callback method within class declarations.
 *
 * Example:
 * \code
 * RC_ON_DEACTIVATE() {
 *     [...] // user-defined code (executed after deactivation)
 * }
 * \endcode
 *
 * \see ReChannel::rc_reconfigurable
 */
#define RC_ON_DEACTIVATE() \
        virtual void rc_on_deactivate()
/**
 * @brief Declares the rc_on_unload() callback method.
 * 
 * This convenience macro can be invoked within classes that implement
 * interface rc_reconfigurable. It simplifies the notation of the virtual
 * rc_on_unload() callback method within class declarations.
 *
 * Example:
 * \code
 * RC_ON_UNLOAD() {
 *     [...] // user-defined code (executed after unloading finished)
 * }
 * \endcode
 *
 * \see ReChannel::rc_reconfigurable
 */
#define RC_ON_UNLOAD() \
        virtual void rc_on_unload()

#endif //RC_RECONFIGURABLE_H_

//
// $Id: rc_reconfigurable.h,v 1.22 2008/01/01 13:34:00 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_reconfigurable.h,v $
//
