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
 * @file    rc_fallback_interface.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/06 11:46:32 $
 * $Revision: 1.3 $
 */

#ifndef RC_FALLBACK_INTERFACE_H_
#define RC_FALLBACK_INTERFACE_H_

#include <boost/type_traits/is_empty.hpp>
//#include "ReChannel/util/rc_report.h"

namespace ReChannel {

/**
 * @brief returns the default undefined value of type T
 */
template<class T>
inline
T rc_undefined_value()
{
    return T();
}

/**
 * @brief fallback interface template (skeleton)
 */
template<class IF>
class rc_fallback_interface
{
    // must be an empty class
};

/**
 * @brief fallback interface factory
 */
template<
    class IF, bool IF_EXISTS =
    !(boost::is_empty<rc_fallback_interface<IF> >::value)
>
class rc_fallback_interface_factory;

/**
 * @brief fallback interface factory (if fallback interface exists)
 */
template<class IF>
class rc_fallback_interface_factory<IF, true>
{
public:
    static inline IF* create()
        { return (IF*)(new rc_fallback_interface<IF>()); }

private:
    rc_fallback_interface_factory() {} //disabled
};

/**
 * @brief fallback interface factory (if fallback interface doesn't exist)
 */
template<class IF>
class rc_fallback_interface_factory<IF, false>
{
public:
    static inline IF* create()
        { return 0; }

private:
    rc_fallback_interface_factory() {} //disabled
};

} // namespace ReChannel

#define RC_UNDEFINED_VALUE(value_type, value) \
	namespace ReChannel { \
        template<> \
        inline value_type rc_undefined_value<value_type >() \
            { return (value); } \
	}

#define RC_FALLBACK_INTERFACE(interface_type) \
    template<> \
    struct rc_fallback_interface<interface_type> \
        : virtual public interface_type

#define RC_FALLBACK_INTERFACE_TEMPLATE(interface_type) \
    struct rc_fallback_interface<interface_type > \
        : virtual public interface_type

#define RC_FALLBACK_INTERFACE_CTOR(interface_type) \
    rc_fallback_interface()

#define RC_FALLBACK_INTERFACE_TEMPLATE_CTOR(interface_type) \
    RC_FALLBACK_INTERFACE_CTOR(interface_type)

#endif // RC_FALLBACK_INTERFACE_H_

//
// $Id: rc_fallback_interface.h,v 1.3 2007/12/06 11:46:32 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/rc_fallback_interface.h,v $
//
