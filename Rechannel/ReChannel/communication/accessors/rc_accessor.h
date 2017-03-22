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
 * @file    rc_accessor.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Definition of class rc_abstract_accessor.
 *
 *
 * $Date: 2007/11/23 13:24:55 $
 * $Revision: 1.4 $
 */

#ifndef RC_ACCESSOR_H_
#define RC_ACCESSOR_H_

#include "ReChannel/communication/accessors/rc_abstract_accessor.h"

namespace ReChannel {

/**
 * @brief The generic accessor class.
 *
 * \note Accessors are defined by specializing template rc_accessor<IF>.
 */
template<class IF>
class rc_accessor
    : public rc_abstract_accessor<IF>
{
public:
    typedef IF if_type;

private:
    rc_accessor();
};

} // namespace ReChannel

#define RC_ACCESSOR(interface_type) \
    template<> \
    struct rc_accessor<interface_type > \
        : public rc_abstract_accessor<interface_type > \

#define RC_ACCESSOR_TEMPLATE(interface_type) \
    struct rc_accessor<interface_type > \
        : public rc_abstract_accessor<interface_type > \

#define RC_ACCESSOR_CTOR(interface_type) \
    typedef interface_type if_type; \
    rc_accessor() \
        : rc_abstract_accessor<if_type >()

#define RC_ACCESSOR_TEMPLATE_CTOR(interface_type) \
    RC_ACCESSOR_CTOR(interface_type)

#define RC_ON_TARGET_CHANGED() \
    virtual void rc_on_target_changed()

#define _RECHANNEL_EVENT_MEMBER(event_method_name) \
    _rc_p_event_##event_method_name

#define RC_EVENT(event_method_name) \
    virtual const sc_event& event_method_name() const \
    { \
        return _RECHANNEL_EVENT_MEMBER(event_method_name); \
    } \
private: \
    sc_event _RECHANNEL_EVENT_MEMBER(event_method_name); \
public: \
    /*dummy*/typedef sc_event _rc_event_dummy_##event_method_name##_type

#define RC_EVENT_ALIAS(event_method_name, alias) \
    virtual const sc_event& alias() const \
    { \
        return this->event_method_name(); \
    } \
    /*dummy*/typedef sc_event _rc_event_dummy_##alias##_type

#endif // RC_ACCESSOR_H_

//
// $Id: rc_accessor.h,v 1.4 2007/11/23 13:24:55 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/accessors/rc_accessor.h,v $
//
