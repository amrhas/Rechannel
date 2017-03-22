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
 * @file    rc_process.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/06 11:42:43 $
 * $Revision: 1.15 $
 */

#ifndef RC_PROCESS_H_
#define RC_PROCESS_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/util/rc_hash_map.h"

namespace ReChannel {

// you may decrease stack size to 1KB (or even lower) if your OS allows
#ifndef RC_PROCESS_TEMP_PROC_STACK_SIZE
 #define RC_PROCESS_TEMP_PROC_STACK_SIZE 16384
#endif // RC_PROCESS_TEMP_PROC_STACK_SIZE

class rc_process_handle;
class rc_process_behavior_change;
class rc_process_cancel_exception;
class rc_spawn_options;

class rc_reset_channel_if;
class rc_process_reset;

namespace internals {
class process_support;
} // namespace internals

/**
 * @brief Controls the cancellation of resettable processes.
 */
class rc_process_control
{
    friend class rc_process_registry;
    friend class rc_process_behavior_change;

private:
    typedef std::set<rc_process_control*> pctrl_set;

public:
    explicit rc_process_control(
        bool is_initially_active=false, bool deactivate_silently=false);

    explicit rc_process_control(
        const rc_process_control& parent,
        bool is_initially_active=false, bool deactivate_silently=false);

    void activate();

    void deactivate();

    inline bool is_active() const
        { return !p_cancel_flag; }

    inline bool is_deactivation_event() const
        { return (p_cancel_delta == sc_delta_count()); }

    inline const sc_event& get_deactivation_event() const
        { return p_cancel_event; }

    inline bool is_activation_event() const
        { return (p_activation_delta == sc_delta_count()); }

    inline const sc_event& get_activation_event() const
        { return p_activation_event; }

    inline bool is_deactivate_silently() const
        { return p_deactivate_silently; }

    inline void set_deactivate_silently(bool b)
        { p_deactivate_silently = b; }

    inline bool has_parent() const
        { return (p_parent != NULL); }

    inline const rc_process_control* get_parent() const
        { return p_parent; }

    ~rc_process_control();

private:
    sc_event      p_activation_event;
    sc_dt::uint64 p_activation_delta;

    sc_event      p_cancel_event;
    bool          p_cancel_flag;
    sc_dt::uint64 p_cancel_delta;

    bool          p_deactivate_silently;

    const rc_process_control* p_parent;
    mutable pctrl_set         p_child_set;

private:
    // disabled
    rc_process_control& operator=(const rc_process_control& orig);
};

/**
 * @brief Internal struct used by the process registry.
 */
struct rc_process_info
{
    unsigned int         ref_count;
    sc_process_handle    handle;
    bool                 dont_initialize : 1;
    bool                 is_cancelable   : 1;
    bool                 is_terminated   : 1;
    bool                 is_temporary    : 1;
    const sc_event*      cancel_event;
    const bool*          cancel_flag;
    const sc_dt::uint64* cancel_delta;
    sc_event*            terminated_event;
};

/**
 * @brief ReChannel's representation of sc_process_handle.
 */
class rc_process_handle
{
    friend rc_process_handle rc_get_current_process_handle();

    friend void rc_next_trigger();
    friend void rc_next_trigger(const sc_event&);
    friend void rc_next_trigger(sc_event_or_list&);
    friend void rc_next_trigger(sc_event_and_list&);
    friend void rc_next_trigger(const sc_time&);
    friend void rc_next_trigger(double v, sc_time_unit tu);
    friend void rc_next_trigger(const sc_time&, const sc_event&);
    friend void rc_next_trigger(double, sc_time_unit, const sc_event&);
    friend void rc_next_trigger(const sc_time&, sc_event_or_list&);
    friend void rc_next_trigger(double, sc_time_unit, sc_event_or_list&);
    friend void rc_next_trigger(const sc_time&, sc_event_and_list&);
    friend void rc_next_trigger(double, sc_time_unit, sc_event_and_list&);

