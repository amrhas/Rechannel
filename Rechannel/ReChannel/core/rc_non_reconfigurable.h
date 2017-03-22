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
 * @file    rc_non_reconfigurable.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.4 $
 */

#ifndef rc_non_reconfigurable_H_
#define rc_non_reconfigurable_H_

#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/core/rc_control.h"

namespace ReChannel {

/**
 * @brief This class represents a non-reconfigurable reconfigurable.
 */
class rc_non_reconfigurable
    : private rc_control, public rc_reconfigurable
{
private:
    rc_non_reconfigurable()
        : rc_control("_rc_non_reconfigurable"),
          rc_reconfigurable(static_cast<sc_object*>(this))
    {
        add(*this);
        activate(*this);
    }

public:
    static rc_non_reconfigurable& get_instance()
    {
        if (s_dyn_obj == NULL) {
            s_dyn_obj = new rc_non_reconfigurable();
        }
        return *s_dyn_obj;
    }

private:
    static rc_non_reconfigurable* s_dyn_obj;
};

} // namespace ReChannel

#endif // rc_non_reconfigurable_H_

//
// $Id: rc_non_reconfigurable.h,v 1.4 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_non_reconfigurable.h,v $
//

