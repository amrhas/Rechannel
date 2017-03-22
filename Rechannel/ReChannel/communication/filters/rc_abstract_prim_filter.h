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
 * @file    rc_abstract_prim_filter.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Definition of class rc_abstract_prim_filter.
 *
 *
 * $Date: 2008/01/01 13:50:07 $
 * $Revision: 1.6 $
 */

#ifndef RC_ABSTRACT_PRIM_FILTER_H_
#define RC_ABSTRACT_PRIM_FILTER_H_

#include "ReChannel/communication/filters/rc_abstract_filter.h"

namespace ReChannel {

/**
 * @brief Abstract primitive filter class.
 */
template<class IF>
class rc_abstract_prim_filter
    : public rc_abstract_filter<IF>
{
    RC_STATIC_ASSERT_VALID_INTERFACE(IF);

private:
    typedef rc_abstract_prim_filter<IF> this_type;

protected:
    typedef rc_accessor<IF> accessor_type;
    typedef typename boost::template function<void (bool)> sync_callback;

    enum rc_predefined_sync_callback_id {
        RC_DO_NOTHING_ = 0,
        RC_POSSIBLE_DEACTIVATION_,
        RC_MODIFY_TRANSACTION_
    };

public:
    typedef IF if_type;

protected:
    rc_abstract_prim_filter();

    explicit rc_abstract_prim_filter(
        const sync_callback& func_before, const sync_callback& func_after);

    rc_abstract_prim_filter(
        rc_predefined_sync_callback_id func_before,
        rc_predefined_sync_callback_id func_after,
        rc_transaction_counter& tc, int tc_modify);

protected:

    inline void rc_sync_callback_before()
        { this->_rc_sync_callback_before(false); }

    inline void rc_sync_callback_after()
        { this->_rc_sync_callback_after(false); }

    inline void rc_nb_sync_callback_before()
        { this->_rc_sync_callback_before(true); }

    inline void rc_nb_sync_callback_after()
        { this->_rc_sync_callback_after(true); }

    sync_callback rc_predefined_sync_callback(
        rc_predefined_sync_callback_id func);

    sync_callback rc_predefined_sync_callback(
        rc_predefined_sync_callback_id func,
        rc_transaction_counter& tc, int tc_modify);

private:

    inline void _rc_sync_callback_before(bool nb);

    inline void _rc_sync_callback_after(bool nb);

private:

    static void _rc_modify_transaction_callback(
        bool nb, rc_transaction_counter& tc, int count);

    static void _rc_possible_deactivation_callback(
        bool nb, rc_reconfigurable* reconf);

protected:

    sync_callback m_sync_callback_before;
    sync_callback m_sync_callback_after;

private:
    // disabled
    rc_abstract_prim_filter(const this_type& other_);
    this_type& operator=(const this_type& other_);
};

/* inline code */

template<class IF>
inline
void rc_abstract_prim_filter<IF>::_rc_sync_callback_before(bool nb)
{
    if (!m_sync_callback_before.empty()) {
        this->m_sync_callback_before(nb);
    }
}

template<class IF>
inline
void rc_abstract_prim_filter<IF>::_rc_sync_callback_after(bool nb)
{
    if (!m_sync_callback_after.empty()) {
        this->m_sync_callback_after(nb);
    }
}

/* template code */

template<class IF>
rc_abstract_prim_filter<IF>::rc_abstract_prim_filter()
{ }

template<class IF>
rc_abstract_prim_filter<IF>::rc_abstract_prim_filter(
    const sync_callback& func_before, const sync_callback& func_after)
    : m_sync_callback_before(func_before),
      m_sync_callback_after(func_after)
{ }

template<class IF>
typename rc_abstract_prim_filter<IF>::sync_callback
rc_abstract_prim_filter<IF>::rc_predefined_sync_callback(
    rc_predefined_sync_callback_id func_type,
    rc_transaction_counter& tc, int tc_modify)
{
    switch(func_type) {
        case RC_POSSIBLE_DEACTIVATION_:
            return boost::bind(
                &this_type::_rc_possible_deactivation_callback,
                _1, this->rc_get_reconfigurable());
        case RC_MODIFY_TRANSACTION_:
            return boost::bind(
                &this_type::_rc_modify_transaction_callback,
                _1, boost::ref(tc), tc_modify);
        case RC_DO_NOTHING_:
        default:
            return sync_callback();
    }
}

template<class IF>
typename rc_abstract_prim_filter<IF>::sync_callback
rc_abstract_prim_filter<IF>::rc_predefined_sync_callback(
    rc_predefined_sync_callback_id func_type)
{
    switch(func_type) {
        case RC_POSSIBLE_DEACTIVATION_:
            return boost::bind(
                &this_type::_rc_possible_deactivation_callback,
                _1, this->rc_get_reconfigurable());
        case RC_MODIFY_TRANSACTION_:
        case RC_DO_NOTHING_:
        default:
            return sync_callback();
    }
}

template<class IF>
void rc_abstract_prim_filter<IF>::_rc_modify_transaction_callback(
    bool nb, rc_transaction_counter& tc, int count)
{
    tc.modify(count);
}

template<class IF>
void rc_abstract_prim_filter<IF>::_rc_possible_deactivation_callback(
    bool nb, rc_reconfigurable* reconf)
{
    if (nb == false && reconf != NULL) {
        reconf->rc_possible_deactivation();
    }
}

} // namespace ReChannel