    friend void rc_wait();
    friend void rc_wait(const sc_event&);
    friend void rc_wait(sc_event_or_list&);
    friend void rc_wait(sc_event_and_list&);
    friend void rc_wait(const sc_time&);
    friend void rc_wait(double v, sc_time_unit tu);
    friend void rc_wait(const sc_time&, const sc_event&);
    friend void rc_wait(double, sc_time_unit, const sc_event&);
    friend void rc_wait(const sc_time&, sc_event_or_list&);
    friend void rc_wait(double, sc_time_unit, sc_event_or_list&);
    friend void rc_wait(const sc_time&, sc_event_and_list&);
    friend void rc_wait(double, sc_time_unit, sc_event_and_list&);

    friend bool operator==(
        const rc_process_handle& left, const rc_process_handle& right);
    friend bool operator!=(
        const rc_process_handle& left, const rc_process_handle& right);

    friend class rc_process_behavior_change;
    friend class rc_process_registry;
    friend class internals::process_support;

public:
    inline rc_process_handle();
    inline rc_process_handle(const rc_process_handle& hproc);
    inline rc_process_handle(const sc_process_handle& hproc);
    inline ~rc_process_handle();

    inline rc_process_handle& operator=(const rc_process_handle& hproc);

public:
    inline bool is_dont_initialize() const;
    inline sc_process_handle get_sc_process_handle() const;
    inline operator sc_process_handle() const;
    inline bool is_cancelable() const;
    inline bool is_canceled() const;
    inline bool is_cancel_event() const;
    inline bool is_terminatable() const;
    inline bool is_temporary() const;

    inline rc_process_behavior_change behavior_change(
        bool is_cancelable);

    inline rc_process_behavior_change behavior_change(
        const rc_process_control& pctrl);

    /*inline rc_process_behavior_change behavior_change(
        bool is_cancelable, const sc_event* cancel_event,
        const bool* cancel_flag, const sc_dt::uint64* cancel_delta);*/

/* members for sc_process_handle compatibility */

    inline bool valid() const;
    inline const char* name() const;
    inline sc_curr_proc_kind proc_kind() const;
    inline const std::vector<sc_object*>& get_child_objects() const;
    inline sc_object* get_parent_object() const;
    inline sc_object* get_process_object() const;
    inline bool dynamic() const;
    inline bool terminated() const;
    inline const sc_event& terminated_event() const;

private:
    inline rc_process_handle(rc_process_info* proc_info);
    inline rc_process_handle(rc_process_info& proc_info);

    inline const sc_event& get_cancel_trigger_event() const;

/* special members called by class process_support */

    // note: calling these methods requires a handle on the current process
    void wait_terminated(const sc_event& reactivation_event);
    bool terminate();
    inline void notify_terminated_event();

private:
    rc_process_info* p_proc_info;

    static const sc_event s_never_notified;
};

/**
 * @brief ReChannel's process registry.
 */
class rc_process_registry
{
    friend rc_process_registry& rc_get_process_registry();
    friend rc_process_handle rc_get_current_process_handle();

    friend void rc_next_trigger();
    friend void rc_next_trigger(const sc_event&);
    friend void rc_next_trigger(sc_event_or_list&);
    friend void rc_next_trigger(sc_event_and_list&);
    friend void rc_next_trigger(const sc_time&);
    friend void rc_next_trigger(double v, sc_time_unit tu);
    friend void rc_next_trigger(const sc_time&, const sc_event&);
    friend void rc_next_trigger(double, sc_time_unit, const sc_event&);
    friend void rc_next_trigger(const sc_time&, sc_event_or_list&);
    friend void rc_next_trigger(double, sc_time_unit, sc_event_or_list&);
    friend void rc_next_trigger(const sc_time&, sc_event_and_list&);
    friend void rc_next_trigger(double, sc_time_unit, sc_event_and_list&);

    friend void rc_wait();
    friend void rc_wait(const sc_event&);
    friend void rc_wait(sc_event_or_list&);
    friend void rc_wait(sc_event_and_list&);
    friend void rc_wait(const sc_time&);
    friend void rc_wait(double v, sc_time_unit tu);
    friend void rc_wait(const sc_time&, const sc_event&);
    friend void rc_wait(double, sc_time_unit, const sc_event&);
    friend void rc_wait(const sc_time&, sc_event_or_list&);
    friend void rc_wait(double, sc_time_unit, sc_event_or_list&);
    friend void rc_wait(const sc_time&, sc_event_and_list&);
    friend void rc_wait(double, sc_time_unit, sc_event_and_list&);

