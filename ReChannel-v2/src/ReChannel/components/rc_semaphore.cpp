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
 * @file    rc_semaphore.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/10/09 00:22:26 $
 * $Revision: 1.3 $
 */

#include "rc_semaphore.h"

namespace ReChannel {

rc_semaphore::rc_semaphore(int value_)
    : rc_prim_channel(sc_gen_unique_name("semaphore")),
        m_value(value_), m_reset_value(value_)
{
    if (m_value < 0) {
        RC_REPORT_ERROR(RC_ID_SEMAPHORE_INVALID_VALUE_,
            "negative initial value (in semaphore '"
                << this->name() << "')");
    }
}

rc_semaphore::rc_semaphore(const char* name_, int value_)
    : rc_prim_channel(name_), m_value(value_), m_reset_value(value_)
{
    if (m_value < 0) {
        RC_REPORT_ERROR(RC_ID_SEMAPHORE_INVALID_VALUE_,
            "negative initial value (in semaphore '"
                << this->name() << "')");
    }
}

void rc_semaphore::rc_on_reset()
{
    rc_prim_channel::rc_on_reset();

    // reset the semaphore value
    m_value = m_reset_value;

    if (m_value > 0) {
        rc_notify(m_free);
    }
}

int rc_semaphore::wait()
{
    while(m_value <= 0) {
        rc_wait(m_free);
    }
    --m_value;
    return 0;
}

int rc_semaphore::trywait()
{
    if (m_value <= 0) {
        return -1;
    }
    --m_value;
    return 0;
}

int rc_semaphore::post()
{
    ++m_value;
    rc_notify(m_free);
    return 0;
}

} // namespace ReChannel

//
// $Id: rc_semaphore.cpp,v 1.3 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_semaphore.cpp,v $
//

