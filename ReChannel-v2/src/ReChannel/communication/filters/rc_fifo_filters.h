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
 * @file    rc_fifo_filters.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Contains the fifo filters.
 *
 *
 * $Date: 2008/01/01 13:49:24 $
 * $Revision: 1.9 $
 */

#ifndef RC_FIFO_FILTERS_H_
#define RC_FIFO_FILTERS_H_

#include "ReChannel/communication/filters/rc_abstract_prim_filter.h"
#include "ReChannel/communication/accessors/rc_fifo_accessors.h"

namespace ReChannel {

/**
 * @brief Predefined FIFO input filter.
 */
template<class T>
class rc_fifo_in_filter
    : public rc_abstract_prim_filter<sc_fifo_in_if<T> >
{
public:
    typedef rc_abstract_prim_filter<sc_fifo_in_if<T> > base_type;
    typedef typename base_type::accessor_type          accessor_type;
    typedef typename base_type::sync_callback          sync_callback;

    using base_type::RC_POSSIBLE_DEACTIVATION_;
    using base_type::RC_MODIFY_TRANSACTION_;
    using base_type::m_sync_callback_before;
    using base_type::m_sync_callback_after;

public:

    explicit rc_fifo_in_filter(int read_limit=-1);

    rc_fifo_in_filter(
        const sync_callback& func_before, const sync_callback& func_after,
        int read_limit=-1);

    explicit rc_fifo_in_filter(
        rc_transaction_counter& tc, int tc_modify=+1, int read_limit=-1);

    virtual void read(T& value);

    virtual T read();

    virtual bool nb_read(T& value);

    virtual int num_available() const;

    inline int get_read_limit() const
        { return p_max_available; }

    inline void set_read_limit(int read_limit_)
    {
        if (read_limit_ != 0 && && p_max_available == 0
        && base_type::num_available() > 0)
        {
            this->trigger_written_event();
        }
        p_max_available = (read_limit_ >= 0 ? read_limit_ : -1);
    }

    inline int incr_read_limit(int amount_=1);

    inline int decr_read_limit(int amount_=1)
        { return this->incr_read_limit(-amount_); }

    inline void reset_read_limit()
    {
        if (p_max_available_default != 0 && p_max_available == 0
        && base_type::num_available() > 0)
        {
            this->trigger_written_event();
        }
        p_max_available = p_max_available_default;
    }

    inline virtual rc_event_filter* rc_get_event_filter()
        { return this; }

    inline T get_last_read() const
        { return p_last_read_value; }

protected:

    virtual void rc_on_target_changed();

    inline virtual bool rc_on_event(const sc_event& e)
    {
        if (&e != &this->data_written_event()) {
            return false;
        }
        return (p_max_available != 0);
    }

    virtual void rc_set_event_trigger(
        const sc_event& e, rc_event_trigger& t)
    {
        if (&e == &this->data_written_event()) {
            p_event_trigger = &t;
        }
    }

    virtual void rc_clear_event_trigger(const sc_event& e)
    {
        if (&e == &this->data_written_event()) {
            p_event_trigger = NULL;
        }
    }

    inline void trigger_written_event()
    {
        if (p_event_trigger != NULL) {
            p_event_trigger->rc_trigger_event(
                this->data_written_event());
        }
    }

private:
    int p_max_available_default;
    int p_max_available;
    rc_event_trigger* p_event_trigger;
    T   p_last_read_value;
};

/**
 * @brief Predefined FIFO output filter.
 */
template<class T>
class rc_fifo_out_filter
    : public rc_abstract_prim_filter<sc_fifo_out_if<T> >
{
public:
    typedef rc_abstract_prim_filter<sc_fifo_out_if<T> > base_type;
    typedef typename base_type::accessor_type           accessor_type;
    typedef typename base_type::sync_callback           sync_callback;

    using base_type::RC_POSSIBLE_DEACTIVATION_;
    using base_type::RC_MODIFY_TRANSACTION_;
    using base_type::m_sync_callback_before;
    using base_type::m_sync_callback_after;

public:

    explicit rc_fifo_out_filter(int write_limit=-1);

    rc_fifo_out_filter(
        const sync_callback& func_before, const sync_callback& func_after,
        int write_limit=-1);

    explicit rc_fifo_out_filter(
        rc_transaction_counter& tc, int tc_modify=-1, int write_limit=-1);

    virtual void write(const T& value);

    virtual bool nb_write(const T& value);

    virtual int num_free() const;

    inline int get_write_limit() const
        { return p_max_free; }

    inline void set_write_limit(int write_limit_)
    {
        if (write_limit_ != 0 && p_max_free == 0
        && base_type::num_free() > 0)
        {
            this->trigger_read_event();
        }
        p_max_free = (write_limit_ >= 0 ? write_limit_ : -1);
    }

    inline int incr_write_limit(int amount_=1);

    inline int decr_write_limit(int amount_=1)
        { return this->incr_write_limit(-amount_); }

    inline void reset_write_limit()
    {
        if (p_max_free_default != 0 && p_max_free == 0
        && base_type::num_free() > 0)
        {
            this->trigger_read_event();
        }
        p_max_free = p_max_free_default;
    }

    inline virtual rc_event_filter* rc_get_event_filter()
        { return this; }

    inline T get_last_written() const
        { return p_last_written_value; }

protected:

    virtual void rc_on_target_changed();

    inline virtual bool rc_on_event(const sc_event& e)
    {
        if (&e != &this->data_read_event()) {
            return false;
        }
        return (p_max_free != 0);
    }

    virtual void rc_set_event_trigger(
        const sc_event& e, rc_event_trigger& t)
    {
        if (&e == &this->data_read_event()) {
            p_event_trigger = &t;
        }
    }

    virtual void rc_clear_event_trigger(const sc_event& e)
    {
        if (&e == &this->data_read_event()) {
            p_event_trigger = NULL;
        }
    }

    inline void trigger_read_event()
    {
        if (p_event_trigger != NULL) {
            p_event_trigger->rc_trigger_event(
                this->data_read_event());
        }
    }

private:
    int p_max_free_default;
    int p_max_free;
    rc_event_trigger* p_event_trigger;
    T   p_last_written_value;
};

/* inline code */

template<class T>
inline
int rc_fifo_in_filter<T>::incr_read_limit(int amount_)
{
    if (p_max_available >= 0)
    {
        if (amount_ > 0 && p_max_available == 0
        && base_type::num_available() > 0)
        {
            this->trigger_written_event();
        }
        p_max_available += amount_;
        if (p_max_available < 0) {
            p_max_available = 0;
        }
    }
    return p_max_available;
}

template<class T>
inline
int rc_fifo_out_filter<T>::incr_write_limit(int amount_)
{
    if (p_max_free >= 0)
    {
        if (amount_ > 0 && p_max_free == 0
        && base_type::num_free() > 0) {
            this->trigger_read_event();
        }
        p_max_free += amount_;
        if (p_max_free < 0) {
            p_max_free = 0;
        }
    }
    return p_max_free;
}

/* template code */

template<class T>
rc_fifo_in_filter<T>::rc_fifo_in_filter(int read_limit)
    : p_max_available_default(read_limit >= 0 ? read_limit : -1),
      p_max_available(p_max_available_default), p_event_trigger(NULL),
      p_last_read_value(rc_undefined_value<T>())
{ }

template<class T>
rc_fifo_in_filter<T>::rc_fifo_in_filter(
    const sync_callback& func_before, const sync_callback& func_after,
    int read_limit)
    : base_type(func_before, func_after),
      p_max_available_default(read_limit >= 0 ? read_limit : -1),
      p_max_available(p_max_available_default), p_event_trigger(NULL),
      p_last_read_value(rc_undefined_value<T>())
{ }

template<class T>
rc_fifo_in_filter<T>::rc_fifo_in_filter(
    rc_transaction_counter& tc, int tc_modify, int read_limit)
    : p_max_available_default(read_limit >= 0 ? read_limit : -1),
      p_max_available(p_max_available_default), p_event_trigger(NULL),
      p_last_read_value(rc_undefined_value<T>())
{
    m_sync_callback_before =
        this->rc_predefined_sync_callback(
            (tc_modify >= 0 ?
                RC_POSSIBLE_DEACTIVATION_ : RC_MODIFY_TRANSACTION_),
            tc, tc_modify);
    m_sync_callback_after =
        this->rc_predefined_sync_callback(
            (tc_modify >= 0 ?
                RC_MODIFY_TRANSACTION_ : RC_POSSIBLE_DEACTIVATION_),
            tc, tc_modify);
}

template<class T>
void rc_fifo_in_filter<T>::read(T& value)
{
    this->rc_sync_callback_before();
    base_type::read(value);
    if (p_max_available > 0) {
        p_max_available--;
    }
    p_last_read_value = value;
    this->rc_sync_callback_after();
}

template<class T>
T rc_fifo_in_filter<T>::read()
{
    this->rc_sync_callback_before();
    p_last_read_value = base_type::read();
    if (p_max_available > 0) {
        p_max_available--;
    }
    this->rc_sync_callback_after();
    return p_last_read_value;
}

template<class T>
bool rc_fifo_in_filter<T>::nb_read(T& value)
{
    this->rc_nb_sync_callback_before();
    const bool has_read =
        (p_max_available != 0 && base_type::nb_read(value));
    if (has_read) {
        if (p_max_available > 0) {
            p_max_available--;
        }
        p_last_read_value = value;
        this->rc_nb_sync_callback_after();
    }
    return has_read;
}

template<class T>
int rc_fifo_in_filter<T>::num_available() const
{
    const int num = base_type::num_available();
    if (p_max_available == -1) {
        return num;
    } else {
        return (num <= p_max_available ? num : p_max_available);
    }
}

template<class T>
void rc_fifo_in_filter<T>::rc_on_target_changed()
{
    p_max_available = p_max_available_default;
    base_type::rc_on_target_changed();
}

template<class T>
rc_fifo_out_filter<T>::rc_fifo_out_filter(int write_limit)
    : p_max_free_default(write_limit >= 0 ? write_limit : -1),
      p_max_free(p_max_free_default), p_event_trigger(NULL),
      p_last_written_value(rc_undefined_value<T>())
{ }

template<class T>
rc_fifo_out_filter<T>::rc_fifo_out_filter(
    const sync_callback& func_before, const sync_callback& func_after,
    int write_limit)
    : base_type(func_before, func_after),
      p_max_free_default(write_limit >= 0 ? write_limit : -1),
      p_max_free(p_max_free_default), p_event_trigger(NULL),
      p_last_written_value(rc_undefined_value<T>())
{ }

template<class T>
rc_fifo_out_filter<T>::rc_fifo_out_filter(
    rc_transaction_counter& tc, int tc_modify, int write_limit)
    : p_max_free_default(write_limit >= 0 ? write_limit : -1),
      p_max_free(p_max_free_default), p_event_trigger(NULL),
      p_last_written_value(rc_undefined_value<T>())
{
    if (tc_modify < 0) {
        m_sync_callback_after =
            this->rc_predefined_sync_callback(
                RC_MODIFY_TRANSACTION_, tc, tc_modify);
    } else {
        m_sync_callback_before =
            this->rc_predefined_sync_callback(
                RC_MODIFY_TRANSACTION_, tc, tc_modify);
    }
}

template<class T>
void rc_fifo_out_filter<T>::write(const T& value)
{
    this->rc_sync_callback_before();
    base_type::write(value);
    if (p_max_free > 0) {
        p_max_free--;
    }
    p_last_written_value = value;
    this->rc_sync_callback_after();
}

template<class T>
bool rc_fifo_out_filter<T>::nb_write(const T& value)
{
    this->rc_nb_sync_callback_before();
    if (p_max_free != 0 && base_type::nb_write(value)) {
        if (p_max_free > 0) {
            p_max_free--;
        }
        p_last_written_value = value;
        this->rc_nb_sync_callback_after();
        return true;
    } else {
        return false;
    }
}

template<class T>
int rc_fifo_out_filter<T>::num_free() const
{
    const int num = base_type::num_free();
    if (p_max_free == -1) {
        return num;
    } else {
        return (num <= p_max_free ? num : p_max_free);
    }
}

template<class T>
void rc_fifo_out_filter<T>::rc_on_target_changed()
{
    p_max_free = p_max_free_default;
    base_type::rc_on_target_changed();
}

} // namespace ReChannel

#endif // RC_FIFO_FILTERS_H_

//
// $Id: rc_fifo_filters.h,v 1.9 2008/01/01 13:49:24 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/filters/rc_fifo_filters.h,v $
//