    friend class rc_process_handle;
    friend class tmp_behavior_modification;
    friend class tmp_behavior_modification_simple;
    friend class internals::process_support;

private:
    enum registry_options { gc_interval=100 };

protected:
    typedef rc_hash_map<sc_object*, rc_process_info> process_map;

public:
    // returns the number of the current entries in the registry
    inline unsigned int get_process_count() const
        { return p_process_map.size(); }

    // deletes physically terminated processes from the registry
    void gc();

protected:
    ~rc_process_registry()
        { rc_process_registry::s_instance = NULL; }

private:
    rc_process_registry();

    rc_process_handle get_process_handle(
        const sc_process_handle& hproc, bool implicit_creation=true);

    rc_process_handle register_process(
        const sc_process_handle& hproc);

    rc_process_handle register_process(
        const sc_process_handle& hproc, bool dont_initialize,
        bool is_cancelable, const rc_process_control& pctrl,
        bool is_temporary=false);

    rc_process_handle register_process(
        const sc_process_handle& hproc, bool dont_initialize,
        bool is_cancelable, const sc_event* cancel_event,
        const bool* cancel_flag, const sc_dt::uint64* cancel_delta,
        bool is_temporary=false);

private:
    static void _rc_wait_event_and_list_proc(
        sc_event_and_list& el, sc_event* continue_event);

    static void _rc_wait_event_and_list_proc(
        const sc_time& t, sc_event_and_list& el,
        sc_event* continue_event);

    static void _rc_create_singleton();

private:
    process_map p_process_map;

    rc_process_handle p_cached_hproc;

private:
    static rc_process_registry* s_instance;

private:
    // disabled
    rc_process_registry(const rc_process_registry& orig);
    rc_process_registry& operator=(const rc_process_registry& orig);
};

inline
rc_process_registry& rc_get_process_registry()
{
    if (rc_process_registry::s_instance == NULL) {
        rc_process_registry::_rc_create_singleton();
    }
    return *(rc_process_registry::s_instance);
}

class rc_process_behavior_change
{
    friend class rc_process_handle;

public:
    inline rc_process_behavior_change();

    inline rc_process_behavior_change(
        const rc_process_behavior_change& pbc);

    inline ~rc_process_behavior_change()
        { this->release(); }

    inline bool is_released() const
        { return (p_proc_info == NULL); }

    inline void release();

private:
    inline rc_process_behavior_change(
        rc_process_info& proc_info, bool is_cancelable);

    inline rc_process_behavior_change(
        rc_process_info& proc_info, bool is_cancelable,
        const rc_process_control& pctrl);

    inline rc_process_behavior_change(
        rc_process_info& proc_info, bool is_cancelable,
        const sc_event* cancel_event, const bool* cancel_flag,
        const sc_dt::uint64* cancel_delta);

