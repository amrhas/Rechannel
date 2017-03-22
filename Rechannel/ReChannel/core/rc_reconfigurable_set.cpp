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
 * @file    rc_reconfigurable_set.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:39:40 $
 * $Revision: 1.4 $
 */

#include "rc_reconfigurable_set.h"
#include "rc_reconfigurable.h"
#include "ReChannel/core/rc_report_ids.h"

namespace ReChannel {

void rc_reconfigurable_set::erase(const rc_reconfigurable_set& reconf_set)
{
    for (const_iterator it = reconf_set.begin();
        it != reconf_set.end();
        ++it)
    {
        base_type::erase((*it));
    }
}

bool rc_reconfigurable_set::contains(const rc_reconfigurable_set& reconf_set) const
{
    for (const_iterator it = reconf_set.begin();
        it != reconf_set.end();
        ++it)
    {
        if (base_type::find(*it) == base_type::end()) {
            return false;
        }
    }
    return true;
}

} //namespace ReChannel

//
// $Id: rc_reconfigurable_set.cpp,v 1.4 2007/12/20 20:39:40 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_reconfigurable_set.cpp,v $
//

