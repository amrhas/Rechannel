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
 * @file    rc_process_support.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/10/24 23:55:23 $
 * $Revision: 1.11 $
 */

#ifndef RC_PROCESS_SUPPORT_H_
#define RC_PROCESS_SUPPORT_H_

#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/core/rc_non_reconfigurable.h"
#include "ReChannel/core/rc_reset_channel_if.h"

namespace ReChannel {

class rc_process_reset;

namespace internals {

/**
 * @brief This class provides support for resettable processes.
 *
 * Resettable processes can only be used if a process support
 * object is available at a modules scope.
 *
 * The macro RC_HAS_PROCESS() can be invoked within resettable modules
 * to add an process_support object as a member of this module.
 * Resettable processes may now be declared by the respective process
 * declaration macros.
 * Furthermore rc_spawn() will be available within that module.
 *
 * \remark Only modules that implement rc_resettable can be
 * augmented with the support for resettable processes. Otherwise a
 * runtime error will be reported.
 *
 * \see RC_HAS_PROCESS(), RC_THREAD(), RC_METHOD(), RC_CTHREAD()
 */
class process_support
    : public sc_prim_channel
{
private:
    class process_info;

private:
    typedef rc_hash_map<sc_object*, process_info*> process_info_map;

public:
    process_support();

    inline bool has_reconfigurable_context() const
        { return (&p_reconf != &rc_non_reconfigurable::get_instance()); }

    inline bool has_non_reconfigurable_context() const
        { return (&p_reconf == &rc_non_reconfigurable::get_instance()); }

    inline rc_reconfigurable& get_context() const
        { return p_reconf; }

    inline rc_reconfigurable* get_reconfigurable_context() const;

    void declare_process(
        const sc_object& host,
        const boost::function<void (void)>& declare_proc_callback,
        const boost::function<void (void)>& func, sc_sensitive& sensitive,
        bool is_thread=false, bool dont_initialize=false,
        bool has_sync_reset=false);

    void method_process();

    void thread_process();

    void dont_initialize(const sc_object& host) const;

    // spawns a new, reconfigurable process
     rc_process_handle rc_spawn(
        boost::function<void (void)> func, const char* name_p=NULL,
        rc_spawn_options* opt_p=NULL);

    void terminate();

    bool reset_signal_is(
        const sc_object& host, const sc_in<bool>& port_,
        bool active_level);

    bool reset_signal_is(
        const sc_object& host, const sc_signal<bool>& signal_,
        bool active_level);

    const sc_signal<bool>* reset_channel_is(
        const sc_object& host, rc_reset_channel_if& reset_,
        bool active_level);

    void reset_is(const sc_object& host, rc_process_reset& process_reset);

    void set_sync_reset(const sc_object& host, bool b) const;

    rc_process_behavior_change disable_process_control() const;

protected:
    void before_end_of_elaboration();

private:
    process_info* _rc_find_process_info(sc_object* proc_obj) const;

    process_info* _rc_fetch_process_info(sc_object* proc_obj);

    bool _rc_check_valid_last_proc(const sc_object& host) const;

private:
    static rc_reconfigurable& _rc_find_context(sc_object* start_search);

    static inline void register_process_control(
        const rc_reset_channel_if& reset_channel,
        rc_process_control& pctrl, bool active_level)
        { reset_channel.register_process_control(pctrl, active_level); }

    static inline void unregister_process_control(
        const rc_reset_channel_if& reset_channel,
        rc_process_control& pctrl)
        { reset_channel.unregister_process_control(pctrl); }

    static inline bool get_current_level(
        const rc_reset_channel_if& reset_channel)
        { return reset_channel.get_current_level(); }

private:
    rc_reconfigurable& p_reconf;
    process_info_map   p_reconf_pinfo_map; // TODO: static hashmap?
    process_info*      p_last_pinfo;
    const sc_object*   p_last_host;
    unsigned int       p_last_host_child_count;
};

/**
 * @brief This class is used to store additional process information
 * (internally used).
 */
class process_support::process_info
{
private:
    typedef std::vector<const rc_reset_channel_if*> reset_channel_vector;
    typedef std::vector<const sc_in<bool>*>         reset_port_vector;
    typedef internals::reset_signal                 reset_signal;
    typedef std::map<const sc_signal_in_if<bool>*, reset_signal*>
                                                    reset_signal_map;
public:
    process_info(
        boost::function<void (void)> func_,
        const rc_process_control& parent_pctrl, bool is_thread_=false,
        bool is_dont_initialize=false, bool has_sync_reset=false);

