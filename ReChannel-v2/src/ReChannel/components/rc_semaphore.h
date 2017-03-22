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
 * @file    rc_semaphore.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.4 $
 */

#ifndef RC_SEMAPHORE_H_
#define RC_SEMAPHORE_H_

#include "ReChannel/components/rc_prim_channel.h"
#include "ReChannel/components/rc_report_ids.h"

namespace ReChannel {

/**
 * @brief Resettable representation of sc_semaphore.
 */
class rc_semaphore
    : public sc_semaphore_if,
      public rc_prim_channel
{
public:

    explicit rc_semaphore(int value_);

    rc_semaphore(const char* name_, int value_);

    inline virtual const char* kind() const
        { return "rc_semaphore"; }

    virtual int wait();

    virtual int trywait();

    virtual int post();

    inline virtual int get_value() const
        { return m_value; }

protected:

    virtual void rc_on_reset();    

protected:

    int      m_value;
    int      m_reset_value;
    sc_event m_free;

private:
    // disabled
    rc_semaphore(const rc_semaphore& other);
    rc_semaphore& operator=(const rc_semaphore& other);
};

} // namespace ReChannel

#endif //RC_SEMAPHORE_H_

//
// $Id: rc_semaphore.h,v 1.4 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_semaphore.h,v $
//

