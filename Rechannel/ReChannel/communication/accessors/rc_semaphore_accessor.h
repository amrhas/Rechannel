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
 * @file    rc_semaphore_accessor.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Defines the accessor for the sc_semaphore channel.
 *
 * This file contains the accessor for the sc_semaphore_if interface
 * as well as the implementation of the respective rc_fallback_interface.
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.7 $
 */

#ifndef RC_SEMAPHORE_ACCESSOR_H_
#define RC_SEMAPHORE_ACCESSOR_H_

#include "ReChannel/communication/accessors/rc_accessor.h"

namespace ReChannel {

/**
 * @brief Fallback interface for a semaphore.
 */
template<>
class rc_fallback_interface<sc_semaphore_if>
: virtual public sc_semaphore_if
{
public:
    virtual int wait()
    {
        SC_REPORT_WARNING(RC_ID_UNDEFINED_BLOCKING_ACCESS_WARNING_, 0);
        ::sc_core::wait(p_undef);
        return 0;
    }
    virtual int trywait()
        { return -1; }
    virtual int post()
        { return 0; }
    virtual int get_value() const
        { return 0; }
private:
    sc_event p_undef;
};

/**
 * @brief Accessor for a semaphore.
 */
RC_ACCESSOR(sc_semaphore_if)
{
    RC_ACCESSOR_CTOR(sc_semaphore_if) { }

    virtual int wait()
    {
        return this->rc_forward(&if_type::wait);
    }
    virtual int trywait()
    {
        return this->rc_nb_forward(&if_type::trywait);
    }
    virtual int post()
    {
        return this->rc_nb_forward(&if_type::post);
    }
    virtual int get_value() const
    {
        return this->rc_nb_forward(&if_type::get_value);
    }
};

} // namespace ReChannel

#endif // RC_SEMAPHORE_ACCESSOR_H_
//
// $Id: rc_semaphore_accessor.h,v 1.7 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/accessors/rc_semaphore_accessor.h,v $
//

