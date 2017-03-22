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
 * @file    rc_portal.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Definition of class rc_portal.
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.17 $
 */

#ifndef RC_PORTAL_H_
#define RC_PORTAL_H_

#include "ReChannel/communication/portals/rc_abstract_portal.h"

namespace ReChannel {

/**
 * @brief The generic portal class.
 *
 * Portals are defined by specializing template rc_portal<IF>.
 */
template<class PORT>
class rc_portal
    : public rc_abstract_portal<PORT>
{
private:
    typedef rc_abstract_portal<PORT> base_type;
public:
    typedef PORT                              port_type;
    typedef typename base_type::if_type       if_type;
    typedef typename base_type::accessor_type accessor_type;

public:
    explicit rc_portal(
        const sc_module_name& module_name_ =
            sc_gen_unique_name("rc_portal"))
        : base_type(module_name_)
    { }
};

} // namespace ReChannel

/* portal construction macros */

#define RC_PORTAL(port_class_name) \
    template<> \
    struct rc_portal<port_class_name > \
        : public rc_abstract_portal<port_class_name >

#define RC_PORTAL_TEMPLATE(port_class_name) \
    struct rc_portal<port_class_name > \
        : public rc_abstract_portal<port_class_name >

#define RC_PORTAL_CTOR(port_class_name) \
    typedef port_class_name port_type; \
    typedef rc_abstract_portal<port_class_name >::if_type if_type; \
    typedef rc_abstract_portal<port_class_name >::accessor_type \
        accessor_type; \
    explicit rc_portal(const sc_module_name& module_name_ \
        = sc_gen_unique_name("rc_portal")) \
        : rc_abstract_portal<port_class_name >(module_name_)

#define RC_PORTAL_TEMPLATE_CTOR(port_class_name) \
    typedef port_class_name port_type; \
    typedef typename rc_abstract_portal<port_class_name >::if_type \
        if_type; \
    typedef typename rc_abstract_portal<port_class_name >::accessor_type \
        accessor_type; \
    explicit rc_portal(const sc_module_name& module_name_ \
        = sc_gen_unique_name("rc_portal")) \
        : rc_abstract_portal<port_class_name >(module_name_)

// note: using this macro requires the definition of 
//       the typedef 'if_type' at class scope
//       or the use of macro RC_PORTAL_CTOR
#define RC_PORTAL_FORWARD_EVENT(event_method_name) \
    this->add_event_forwarder( \
        &if_type::event_method_name, #event_method_name)

#endif // RC_PORTAL_H_
//
// $Id: rc_portal.h,v 1.17 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/portals/rc_portal.h,v $
//