    inline rc_process_behavior_change& operator=(
        const rc_process_behavior_change& pbc);

private:
    mutable rc_process_info* p_proc_info;
    bool                     p_orig_is_cancelable;
    const sc_event*          p_orig_cancel_event;
    const bool*              p_orig_cancel_flag;
    const sc_dt::uint64*     p_orig_cancel_delta;
    sc_event*                p_orig_terminated_event;
};

/**
 * @brief Exception used to cancel a resettable process.
 */
class rc_process_cancel_exception
    : public rc_throwable
{
public:
    virtual void rc_throw()
        { throw this; }
};

/**
 * @brief ReChannel's representation of sc_spawn_options (+reset).
 */
class rc_spawn_options
    : private sc_spawn_options
{
    friend class internals::process_support;

private:
    typedef std::vector<std::pair<const rc_reset_channel_if*, bool> >
        reset_channel_vector;
    typedef std::vector<std::pair<const sc_in<bool>*, bool> >
        reset_port_vector;
    typedef std::vector<std::pair<const sc_signal<bool>*, bool> >
        reset_signal_vector;
    typedef std::vector<rc_process_reset*> process_reset_vector;

public:
    rc_spawn_options();

public:
    inline void spawn_method()
        { sc_spawn_options::spawn_method(); }
    inline bool is_method() const
        { return sc_spawn_options::is_method(); }
    inline void dont_initialize()
    {
        sc_spawn_options::dont_initialize();
        p_is_dont_initialize = true;
    }
    inline bool is_dont_initialize() const
        { return p_is_dont_initialize; }
    inline void set_stack_size(int stack_size)
        { sc_spawn_options::set_stack_size(stack_size); }
    inline void set_sensitivity(const sc_event* event_)
        { sc_spawn_options::set_sensitivity(event_); }
    inline void set_sensitivity(sc_port_base* port_)
        { sc_spawn_options::set_sensitivity(port_); }
    inline void set_sensitivity(sc_export_base* export_)
        { sc_spawn_options::set_sensitivity(export_); }
    inline void set_sensitivity(sc_interface* if_)
        { sc_spawn_options::set_sensitivity(if_); }
    inline void set_sensitivity(sc_event_finder* ef_)
        { sc_spawn_options::set_sensitivity(ef_); }
    void set_reset_signal(const sc_in<bool>& reset_port_, bool level_);
    void set_reset_signal(
        const sc_signal<bool>& reset_signal_, bool level_);
    void set_reset_signal(
        rc_reset_channel_if& reset_channel_, bool level_);
    void set_reset(rc_process_reset& process_reset_);

private:
    inline reset_port_vector& get_reset_port_vector()
        { return p_reset_port_vector; }
    inline reset_signal_vector& get_reset_signal_vector()
        { return p_reset_signal_vector; }
    inline reset_channel_vector& get_reset_channel_vector()
        { return p_reset_channel_vector; }
    inline process_reset_vector& get_process_reset_vector()
        { return p_process_reset_vector; }

private:
    bool                 p_is_dont_initialize;
    reset_channel_vector p_reset_channel_vector;
    reset_signal_vector  p_reset_signal_vector;
    reset_port_vector    p_reset_port_vector;
    process_reset_vector p_process_reset_vector;

private:
    // disabled
    rc_spawn_options(const rc_spawn_options&);
    rc_spawn_options& operator=(const rc_spawn_options&);
};

/* inline code */

inline rc_process_handle::rc_process_handle()
    : p_proc_info(NULL)
{ }

inline
rc_process_handle::rc_process_handle(rc_process_info* proc_info)
{
    p_proc_info = proc_info;
    if (proc_info != NULL) { ++proc_info->ref_count; }
}

inline
rc_process_handle::rc_process_handle(rc_process_info& proc_info)
{
    p_proc_info = &proc_info;
    ++proc_info.ref_count;
}

inline rc_process_handle::rc_process_handle(const rc_process_handle& hproc)
{
    p_proc_info = hproc.p_proc_info;
    if (p_proc_info != NULL) { ++p_proc_info->ref_count; }
}

inline rc_process_handle::rc_process_handle(const sc_process_handle& hproc)
    : p_proc_info(NULL)
{
    rc_process_handle temp =
        rc_get_process_registry().get_process_handle(hproc, true);
    p_proc_info = temp.p_proc_info;
    if (p_proc_info != NULL) { ++p_proc_info->ref_count; }
}

inline rc_process_handle::~rc_process_handle()
{
    if (p_proc_info != NULL) { --p_proc_info->ref_count; }
}

inline
rc_process_handle&
rc_process_handle::operator=(const rc_process_handle& hproc)
{
    if (&hproc != this) {
        if (p_proc_info != NULL) { --p_proc_info->ref_count; }
        p_proc_info = hproc.p_proc_info;
        if (p_proc_info != NULL) { ++p_proc_info->ref_count; }
    }
    return *this;
}

inline bool rc_process_handle::is_dont_initialize() const
{
    if (p_proc_info != NULL) {
        return p_proc_info->dont_initialize;
    } else {
        return false;
    }
}

inline sc_process_handle rc_process_handle::get_sc_process_handle() const
{
    if (p_proc_info != NULL) {
        return p_proc_info->handle;
    } else {
        return sc_process_handle();
    }
}

inline rc_process_handle::operator sc_process_handle() const
{
    return get_sc_process_handle();
}

inline bool rc_process_handle::is_cancelable() const
{
    if (p_proc_info != NULL && p_proc_info->handle.valid()) {
        return p_proc_info->is_cancelable;
    } else {
        return false;
    }
}

inline bool rc_process_handle::is_canceled() const
{
    if (p_proc_info != NULL) {
        if (p_proc_info->terminated_event != NULL
        && p_proc_info->is_terminated)
        {
            return true;
        } else if (p_proc_info->handle.valid()
        && p_proc_info->is_cancelable
        && p_proc_info->cancel_flag != NULL)
        {
            return *(p_proc_info->cancel_flag);
        }
    }
    return false;
}

inline bool rc_process_handle::is_cancel_event() const
{
    return (p_proc_info != NULL && p_proc_info->handle.valid()
        && p_proc_info->is_cancelable
        && *(p_proc_info->cancel_delta) == sc_delta_count());
}

inline const sc_event& rc_process_handle::get_cancel_trigger_event() const
{
    if (p_proc_info != NULL && p_proc_info->handle.valid()
    && p_proc_info->cancel_event != NULL)
    {
        return *p_proc_info->cancel_event;
    } else {
        return s_never_notified;
    }
}

inline
rc_process_behavior_change
rc_process_handle::behavior_change(bool is_cancelable)
{
    if (p_proc_info != NULL && p_proc_info->handle.valid()) {
        return rc_process_behavior_change(
            *p_proc_info, is_cancelable);
    } else {
        return rc_process_behavior_change();
    }
}

inline
rc_process_behavior_change
rc_process_handle::behavior_change(const rc_process_control& pctrl)
{
    if (p_proc_info != NULL && p_proc_info->handle.valid()) {
        return rc_process_behavior_change(
            *p_proc_info, true, pctrl);
    } else {
        return rc_process_behavior_change();
    }
}

/*inline
rc_process_behavior_change
rc_process_handle::behavior_change(
    bool is_cancelable, const sc_event* cancel_event,
    const bool* cancel_flag, const sc_dt::uint64* cancel_delta)
{
    if (p_proc_info != NULL && p_proc_info->handle.valid()) {
        return rc_process_behavior_change(
            *p_proc_info, is_cancelable, cancel_event, cancel_flag,
            cancel_delta);
    } else {
        return rc_process_behavior_change();
    }
}*/

inline
bool rc_process_handle::is_terminatable() const
{
    return (p_proc_info != NULL && p_proc_info->terminated_event != NULL);
}

inline
bool rc_process_handle::is_temporary() const
{
    return (p_proc_info != NULL && p_proc_info->is_temporary);
}

inline bool rc_process_handle::valid() const
{
    return (p_proc_info != NULL && p_proc_info->handle.valid());
}
inline const char* rc_process_handle::name() const
{
    if (p_proc_info != NULL) {
        return p_proc_info->handle.name();
    } else {
        return NULL;
    }
}
inline sc_curr_proc_kind rc_process_handle::proc_kind() const
{
    if (p_proc_info != NULL) {
        return p_proc_info->handle.proc_kind();
    } else {
        return SC_NO_PROC_;
    }
}
inline
const std::vector<sc_object*>&
rc_process_handle::get_child_objects() const
{
    if (p_proc_info != NULL) {
        return p_proc_info->handle.get_child_objects();
    } else {
        return sc_process_handle().get_child_objects();
    }
}
inline sc_object* rc_process_handle::get_parent_object() const
{
    if (p_proc_info != NULL) {
        return p_proc_info->handle.get_parent_object();
    } else {
        return NULL;
    }
}
inline sc_object* rc_process_handle::get_process_object() const
{
    if (p_proc_info != NULL) {
        return p_proc_info->handle.get_process_object();
    } else {
        return NULL;
    }
}

inline bool rc_process_handle::dynamic() const
{
    if (p_proc_info != NULL) {
        return p_proc_info->handle.dynamic();
    } else {
        return false;
    }
}

inline bool rc_process_handle::terminated() const
{
    if (p_proc_info != NULL) {
        const bool is_phys_terminated = p_proc_info->handle.terminated();
        if (p_proc_info->terminated_event != NULL && !is_phys_terminated) {
            return p_proc_info->is_terminated;
        } else {
            return is_phys_terminated;
        }
    } else {
        return false;
    }
}

inline const sc_event& rc_process_handle::terminated_event() const
{
    if (p_proc_info != NULL) {
        if (p_proc_info->terminated_event != NULL
        && !(p_proc_info->handle.terminated())) {
            return *p_proc_info->terminated_event;
        } else {
            return p_proc_info->handle.terminated_event();
        }
    } else {
        return s_never_notified;
    }
}

inline void rc_process_handle::notify_terminated_event()
{
    if (p_proc_info != NULL && p_proc_info->terminated_event != NULL) {
        p_proc_info->terminated_event->notify();
    }
}

inline
rc_process_behavior_change::rc_process_behavior_change()
    : p_proc_info(NULL),
      p_orig_is_cancelable(false), p_orig_cancel_event(NULL),
      p_orig_cancel_flag(NULL), p_orig_cancel_delta(NULL),
      p_orig_terminated_event(NULL)
{ }

inline
rc_process_behavior_change::rc_process_behavior_change(
    rc_process_info& proc_info, bool is_cancelable)
    : p_proc_info(&proc_info),
      p_orig_is_cancelable(proc_info.is_cancelable),
      p_orig_cancel_event(proc_info.cancel_event),
      p_orig_cancel_flag(proc_info.cancel_flag),
      p_orig_cancel_delta(proc_info.cancel_delta),
      p_orig_terminated_event(proc_info.terminated_event)
{
    ++proc_info.ref_count;
    proc_info.is_cancelable = is_cancelable;
    if (is_cancelable == false) {
        proc_info.terminated_event = NULL;
    }
}

inline
rc_process_behavior_change::rc_process_behavior_change(
    rc_process_info& proc_info, bool is_cancelable,
    const rc_process_control& pctrl)
    : p_proc_info(&proc_info),
      p_orig_is_cancelable(proc_info.is_cancelable),
      p_orig_cancel_event(proc_info.cancel_event),
      p_orig_cancel_flag(proc_info.cancel_flag),
      p_orig_cancel_delta(proc_info.cancel_delta),
      p_orig_terminated_event(proc_info.terminated_event)
{
    ++proc_info.ref_count;
    proc_info.is_cancelable = is_cancelable;
    proc_info.cancel_event = &pctrl.p_cancel_event;
    proc_info.cancel_flag = &pctrl.p_cancel_flag;
    proc_info.cancel_delta = &pctrl.p_cancel_delta;
    if (is_cancelable == false) {
        proc_info.terminated_event = NULL;
    }
}

inline
rc_process_behavior_change::rc_process_behavior_change(
    rc_process_info& proc_info, bool is_cancelable,
    const sc_event* cancel_event, const bool* cancel_flag,
    const sc_dt::uint64* cancel_delta)
    : p_proc_info(&proc_info),
      p_orig_is_cancelable(proc_info.is_cancelable),
      p_orig_cancel_event(proc_info.cancel_event),
      p_orig_cancel_flag(proc_info.cancel_flag),
      p_orig_cancel_delta(proc_info.cancel_delta),
      p_orig_terminated_event(proc_info.terminated_event)
{
    ++proc_info.ref_count;
    proc_info.is_cancelable = is_cancelable;
    proc_info.cancel_event = cancel_event;
    proc_info.cancel_flag = cancel_flag;
    proc_info.cancel_delta = cancel_delta;
    if (is_cancelable == false) {
        proc_info.terminated_event = NULL;
    }
}

inline
rc_process_behavior_change::rc_process_behavior_change(
    const rc_process_behavior_change& pbc)
{
    p_proc_info = pbc.p_proc_info;
    p_orig_is_cancelable = pbc.p_orig_is_cancelable;
    p_orig_cancel_event = pbc.p_orig_cancel_event;
    p_orig_cancel_flag = pbc.p_orig_cancel_flag;
    p_orig_cancel_delta = pbc.p_orig_cancel_delta;
    p_orig_terminated_event = pbc.p_orig_terminated_event;
    pbc.p_proc_info = NULL;
}

inline void rc_process_behavior_change::release()
{
    if (p_proc_info != NULL) {
        p_proc_info->is_cancelable = p_orig_is_cancelable;
        p_proc_info->cancel_event = p_orig_cancel_event;
        p_proc_info->cancel_flag = p_orig_cancel_flag;
        p_proc_info->cancel_delta = p_orig_cancel_delta;
        p_proc_info->terminated_event = p_orig_terminated_event;
        --p_proc_info->ref_count;
        p_proc_info = NULL;
    }
}

inline
rc_process_behavior_change& rc_process_behavior_change::operator=(
    const rc_process_behavior_change& pbc)
{
    if (&pbc != this) {
        this->release();
        p_proc_info = pbc.p_proc_info;
        p_orig_is_cancelable = pbc.p_orig_is_cancelable;
        p_orig_cancel_event = pbc.p_orig_cancel_event;
        p_orig_cancel_flag = pbc.p_orig_cancel_flag;
        p_orig_cancel_delta = pbc.p_orig_cancel_delta;
        p_orig_terminated_event = pbc.p_orig_terminated_event;
        pbc.p_proc_info = NULL;
    }
    return *this;
}

/* global functions */

inline bool operator == (
    const rc_process_handle& left_, const rc_process_handle& right_)
{
    return (left_.p_proc_info != NULL) && (right_.p_proc_info != NULL) &&
        (left_.p_proc_info == right_.p_proc_info);
}

inline bool operator != (
    const rc_process_handle& left_, const rc_process_handle& right_)
{
    return (left_.p_proc_info == NULL) || (right_.p_proc_info == NULL) ||
        (left_.p_proc_info != right_.p_proc_info);
}

inline rc_process_handle rc_get_current_process_handle()
{
    const sc_process_handle handle = sc_get_current_process_handle();
    rc_process_registry& preg = rc_get_process_registry();
    if (preg.p_cached_hproc.p_proc_info != NULL
      && preg.p_cached_hproc.p_proc_info->handle
        == handle) {
        return preg.p_cached_hproc;
    } else {
        return preg.get_process_handle(handle);
    }
}

extern void rc_next_trigger();

extern void rc_next_trigger(const sc_event& e);

extern void rc_next_trigger(sc_event_or_list& el);

extern void rc_next_trigger(sc_event_and_list& el);

extern void rc_next_trigger(const sc_time& t);

extern void rc_next_trigger(double v, sc_time_unit tu);

extern void rc_next_trigger(const sc_time& t, const sc_event& e);

extern void rc_next_trigger(double v, sc_time_unit tu, const sc_event& e);

extern void rc_next_trigger(const sc_time& t, sc_event_or_list& el);

extern void rc_next_trigger(
    double v, sc_time_unit tu, sc_event_or_list& el);

extern void rc_next_trigger(const sc_time& t, sc_event_and_list& el);

extern void rc_next_trigger(
    double v, sc_time_unit tu, sc_event_and_list& el);

extern void rc_wait();

extern void rc_wait(int n);

extern void rc_wait(const sc_event& e);

extern void rc_wait(sc_event_or_list& el);

extern void rc_wait(sc_event_and_list& el);

extern void rc_wait(const sc_time& t);

extern void rc_wait(double v, sc_time_unit tu);

extern void rc_wait(const sc_time& t, const sc_event& e);

extern void rc_wait(double v, sc_time_unit tu, const sc_event& e);

extern void rc_wait(const sc_time& t, sc_event_or_list& el);

extern void rc_wait(double v, sc_time_unit t, sc_event_or_list& el);

extern void rc_wait(const sc_time& t, sc_event_and_list& el);

extern void rc_wait(double v, sc_time_unit tu, sc_event_and_list& el);

extern bool rc_is_update_phase();

extern void rc_notify(sc_event& e);

// note: rc_spawn(..) only makes sense being called from inside a
//       rc_reconfigurable. Thus there is no global rc_spawn(..) function
//       available.

} // namespace ReChannel

#define rc_bind    boost::bind
#define rc_ref(x)  boost::ref(x)
#define rc_cref(x) boost::cref(x)

#endif // RC_PROCESS_H_

//
// $Id: rc_process.h,v 1.15 2007/12/06 11:42:43 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_process.h,v $
//
