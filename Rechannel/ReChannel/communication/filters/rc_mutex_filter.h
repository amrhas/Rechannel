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
 * @file    rc_mutex_filter.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Definition of class rc_mutex_filter.
 *
 *
 * $Date: 2007/12/06 11:42:43 $
 * $Revision: 1.4 $
 */

#ifndef RC_MUTEX_FILTER_H_
#define RC_MUTEX_FILTER_H_

#include "ReChannel/communication/filters/rc_abstract_filter.h"

namespace ReChannel {

/**
 * @brief Predefined mutex filter.
 */
class rc_mutex_filter
    : public rc_abstract_filter<sc_mutex_if>
{

private:
    typedef rc_abstract_filter<sc_mutex_if> base_type;
    typedef rc_mutex_filter                 this_type;

public:
    rc_mutex_filter(rc_transaction_counter& tc)
        : m_tc(tc)
    { }

    virtual int lock()
    {
        base_type::lock();
        ++m_tc;
        return 0;
    }
    virtual int trylock()
    {
        const int ret = base_type::trylock();
        if (ret == 0) {
            ++m_tc;
        }
        return ret;
    }
    virtual int unlock()
    {
        const int ret = base_type::unlock();
        if (ret == 0) {
            --m_tc;
        }
        return ret;
    }

protected:

    rc_transaction_counter& m_tc;

private:
    // disabled
    rc_mutex_filter(const this_type& other_);
    this_type& operator=(const this_type& other_);
};

} // namespace ReChannel

#endif // RC_MUTEX_FILTER_H_

//
// $Id: rc_mutex_filter.h,v 1.4 2007/12/06 11:42:43 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/filters/rc_mutex_filter.h,v $
//
