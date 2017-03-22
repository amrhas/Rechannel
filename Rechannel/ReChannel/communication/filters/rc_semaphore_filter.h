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
 * @file    rc_semaphore_filter.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Definition of class rc_semaphore_filter.
 *
 *
 * $Date: 2007/12/06 11:42:43 $
 * $Revision: 1.4 $
 */

#ifndef RC_SEMAPHORE_FILTER_H_
#define RC_SEMAPHORE_FILTER_H_

#include "ReChannel/communication/filters/rc_abstract_filter.h"

namespace ReChannel {

/**
 * @brief Predefined semaphore filter.
 */
class rc_semaphore_filter
    : public rc_abstract_filter<sc_semaphore_if>
{

private:
    typedef rc_abstract_filter<sc_semaphore_if> base_type;
    typedef rc_semaphore_filter                 this_type;

public:
    rc_semaphore_filter(
        rc_transaction_counter& tc, bool tc_zero_saturated=true)
        : m_tc(tc), m_post_count(0), m_zero_saturated(tc_zero_saturated)
    { }

    virtual int wait()
    {
        this->rc_forward(&sc_semaphore_if::wait);
        ++m_post_count;
        if (m_post_count > 0 || !m_zero_saturated) {
            ++m_tc;
        }
        return 0;
    }

    virtual int trywait()
    {
        const int ret = base_type::trywait();
        if (ret == 0) {
            ++m_post_count;
            if (m_post_count > 0 || !m_zero_saturated) {
                ++m_tc;
            }
        }
        return ret;
    }

    virtual int post()
    {
        base_type::post();
        if (m_post_count > 0 || !m_zero_saturated) {
            --m_tc;
        }
        --m_post_count;
        return 0;
    }

    virtual int get_value() const
    {
        return base_type::get_value();
    }

protected:

    rc_transaction_counter& m_tc;
    int  m_post_count;
    bool m_zero_saturated;

private:
    // disabled
    rc_semaphore_filter(const this_type& other_);
    this_type& operator=(const this_type& other_);
};

} // namespace ReChannel

#endif // RC_SEMAPHORE_FILTER_H_

//
// $Id: rc_semaphore_filter.h,v 1.4 2007/12/06 11:42:43 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/filters/rc_semaphore_filter.h,v $
//
