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
 * @file    rc_common_header.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:24:29 $
 * $Revision: 1.11 $
 */

#ifndef RC_COMMON_H_
#define RC_COMMON_H_

/*
 * Note: Define RC_DEBUG to enable ReChannel's debug message reporting.
 *       If RC_DEBUG is defined, constant RC_DEBUG_LEVEL sets the
 *          debug level (default is RC_DEBUG_LEVEL=3).
 */

#include <typeinfo>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/type_traits.hpp>

#include <systemc.h>

#include "ReChannel/util/rc_report.h"
#include "ReChannel/util/rc_throwable.h"
#include "ReChannel/util/rc_port_traits.h"

#include "ReChannel/core/rc_report_ids.h"

namespace ReChannel {

//using namespace sc_core;

/* global functions */

/**
 * @brief Returns the sc_object with the given name if exists
 *        otherwise returns the null pointer.
 *
 * @param name the name of the object to search for
 * @param rel_obj the base object used to resolve relative names
 *
 * The name of the object to be searched for corresponds to its
 * SystemC name. Names beginning with '.' or '#' are relative names
 * which are unknown to SystemC.
 * The absolute SystemC name is resolved in relation to the given
 * SystemC object rel_obj. A preceding '.' indicates a path down the
 * SystemC name tree. A preceding '#' indicates a path up the SystemC
 * name tree. Every '.' following the '#' moves the relative position
 * up by one. Example: A search for '#..x' with the rel_obj named
 * 'a.b.c' will return the module with the name 'a.x'. If the syntax
 * of the relative name is invalid it will report an error. 
 */
extern sc_object* rc_find_object(
    const char* obj_name, const sc_object* rel_obj);

extern sc_module* rc_find_parent_module(sc_object* obj);

/* internals */

namespace internals {

enum undef { UNDEF = 0 };

template<class T, bool B> class arg;

/**
 * @brief This class can be used to implement variable argument lists.
 */
template<class T>
class arg<T, true>
{
public:
    arg(T& arg_)    : p_arg(&arg_) {}
    arg(undef arg_) : p_arg(NULL) {}
    inline operator T*() const { return p_arg; }
private:
    T* p_arg;
};

template<class T>
class arg<T, false>
{
public:
    arg(undef arg_) {}
};

template<>
class arg<void, true>
{
public:
    arg(undef arg_) {}
};

} // namespace internals
} // namespace ReChannel

// macros to pass templated expressions as macro arguments
#define RC_PARAMS2(p1, p2) \
    p1, p2
#define RC_PARAMS3(p1, p2, p3) \
    p1, p2, p3
#define RC_PARAMS4(p1, p2, p3, p4) \
    p1, p2, p3, p4
#define RC_PARAMS5(p1, p2, p3, p4, p5) \
    p1, p2, p3, p4, p5
#define RC_PARAMS6(p1, p2, p3, p4, p5, p6) \
    p1, p2, p3, p4, p5, p6
#define RC_PARAMS7(p1, p2, p3, p4, p5, p6, p7) \
    p1, p2, p3, p4, p5, p6, p7
#define RC_PARAMS8(p1, p2, p3, p4, p5, p6, p7, p8) \
    p1, p2, p3, p4, p5, p6, p7, p8
#define RC_PARAMS9(p1, p2, p3, p4, p5, p6, p7, p8, p9) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9
#define RC_PARAMS10(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10

#define RC_STATIC_ASSERT_VALID_INTERFACE(if_type) \
    RC_STATIC_ASSERT_T( \
        interface_type_validity_check, \
        (boost::is_base_of<sc_interface, if_type >::value \
        && !boost::is_same<sc_interface, if_type >::value) \
    )

#define RC_STATIC_ASSERT_VALID_PORT(port_type) \
    RC_STATIC_ASSERT_T( \
        port_type_validity_check, \
        (boost::is_base_of<sc_port_base, port_type >::value \
        && !boost::is_same<sc_port_base, port_type >::value) \
    )

#endif //RC_COMMON_H_

//
// $Id: rc_common_header.h,v 1.11 2007/12/20 20:24:29 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_common_header.h,v $
//