    inline bool is_thread() const
        { return p_is_thread; }

    inline bool is_dont_initialize() const
        { return p_is_dont_initialize; }

    inline void set_dont_initialize(bool b)
        { p_is_dont_initialize = b; }

    inline bool has_sync_reset() const
        { return process_control.is_deactivate_silently(); }

    inline void set_sync_reset(bool b)
        { process_control.set_deactivate_silently(b); }

    inline bool is_init_run() const
        { return p_is_init_run; }

    inline void set_init_run(bool b)
        { p_is_init_run = b; }

    void register_reset_channel(
        const rc_reset_channel_if& reset_channel, bool active_level);

    void register_reset_interface(
        const sc_signal_in_if<bool>& reset_interface, bool active_level);

    void register_reset_signal(
        const sc_signal<bool>& reset_signal, bool active_level);

    void register_reset_port(
        const sc_in<bool>& reset_port, bool active_level);

    void register_reset_port_interfaces();

    void update_process_control_state();

    ~process_info();

private:
    static const rc_reset_channel_if& _rc_get_reset_channel(
        const sc_signal_in_if<bool>& signal_interface);

    static void _rc_reset_updater_proc(
        const sc_signal_in_if<bool>& from_, reset_signal& to_);

public:
    boost::function<void (void)> func;
    rc_process_control     process_control;

private:
    bool                   p_is_thread          : 1;
    bool                   p_is_dont_initialize : 1;
    bool                   p_is_init_run        : 1;
    reset_channel_vector   p_reset_channels[2];
    reset_port_vector      p_reset_ports[2];

private:
    static reset_signal_map p_reset_signal_map;
};

} // namespace internals

/**
 * @brief This class resets resettable processes in the same delta cycle.
 *
 * rc_process_reset is a reset channel that implements rc_reset_channel_if.
 * In constrast to normal reset signals it is attached to process by
 * method reset_is(). Another difference is that it does not possess an active
 * level. Instead rc_process_reset only provides a trigger() method that can
 * be called to trigger a single discrete reset and, thus, get_current_level()
 * will always return false. The reset will occur in same delta cycle as the
 * trigger call.
 *
 * Example:
 * \code
 * rc_process_reset preset; // declaration at class scope
 * [...]
 * RC_THREAD(proc);  // creation of the thread process
 * reset_is(preset); // proc is reset by preset now
 * [...]
 * preset.trigger(); // trigger the reset (e.g. from another process)
 * \endcode
 */
class rc_process_reset
    : public sc_prim_channel,
      protected rc_reset_channel_if
{
    friend class internals::process_support;

private:
    typedef std::set<rc_process_control*> pctrl_set;

public:
    explicit rc_process_reset();

    explicit rc_process_reset(const char* name_);

public:
    /**
     * @brief Trigger a reset on this object.
     */
    virtual void trigger();

protected:
    // registers a process control at the reset list
    virtual void register_process_control(
        rc_process_control& pctrl, bool active_level) const;

    // unregisters a process control from the reset list
    virtual void unregister_process_control(
        rc_process_control& pctrl) const;

    // always returns the inactive level: 'false'
    virtual bool get_current_level() const
        { return false; }

    // always returns NULL
    virtual const sc_signal<bool>* get_underlying_reset_signal() const
        { return NULL; }

private:
    mutable pctrl_set p_pctrl_set;
};

/* inline code */

namespace internals {

inline
rc_reconfigurable* process_support::get_reconfigurable_context() const
{
    if (&p_reconf != &rc_non_reconfigurable::get_instance()) {
        return &p_reconf;
    } else {
        return NULL;
    }
}

} // namespace internals

} // namespace ReChannel

