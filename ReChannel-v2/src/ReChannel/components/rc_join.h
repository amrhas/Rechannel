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
 * @file    rc_join.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:24:55 $
 * $Revision: 1.7 $
 */

#ifndef RC_JOIN_H_
#define RC_JOIN_H_

#include "ReChannel/core/rc_process.h"

namespace ReChannel {

/**
 * @brief This class is used by RC_FORK and RC_JOIN.
 */
class rc_join
{
private:
    typedef std::vector<rc_process_handle> hproc_vector;

public:
    rc_join();

    void add_process(rc_process_handle hproc)
        { p_hproc_vector.push_back(hproc); }

    unsigned int get_process_count()
        { return p_hproc_vector.size(); }

    void wait();

    void reset()
        { p_hproc_vector.clear(); }

private:
    hproc_vector p_hproc_vector;

private:
    // disabled
    rc_join(const rc_join& orig);
    rc_join& operator =(const rc_join& orig);
};

} // namespace ReChannel

#define RC_RECONFIGURABLE_FORK \
    { \
        ::ReChannel::rc_process_handle proc_arr[] = {

#define RC_RECONFIGURABLE_JOIN \
        }; \
        ::ReChannel::rc_join join; \
        for (unsigned int i = 0; \
            i < sizeof(proc_arr)/sizeof(::ReChannel::rc_process_handle); \
            i++) \
        { \
            join.add_process(proc_arr[i]); \
        } \
        join.wait(); \
    }

/**
 * @brief Resettable representation of SC_FORK.
 */
#define RC_FORK \
    RC_RECONFIGURABLE_FORK

/**
 * @brief Resettable representation of SC_JOIN.
 */
#define RC_JOIN \
    RC_RECONFIGURABLE_JOIN

#endif // RC_JOIN_H_

//
// $Id: rc_join.h,v 1.7 2007/11/23 13:24:55 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_join.h,v $
//
