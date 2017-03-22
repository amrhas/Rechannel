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
 * @file    rc_port_traits.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class rc_port_traits and specialisations.
 *
 * The traits class rc_port_traits is used to determine the interface type from
 * a port type. The SystemC language lacks a way to do this.
 * If no appropriate rc_port_traits is available for a custom port it has to be
 * defined by the designer. However, for all build-in SystemC ports as well as
 * for all custom ports that provide the typedef if_type, no additional
 * rc_port_traits is needed.
 *
 * $Date: 2007/10/09 00:22:26 $
 * $Revision: 1.3 $
 */

#ifndef RC_PORT_TRAITS_H_
#define RC_PORT_TRAITS_H_

#include <systemc.h>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>

#include "ReChannel/util/rc_report.h"

namespace ReChannel {

/** \typedef rc_port_traits::if_type
 * @brief The interface type of PORT.
 */
/** \typedef rc_port_traits::type
 * @brief The type of PORT.
 */

/** \class rc_port_traits
 *  @brief generic port traits template
 *
 * Class rc_port_traits is used to determine the interface type from a
 * port type. The SystemC language lacks a way to do this.
 * If no appropriate rc_port_traits is available for a custom port it has to be
 * defined by the designer. However, for all build-in SystemC ports as well as
 * for all custom ports that provide the typedef if_type, no additional
 * rc_port_traits is needed.
 */

/**
 * \defgroup grp_generic_port_traits Generic port traits
 */
/*@{*/

template<class PORT>
struct rc_port_traits
{
    /** \brief checks if PORT is a valid port class */
    RC_STATIC_ASSERT_T(
        port_type_validity_check,
        (boost::is_base_of<sc_port_base, PORT>::value
        && !boost::is_same<sc_port_base, PORT>::value));
    /** \brief checks if typedef PORT::if_type does exist */
    RC_STATIC_ASSERT_T(
        interface_typedef_availability_check,
        (boost::is_base_of<
            sc_interface, typename PORT::if_type>::value));

    typedef PORT type;
    typedef typename PORT::if_type if_type;
};

template<class IF, int N, sc_port_policy POL>
struct rc_port_traits<sc_port<IF, N, POL> >
{
    typedef sc_port<IF, N, POL> type;
    typedef IF if_type;
};

/*@}*/

/**
 * \defgroup grp_specialised_port_traits Specialized port traits
 */
/*@{*/

template<class T>
struct rc_port_traits<sc_fifo_in<T> >
{
    typedef sc_fifo_in<T>    type;
    typedef sc_fifo_in_if<T> if_type;
};

template<class T>
struct rc_port_traits<sc_fifo_out<T> >
{
    typedef sc_fifo_out<T>    type;
    typedef sc_fifo_out_if<T> if_type;
};

template<class T>
struct rc_port_traits<sc_in<T> >
{
    typedef sc_in<T>           type;
    typedef sc_signal_in_if<T> if_type;
};

template<class T>
struct rc_port_traits<sc_inout<T> >
{
    typedef sc_inout<T>           type;
    typedef sc_signal_inout_if<T> if_type;
};

template<class T>
struct rc_port_traits<sc_out<T> >
{
    typedef sc_out<T>           type;
    typedef sc_signal_out_if<T> if_type;
};

template<>
struct rc_port_traits<sc_in_resolved>
{
    typedef sc_in_resolved type;
    typedef sc_signal_in_if<sc_dt::sc_logic> if_type;
};

template<>
struct rc_port_traits<sc_inout_resolved>
{
    typedef sc_inout_resolved type;
    typedef sc_signal_inout_if<sc_dt::sc_logic> if_type;
};

template<>
struct rc_port_traits<sc_out_resolved>
{
    typedef sc_out_resolved type;
    typedef sc_signal_out_if<sc_dt::sc_logic> if_type;
};

template<int W>
struct rc_port_traits<sc_in_rv<W> >
{
    typedef sc_in_rv<W> type;
    typedef sc_signal_in_if<sc_dt::sc_lv<W> > if_type;
};

template<int W>
struct rc_port_traits<sc_inout_rv<W> >
{
    typedef sc_inout_rv<W> type;
    typedef sc_signal_inout_if<sc_dt::sc_lv<W> > if_type;
};

template<int W>
struct rc_port_traits<sc_out_rv<W> >
{
    typedef sc_out_rv<W> type;
    typedef sc_signal_out_if<sc_dt::sc_lv<W> > if_type;
};

/*@}*/

} // namespace ReChannel

#endif // RC_PORT_TRAITS_H_

//
// $Id: rc_port_traits.h,v 1.3 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_port_traits.h,v $
//
