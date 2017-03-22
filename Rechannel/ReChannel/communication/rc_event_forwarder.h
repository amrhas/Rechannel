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
 * @file    rc_event_forwarder.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Classes for event forwarding.
 *
 *
 * $Date: 2007/12/20 20:32:36 $
 * $Revision: 1.15 $
 */

#ifndef RC_EVENT_FORWARDER_H_
#define RC_EVENT_FORWARDER_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_process.h"
#include "ReChannel/core/rc_event_filter.h"

namespace ReChannel {

/**
 * @brief Abstract base class of an event forwarder.
 */
class rc_event_forwarder_base
{
public:
    /**
     * @brief Adds a target event.
     */
    virtual bool add_target(
        sc_event& target_event, rc_event_filter* filter=NULL) = 0;

    /**
     * @brief Adds a target event obtained from the target interface.
     */
    virtual bool add_target(
        sc_interface& target_if, rc_event_filter* filter=NULL) = 0;

    /**
     * @brief Returns the number of added target events.
     */
    virtual int get_target_count() const = 0;

    /**
     * @brief Clears the target chain.
     */
    virtual void clear_targets() = 0;

    /**
     * @brief Triggers the event notification on the target chain.
     * \remarks May be used as a forwarder process.
     */
    virtual void trigger_event() = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~rc_event_forwarder_base() {}
};

/**
 * @brief Implementation of an event forwarder (optimized for one or two targets)
 */
template<class IF>
class rc_event_forwarder
    : virtual public rc_event_forwarder_base,
      virtual protected rc_event_trigger
{
    RC_STATIC_ASSERT_VALID_INTERFACE(IF);

private:
    struct target {
        target()
            : e(NULL), filter(NULL)
        { }
        target(sc_event* e_, rc_event_filter* filter_)
            : e(e_), filter(filter_)
        { }
        sc_event*        e;
        rc_event_filter* filter;
    };

protected:
    /**
     * @brief Event getter functor.
     *
     * An event getter functor must provide the following signature:
     * - const sc_event& event_getter(IF* target_if) [const] or
     * - const sc_event& IF::*event_getter() [const]
     */
    typedef boost::function<const sc_event& (IF*)> event_getter_type;

    typedef std::vector<target> target_vector;

public:
    /**
     * @brief Constructor.
     */
    rc_event_forwarder(event_getter_type event_getter_method);

    /**
     * @brief Adds a target event.
     */
    virtual bool add_target(
        sc_event& target_event, rc_event_filter* filter=NULL);

    /**
     * @brief Adds a target event obtained from the target interface.
     */
    virtual bool add_target(
        sc_interface& target_if, rc_event_filter* filter=NULL);

    /**
     * @brief Adds a target event obtained from the target interface.
     */
    virtual bool add_target(
        IF& target_if, rc_event_filter* filter=NULL);

    /**
     * @brief Returns the number of added target events.
     */
    virtual int get_target_count() const;

    /**
     * @brief Clears the target chain.
     */
    virtual void clear_targets();

    /**
     * @brief Triggers the event notification on the target chain.
     * \remarks May be used as a forwarder process.
     */
    virtual void trigger_event();

    /**
     * @brief Virtual destructor.
     */
    virtual ~rc_event_forwarder();

protected:

    /**
     * @brief Event notification trigger (requested by a filter).
     *
     * In constrast to trigger_event(), it starts triggering at the
     * position of the specified event in the chain.
     *
     * The call will be ignored if the event cannot be found in the chain.
     */
    virtual void rc_trigger_event(const sc_event& start_event);

protected:
    /**
     * @brief event getter functor
     */
    const event_getter_type m_get_event;
    /**
     * @brief first target event
     */
    target               m_target1;
    /**
     * @brief second target event
     */
    target               m_target2;
    /**
     * @brief further target events
     */
    target_vector        m_targets;
};

/**
 * @brief Abstract base class of an event forwarder host.
 */
class rc_event_forwarder_host
{
public:
    typedef std::vector<std::string> string_vector;

public:
    /**
     * @brief Directly adds an event forwarder being sensitive to an event.
     */
    virtual void add_event_forwarder(
        rc_event_forwarder_base& forwarder, const sc_event& e,
        const std::string& event_name=NULL) = 0;

    /**
     * @brief Adds a target interface to all event forwarders.
     */
    virtual void add_event_forwarder_target(
        sc_interface& if_, rc_event_filter* filter=NULL) = 0;

    /**
     * @brief Adds a target interface to the specified event forwarder.
     */
    virtual bool add_event_forwarder_target(
        const std::string& event_name, sc_interface& if_,
        rc_event_filter* filter=NULL) = 0;

    /**
     * @brief Returns the number of added event forwarder targets.
     */
    virtual bool get_event_forwarder_target_count() const = 0;

    /**
     * @brief Clears the target lists of all event forwarders.
     */
    virtual void clear_event_forwarder_targets() = 0;

    /**
     * @brief Clears the target lists of the specified event forwarder.
     */
    virtual void clear_event_forwarder_targets(
        const std::string& event_name) = 0;

    /**
     * @brief Returns the names of all added event forwarders.
     */
    virtual string_vector get_event_forwarder_names() const = 0;

    /**
     * @brief Notifies the specified dynamic event.
     */
    virtual void notify_event(const std::string& event_name) = 0;

protected:

    /**
     * @brief Virtual destructor.
     */
    virtual ~rc_event_forwarder_host() {}
};

/* template code */

template<class IF>
rc_event_forwarder<IF>::rc_event_forwarder(
    event_getter_type event_getter_method)
    : m_get_event(event_getter_method)
{
    assert( !event_getter_method.empty() );
}

template<class IF>
rc_event_forwarder<IF>::~rc_event_forwarder()
{ }

template<class IF>
bool rc_event_forwarder<IF>::add_target(
    sc_event& target_event, rc_event_filter* filter)
{
    if (m_target1.e == NULL || m_target1.e == &target_event) {
        m_target1.e = &target_event;
        m_target1.filter = filter;
    } else if (m_target2.e == NULL || m_target2.e == &target_event) {
        m_target2.e = &target_event;
        m_target2.filter = filter;
    } else {
        for (typename target_vector::iterator it =
                m_targets.begin();
            it != m_targets.end();
            ++it)
        {
            target& target_ = (*it);
            // target already added?
            if (target_.e == &target_event) {
                // update the filter (since it may have changed)
                target_.filter = filter;
                if (filter != NULL) {
                    // set *this as the event trigger of target_event
                    filter->rc_set_event_trigger(target_event, *this);
                }
                return true;
            }
        }
        // target not found. add it to the vector
        m_targets.push_back(target(&target_event, filter));
    }
    if (filter != NULL) {
        // set *this as the event trigger of target_event
        filter->rc_set_event_trigger(target_event, *this);
    }
    return true;
}

template<class IF>
bool rc_event_forwarder<IF>::add_target(
    sc_interface& target_if, rc_event_filter* filter)
{
    IF* target_if_ = dynamic_cast<IF*>(&target_if);
    if (target_if_ != NULL) {
        return this->add_target(*target_if_, filter);
    } else {
        return false;
    }
}

template<class IF>
bool rc_event_forwarder<IF>::add_target(
    IF& target_if, rc_event_filter* filter)
{
    // unfortunately, retrieving the event necessiates a const_cast
    sc_event& event_ = const_cast<sc_event&>(m_get_event(&target_if));
    return this->add_target(event_, filter);
}

template<class IF>
int rc_event_forwarder<IF>::get_target_count() const
{
    return (m_target1.e != NULL ? 1 : 0)
            + (m_target2.e != NULL ? 1 + m_targets.size() : 0);
}

template<class IF>
void rc_event_forwarder<IF>::clear_targets()
{
    // clear the first target
    if (m_target1.e != NULL) {
        if (m_target1.filter != NULL) {
            m_target1.filter->rc_clear_event_trigger(*(m_target1.e));
        }
        m_target1.e = NULL;
        m_target1.filter = NULL;
    }
    // clear the second target
    if (m_target2.e != NULL) {
        if (m_target2.filter != NULL) {
            m_target2.filter->rc_clear_event_trigger(*(m_target2.e));
        }
        m_target2.e = NULL;
        m_target2.filter = NULL;
    }
    // clear the target chain
    for (typename target_vector::iterator it
        = m_targets.begin();
        it != m_targets.end();
        ++it)
    {
        target target_ = (*it);
        if (target_.e != NULL && target_.filter != NULL) {
            target_.filter->rc_clear_event_trigger(*(target_.e));
        }
    }
    m_targets.clear();
}

template<class IF>
void rc_event_forwarder<IF>::trigger_event()
{
    if (m_target1.e != NULL)
    {
        if (m_target1.filter != NULL
        && !m_target1.filter->rc_on_event(*(m_target1.e)))
        {
            return;
        }
        rc_notify(*(m_target1.e));

        if (m_target2.e != NULL)
        {
            if (m_target2.filter != NULL
            && !m_target2.filter->rc_on_event(*(m_target2.e)))
            {
                return;
            }
            rc_notify(*(m_target2.e));

            if (!m_targets.empty())
            {
                for (typename target_vector::iterator it
                        = m_targets.begin();
                    it != m_targets.end();
                    ++it)
                {
                    target target_ = (*it);
                    assert(target_.e != NULL);

                    if (target_.filter != NULL
                    && !target_.filter->rc_on_event(*(target_.e))) 
                    {
                        return;
                    }
                    rc_notify(*(target_.e));
                }
            }
        }
    }
}

template<class IF>
void rc_event_forwarder<IF>::rc_trigger_event(const sc_event& start_event)
{
    bool is_started = false;
    if (m_target1.e != NULL)
    {
        bool is_start_event = (m_target1.e == &start_event);
        if (is_start_event) {
            rc_notify(*(m_target1.e));
            is_started = true;
        }
        if (m_target2.e != NULL)
        {
            is_start_event = (m_target2.e == &start_event);
            if (is_start_event) {
                is_started = true;
            }
            if (is_started) {
                if (m_target2.filter != NULL
                && !is_start_event
                && m_target2.filter->rc_on_event(*(m_target2.e)))
                {
                    return;
                }
                rc_notify(*(m_target2.e));
            }
            if (!m_targets.empty())
            {
                for (typename target_vector::iterator it =
                        m_targets.begin();
                    it != m_targets.end();
                    ++it)
                {
                    target target_ = (*it);
                    assert(target_.e != NULL);

                    is_start_event = (target_.e == &start_event);
                    if (is_start_event) {
                        is_started = true;
                    }
                    if (is_started) {
                        if (target_.filter != NULL
                        && !is_start_event
                        && target_.filter->rc_on_event(*(target_.e)))
                        {
                            return;
                        }
                        rc_notify(*(target_.e));
                    }
                }
            }
        }
    }
}

} // namespace ReChannel

#endif // RC_EVENT_FORWARDER_H_

//
// $Id: rc_event_forwarder.h,v 1.15 2007/12/20 20:32:36 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/rc_event_forwarder.h,v $
//