/* construction and helper macros */

#define RC_HAS_RECONFIGURABLE_PROCESS(user_module_name) \
    SC_HAS_PROCESS(user_module_name); \
    private: \
        internals::process_support \
            _rc_process_support; \
        void _rc_declare_method() \
            { SC_METHOD(_rc_method_process); } \
        void _rc_declare_thread() \
            { SC_THREAD(_rc_thread_process); } \
        void _rc_method_process() \
            { this->_rc_process_support.method_process(); } \
        void _rc_thread_process() \
            { this->_rc_process_support.thread_process(); } \
    protected: \
        inline void next_trigger() \
            { rc_next_trigger(); } \
        inline void next_trigger(const sc_event& e) \
            { rc_next_trigger(e); } \
        inline void next_trigger(sc_event_or_list& el) \
            { rc_next_trigger(el); } \
        inline void next_trigger(sc_event_and_list& el) \
            { rc_next_trigger(el); } \
        inline void next_trigger(const sc_time& t) \
            { rc_next_trigger(t); } \
        inline void next_trigger(double v, sc_time_unit tu) \
            { rc_next_trigger(v, tu); } \
        inline void next_trigger(const sc_time& t, const sc_event& e) \
            { rc_next_trigger(t, e); } \
        inline void next_trigger( \
            double v, sc_time_unit tu, const sc_event& e) \
            { rc_next_trigger(v, tu, e); } \
        inline void next_trigger(const sc_time& t, sc_event_or_list& el) \
            { rc_next_trigger(t, el); } \
        inline void next_trigger( \
            double v, sc_time_unit t, sc_event_or_list& el) \
            { rc_next_trigger(v, t, el); } \
        inline void next_trigger(const sc_time& t, sc_event_and_list& el) \
            { rc_next_trigger(t, el); } \
        inline void next_trigger( \
            double v, sc_time_unit tu, sc_event_and_list& el) \
            { rc_next_trigger(v, tu, el); } \
        inline void wait() \
            { rc_wait(); } \
        inline void wait(int n) \
            { rc_wait(n); } \
        inline void wait(const sc_event& e) \
            { rc_wait(e); } \
        inline void wait(sc_event_or_list& el) \
            { rc_wait(el); } \
        inline void wait(sc_event_and_list& el) \
            { rc_wait(el); } \
        inline void wait(const sc_time& t) \
            { rc_wait(t); } \
        inline void wait(double v, sc_time_unit tu) \
            { rc_wait(v, tu); } \
        inline void wait(const sc_time& t, const sc_event& e) \
            { rc_wait(t, e); } \
        inline void wait(double v, sc_time_unit tu, const sc_event& e) \
            { rc_wait(v, tu, e); } \
        inline void wait(const sc_time& t, sc_event_or_list& el) \
            { rc_wait(t, el); } \
        inline void wait(double v, sc_time_unit t, sc_event_or_list& el) \
            { rc_wait(v, t, el); } \
        inline void wait(const sc_time& t, sc_event_and_list& el) \
            { rc_wait(t, el); } \
        inline void wait( \
            double v, sc_time_unit tu, sc_event_and_list& el) \
            { rc_wait(v, tu, el); } \
        template<class _rc_T_param> \
        inline rc_process_handle rc_spawn( \
            _rc_T_param object, const char* name_p=NULL, \
            rc_spawn_options* opt_p=NULL) \
        { \
            return this->_rc_process_support.rc_spawn( \
                object, name_p, opt_p); \
        } \
        inline bool rc_is_reconfigurable() const \
        { \
            return this->_rc_process_support.has_reconfigurable_context(); \
        } \
        inline void rc_terminate() \
            { this->_rc_process_support.terminate(); } \
    public: \
        inline void dont_initialize() \
        { \
            this->_rc_process_support.dont_initialize(*this); \
            sc_module::dont_initialize(); \
        } \
        inline void reset_signal_is( \
            const sc_in<bool>& reset_port, bool active_level) \
        { \
            bool success = this->_rc_process_support.reset_signal_is( \
                *this, reset_port, active_level); \
            if (!success) { \
                sc_module::reset_signal_is(reset_port, active_level); \
            } \
        } \
        inline void reset_signal_is( \
            const sc_signal<bool>& reset_signal, bool active_level) \
        { \
            bool success = this->_rc_process_support.reset_signal_is( \
                *this, reset_signal, active_level); \
            if (!success) { \
                sc_module::reset_signal_is(reset_signal, active_level); \
            } \
        } \
        inline void reset_signal_is( \
            rc_reset_channel_if& reset_channel, bool active_level) \
        { \
            const sc_signal<bool>* reset_signal = \
                this->_rc_process_support.reset_channel_is( \
                    *this, reset_channel, active_level); \
            if (reset_signal != NULL) { \
                sc_module::reset_signal_is(*reset_signal, active_level); \
            } \
        } \
        inline void reset_is(rc_process_reset& process_reset) \
            { this->_rc_process_support.reset_is(*this, process_reset); } \
        inline void rc_set_sync_reset() \
            { this->_rc_process_support.set_sync_reset(*this, true); } \
        inline void rc_set_async_reset() \
            { this->_rc_process_support.set_sync_reset(*this, false); } \
        typedef user_module_name RC_CURRENT_USER_MODULE

