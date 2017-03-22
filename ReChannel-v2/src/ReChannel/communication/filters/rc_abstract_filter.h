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
 * @file    rc_abstract_filter.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Definition of class rc_abstract_filter.
 *
 *
 * $Date: 2008/01/01 13:50:29 $
 * $Revision: 1.7 $
 */

#ifndef RC_ABSTRACT_FILTER_H_
#define RC_ABSTRACT_FILTER_H_

#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/core/rc_transaction_counter.h"
#include "ReChannel/communication/rc_interface_wrapper.h"
#include "ReChannel/communication/accessors/rc_accessor.h"

namespace ReChannel {

/**
 * @brief Abstract filter class.
 */
template<class IF>
class rc_abstract_filter
    : public rc_accessor<IF>,
      virtual public IF
{
    RC_STATIC_ASSERT_VALID_INTERFACE(IF);

private:
    typedef rc_abstract_filter<IF> this_type;

protected:
    typedef rc_accessor<IF> accessor_type;

public:
    typedef IF if_type;

protected:
    rc_abstract_filter();

protected:
    inline rc_reconfigurable* rc_get_reconfigurable() const
        { return reconf; }

    inline bool rc_is_deactivation_requested() const;

    inline void rc_possible_deactivation()
        { this->_rc_possible_deactivation(false); }

    inline void rc_nb_possible_deactivation()
        { this->_rc_possible_deactivation(true); }

    inline void rc_cancel() const
        { throw new rc_process_cancel_exception(); }

    inline void rc_nb_cancel() const
        { throw new rc_process_cancel_exception(); }

/* next_trigger(..) redirects */

    inline void next_trigger()
        { rc_next_trigger(); }

    inline void next_trigger(const sc_event& e)
        { rc_next_trigger(e); }

    inline void next_trigger(sc_event_or_list& el)
        { rc_next_trigger(el); }

    inline void next_trigger(sc_event_and_list& el)
        { rc_next_trigger(el); }

    inline void next_trigger(const sc_time& t)
        { rc_next_trigger(t); }

    inline void next_trigger(double v, sc_time_unit tu)
        { rc_next_trigger(v, tu); }

    inline void next_trigger(const sc_time& t, const sc_event& e)
        { rc_next_trigger(t, e); }

    inline void next_trigger(double v, sc_time_unit tu, const sc_event& e)
        { rc_next_trigger(v, tu, e); }

    inline void next_trigger(const sc_time& t, sc_event_or_list& el)
        { rc_next_trigger(t, el); }

    inline void next_trigger(
        double v, sc_time_unit t, sc_event_or_list& el)
        { rc_next_trigger(v, t, el); }

    inline void next_trigger(const sc_time& t, sc_event_and_list& el)
        { rc_next_trigger(t, el); }

    inline void next_trigger(
        double v, sc_time_unit tu, sc_event_and_list& el)
        { rc_next_trigger(v, tu, el); }

/* rc_wait() redirects */

    inline void wait() const
        { rc_wait(); }

    inline void wait(int n)
        { rc_wait(n); }

    inline void wait(const sc_event& e)
        { rc_wait(e); }

    inline void wait(sc_event_or_list& el)
        { rc_wait(el); }

    inline void wait(sc_event_and_list& el)
        { rc_wait(el); }

    inline void wait(const sc_time& t)
        { rc_wait(t); }

    inline void wait(double v, sc_time_unit tu)
        { rc_wait(v, tu); }

    inline void wait(const sc_time& t, const sc_event& e)
        { rc_wait(t, e); }

    inline void wait(double v, sc_time_unit tu, const sc_event& e)
        { rc_wait(v, tu, e); }

    inline void wait(const sc_time& t, sc_event_or_list& el)
        { rc_wait(t, el); }

    inline void wait(double v, sc_time_unit t, sc_event_or_list& el)
        { rc_wait(v, t, el); }

    inline void wait(const sc_time& t, sc_event_and_list& el)
        { rc_wait(t, el); }

    inline void wait(double v, sc_time_unit tu, sc_event_and_list& el)
        { rc_wait(v, tu, el); }

private:

    rc_reconfigurable* reconf;

private:
    // disabled
    rc_abstract_filter(const this_type& other_);
    this_type& operator=(const this_type& other_);
};

/* inline code */

template<class IF>
inline
bool rc_abstract_filter<IF>::rc_is_deactivation_requested() const
{
    if (reconf != NULL) {
        return reconf->rc_is_deactivation_requested();
    } else {
        return false;
    }
}

/* template code */

template<class IF>
rc_abstract_filter<IF>::rc_abstract_filter()
    : reconf(rc_get_reconfigurable_context())
{ }

} // namespace ReChannel

#endif // RC_ABSTRACT_FILTER_H_

//
// $Id: rc_abstract_filter.h,v 1.7 2008/01/01 13:50:29 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/filters/rc_abstract_filter.h,v $
//
