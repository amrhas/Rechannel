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
 * @file    rc_join.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/10/09 00:22:26 $
 * $Revision: 1.4 $
 */

#include "rc_join.h"

namespace ReChannel {

rc_join::rc_join()
{ }

void rc_join::wait()
{
    if (p_hproc_vector.size() > 0) {
        if (p_hproc_vector.size() == 1) {
            rc_wait((*p_hproc_vector.begin()).terminated_event());
        } else { // p_hproc_vector.size() >= 2
            sc_event_and_list& event_and_list =
                (*p_hproc_vector.begin()).terminated_event()
                & (*(++p_hproc_vector.begin())).terminated_event();
            hproc_vector::iterator it;
            for (it = p_hproc_vector.begin() + 2;
                it != p_hproc_vector.end();
                ++it)
            {
                event_and_list & (*it).terminated_event();
            }
            rc_wait(event_and_list);
        }
    }
}

} // namespace ReChannel

//
// $Id: rc_join.cpp,v 1.4 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_join.cpp,v $
//

