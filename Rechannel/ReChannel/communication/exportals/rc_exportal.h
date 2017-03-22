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
 * @file    rc_exportal.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Template rc_exportal.
 *
 *
 * $Date: 2007/11/23 13:24:55 $
 * $Revision: 1.6 $
 */

#ifndef RC_EXPORTAL_H_
#define RC_EXPORTAL_H_

#include "ReChannel/communication/exportals/rc_abstract_exportal.h"

namespace ReChannel {

/**
 * @brief The generic exportal switch.
 *
 * \note Exportals are defined by specializing template rc_exportal<IF>.
 */
template<class IF>
class rc_exportal
    : public rc_abstract_exportal<IF>
{
public:
    typedef IF              if_type;
    typedef rc_accessor<IF> accessor_type;

private:
    rc_exportal();
    explicit rc_exportal(const sc_module_name& module_name_);
};

} // namespace ReChannel

/* exportal construction macros */

#define RC_EXPORTAL(interface_name) \
    template<> \
    struct rc_exportal<interface_name > \
        : public rc_abstract_exportal<interface_name >

#define RC_EXPORTAL_TEMPLATE(interface_name) \
    struct rc_exportal<interface_name > \
        : public rc_abstract_exportal<interface_name >

#define RC_EXPORTAL_CTOR(interface_name) \
    typedef interface_name if_type; \
    typedef rc_accessor<interface_name > accessor_type; \
    explicit rc_exportal( \
        const sc_module_name& module_name_ \
        = sc_gen_unique_name("rc_exportal")) \
        : rc_abstract_exportal<interface_name >(module_name_)

#define RC_EXPORTAL_TEMPLATE_CTOR(interface_name) \
    RC_EXPORTAL_CTOR(interface_name)

// note: using this macro requires the definition of 
//       the typedef 'if_type' at class scope
//       or the use of RC_EXPORTAL_CTOR
#define RC_EXPORTAL_FORWARD_EVENT(event_method_name) \
    this->add_event_forwarder( \
        &if_type::event_method_name, #event_method_name)

#endif // RC_EXPORTAL_H_
//
// $Id: rc_exportal.h,v 1.6 2007/11/23 13:24:55 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/exportals/rc_exportal.h,v $
//