#define RC_RECONFIGURABLE_METHOD(func) \
    if (true) { \
        if (this->_rc_process_support.has_reconfigurable_context()) { \
            this->_rc_process_support.declare_process( \
                *this, \
                boost::bind( \
                    &RC_CURRENT_USER_MODULE::_rc_declare_method, this), \
                boost::bind( \
                    &RC_CURRENT_USER_MODULE::func, this), \
                this->sensitive); \
        } else { \
            SC_METHOD(func); \
        } \
    } else (void)0

#define RC_RECONFIGURABLE_THREAD(func) \
    if (true) { \
        this->_rc_process_support.declare_process( \
            *this, \
            boost::bind( \
                &RC_CURRENT_USER_MODULE::_rc_declare_thread, this), \
            boost::bind( \
                &RC_CURRENT_USER_MODULE::func, this), \
            this->sensitive, true, false, false); \
    } else (void)0

#define RC_RECONFIGURABLE_CTHREAD(func, edge) \
    if (true) { \
        this->_rc_process_support.declare_process( \
            *this, \
            boost::bind( \
                &RC_CURRENT_USER_MODULE::_rc_declare_thread, this), \
            boost::bind( \
                &RC_CURRENT_USER_MODULE::func, this), \
            this->sensitive, true, true, true); \
        this->sensitive << edge; \
    } else (void)0

#define RC_NO_RECONFIGURABLE_PROCESS_RESET(pbc_name) \
    for(rc_process_behavior_change pbc_name = \
        this->_rc_process_support.disable_process_control(); \
        !pbc_name.is_released(); \
        pbc_name.release())

#define RC_HAS_PROCESS(user_module_name) \
    RC_HAS_RECONFIGURABLE_PROCESS(user_module_name)

#define RC_METHOD(func) \
    RC_RECONFIGURABLE_METHOD(func)

#define RC_THREAD(func) \
    RC_RECONFIGURABLE_THREAD(func)

#define RC_CTHREAD(func, edge) \
    RC_RECONFIGURABLE_CTHREAD(func, edge)

#define RC_NO_RESET \
    RC_NO_RECONFIGURABLE_PROCESS_RESET(_rc_tmp_pbc)

#define _RC_DECLARE_TRANSACTION \
    for(rc_transaction _rc_tmp_trans( \
        this->_rc_process_support.get_reconfigurable_context()); \
        !_rc_tmp_trans.has_ended(); \
        _rc_tmp_trans.end())

#define RC_TRANSACTION _RC_DECLARE_TRANSACTION

#endif // RC_PROCESS_SUPPORT_H_

//
// $Id: rc_process_support.h,v 1.11 2007/10/24 23:55:23 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_process_support.h,v $
//