#define RC_NO_SYNC_CALLBACK() NULL

#define RC_SYNC_CALLBACK(func) \
    rc_bind(&RC_CURRENT_USER_MODULE::func, this, _1)

#define RC_SYNC_CALLBACK0(func) \
    rc_bind(&RC_CURRENT_USER_MODULE::func, this, _1)

#define RC_SYNC_CALLBACK1(func, p1) \
    rc_bind(&RC_CURRENT_USER_MODULE::func, this, _1, p1)

#define RC_SYNC_CALLBACK2(func, p1, p2) \
    rc_bind(&RC_CURRENT_USER_MODULE::func, this, _1, p1, p2)

#define RC_SYNC_CALLBACK3(func, p1, p2, p3) \
    rc_bind(&RC_CURRENT_USER_MODULE::func, this, _1, p1, p2, p3)

#define RC_SYNC_CALLBACK4(func, p1, p2, p3, p4) \
    rc_bind(&RC_CURRENT_USER_MODULE::func, this, _1, p1, p2, p3, p4)

#define RC_SYNC_CALLBACK5(func, p1, p2, p3, p4, p5) \
    rc_bind(&RC_CURRENT_USER_MODULE::func, this, _1, p1, p2, p3, p4, p5)

#define RC_SYNC_CALLBACK6(func, p1, p2, p3, p4, p5, p6) \
    rc_bind(&RC_CURRENT_USER_MODULE::func, this, \
        _1, p1, p2, p3, p4, p5, p6)

#define RC_SYNC_CALLBACK7(func, p1, p2, p3, p4, p5, p6, p7) \
    rc_bind( \
        &RC_CURRENT_USER_MODULE::func, this, \
        _1, p1, p2, p3, p4, p5, p6, p7)

#define RC_SYNC_CALLBACK8(func, p1, p2, p3, p4, p5, p6, p7, p8) \
    rc_bind( \
        &RC_CURRENT_USER_MODULE::func, this, \
        _1, p1, p2, p3, p4, p5, p6, p7, p8)

#define RC_SYNC_CALLBACK9(func, p1, p2, p3, p4, p5, p6, p7, p8, p9) \
    rc_bind( \
        &RC_CURRENT_USER_MODULE::func, this, \
        _1, p1, p2, p3, p4, p5, p6, p7, p8, p9)

#define RC_SYNC_CALLBACK10(func, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    rc_bind( \
        &RC_CURRENT_USER_MODULE::func, this, \
        _1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#endif // RC_ABSTRACT_PRIM_FILTER_H_

//
// $Id: rc_abstract_prim_filter.h,v 1.6 2008/01/01 13:50:07 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/filters/rc_abstract_prim_filter.h,v $
//
