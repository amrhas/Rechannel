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
 * @file    rc_var.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/06 11:42:43 $
 * $Revision: 1.8 $
 */

#include "rc_var.h"

namespace ReChannel {
namespace internals {
namespace resettable_var {

resettable_var_base::resettable_var_base()
{
    /* scope brackets (=> delete tmp as soon as possible) */ {
        const std::auto_ptr<tmp_object> tmp(new tmp_object());
        m_parent = tmp->get_parent_object();
    }
    if (m_parent == NULL) {
        report_invalid_use_error();
        return;
    }
    rc_register_resettable(
        *this, reinterpret_cast<sc_object*>(m_parent));
}

void resettable_var_base::report_invalid_use_error()
{
    RC_REPORT_ERROR(RC_ID_INTERNAL_ERROR_,
            "invalid usage of rc_var object or broken module hierarchy");
}

} // namespace resettable_var
} // namespace internals
} // namespace ReChannel

//
// $Id: rc_var.cpp,v 1.8 2007/12/06 11:42:43 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_var.cpp,v $
//

