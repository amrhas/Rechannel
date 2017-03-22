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
 * @file    rc_switch_connector.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class rc_switch_connector<PortMap>.
 *
 * In this file the classes rc_switch_connector_base and
 * rc_switch_connector<PortMap> are defined.
 *
 * A switch connector is a utility object that simplifies the binding of
 * switches. The static side as well as the dynamic side can be bound by
 * using the methods bind_static() or bind_dynamic(), respectively.
 * A switch connector can be bound to port maps and rc_reconfigurable
 * instances that are compatible.
 *
 * \see ReChannel::rc_switch_connector_base, ReChannel::rc_switch_connector,
 *      ReChannel::rc_reconfigurable, ReChannel::rc_portmap_base,
 *      ReChannel::rc_portmap
 *
 * $Date: 2007/12/06 09:37:47 $
 * $Revision: 1.7 $
 */

#ifndef RC_SWITCH_CONNECTOR_H_
#define RC_SWITCH_CONNECTOR_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_switch.h"
#include "ReChannel/core/rc_report_ids.h"
#include "ReChannel/core/rc_portmap.h"

#include <boost/type_traits/is_base_of.hpp>

namespace ReChannel {

class rc_control;
class rc_reconfigurable;

/**
 * @brief Base class of rc_switch_connector<PortMap>.
 *
 * This is the base class of rc_switch_connector<PortMap>.
 *
 * The base class provides all methods necessary for binding.
 *
 * \see rc_switch_connector, rc_portmap_base, rc_portmap,
 *      rc_reconfigurable
 */
class rc_switch_connector_base
    : public sc_prim_channel
{
private:
    typedef rc_switch_connector_base this_type;

protected:
    /**
     * @brief Constructor.
     * @param name the name of the switch connector.
     */
    rc_switch_connector_base(const char* name)
        : sc_prim_channel(name), p_switch_arr(NULL), p_size(0)
        { }

public:

    /**
     * @brief The size of this switch connector.
     *
     * Equals the number of switches (respectively the size of the port map).
     * \see rc_switch_connector
     */
    int size() const
        { return p_size; }

    /**
     * @brief Get switch at given index.
     * @param index number in the range of 0 to p_size
     */
    rc_switch& get(int index) const;

    /**
     * @brief Convenience operator. Same as get().
     */
    rc_switch& operator[](int index) const
        { return this->get(index); }

    /**
     * @brief Tests for compatibility with the given port map.
     * \remark Is overridden by the actual switch connector implementation.
     */
    virtual bool is_compatible(const rc_portmap_base& portmap) const = 0;

    /**
     * @brief Bind the switches to a compatible static port map.
     */
    void bind_static(const rc_portmap_base& portmap);

    /**
     * @brief Bind the switches to a compatible dynamic port map.
     */
    void bind_dynamic(const rc_portmap_base& portmap);

    /**
     * @brief Bind the switches to a compatible reconfigurable object.
     */
    void bind_dynamic(rc_reconfigurable& reconf);

protected:

    /**
     * @brief Virtual destructor.
     */
    virtual ~rc_switch_connector_base() {}

protected:

/* for internal use only */

    /**
     * @brief Initialise. (internal only)
     * \attention Shall be called once by the constructor of a super class.
     */
    void _rc_init(rc_switch* switch_arr[], int size);

private:
    /**
     * @brief The array of managed switches.
     * \note Is initialised by _rc_init().
     */
    rc_switch** p_switch_arr;
    /**
     * @brief The number of managed switches.
     * \note Is initialised by _rc_init().
     */
    int         p_size;

private:
    // disabled
    /** @brief (disabled) */
    rc_switch_connector_base(const this_type& other);
    /** @brief (disabled) */
    this_type& operator=(const this_type& other);
};

/**
 * @brief Class rc_switch_connector<PortMap>.
 *
 * A switch connector is a utility object that simplifies the binding of
 * switches. The static side as well as the dynamic side can be bound by
 * using the methods bind_static() or bind_dynamic(), respectively.
 * A switch connector can be bound to port maps and rc_reconfigurable
 * instances that are compatible.
 *
 * Example:
 * \code
 * // create the switch connector
 * rc_switch_connector<myPortMap> connector(
 *     "connector", clk_portal, reset_portal, A_portal, B_portal, C_portal);
 *
 * // bind static ports and channels
 * connector.bind_static(
 *     myPortMap_static(clk_port, reset_signal, A_fifo, B_fifo, C_fifo));
 * // bind reconfigurable modules
 * connector.bind_dynamic(m1);
 * connector.bind_dynamic(m2);
 * \endcode
 *
 * \note Binding of a reconfigurable module requires that a compatible port map
 * has previously been registered at this module (see
 * rc_reconfigurable::rc_add_portmap()).
 *
 * \see rc_switch_connector_base, rc_portmap_base, rc_portmap,
 *      rc_reconfigurable, rc_reconfigurable::rc_add_portmap()
 */
template<class DynPortMap>
class rc_switch_connector
    : public rc_switch_connector_base
{
private:
    /**
     * @brief check for DynPortMap is a valid port map type.
     */
    RC_STATIC_ASSERT_T(
        portmap_type_check,
        (boost::is_base_of<rc_portmap_base, DynPortMap>::value == true));

    /**
     * @brief check for DynPortMap is of a size greater than zero.
     */
    RC_STATIC_ASSERT_T(portmap_size_check, DynPortMap::portmap_size > 0);

    typedef rc_switch_connector<DynPortMap> this_type;
    typedef rc_switch_connector_base        base_type;

    /**
     * @brief A valid switch argument.
     */
    typedef typename internals::template arg<rc_switch, true>
        switch_arg;
    /**
     * @brief An undefined argument.
     */
    typedef typename internals::template arg<rc_switch, false>
        undef_arg;

public:
    /**
     * @brief The type of the port map.
     */
    typedef DynPortMap portmap_type;

public:
    /**
     * @brief portmap_size
     *
     * \note portmap_size equals the declared number of switches
     */
    enum { portmap_size = DynPortMap::portmap_size };

public:
    /**
     * @brief Convenience macro to declare variable constructor arguments
     * of type rc_switch. (internal only)
     *
     * This macro is used to enforce a particular argument count in the
     * constructor of class ReChannel::rc_switch_connector<DynPortMap>.
     * An argument of index i can be used like an rc_switch if i is less
     * than portmap_size. Otherwise the argument type represents an
     * undefined argument. 
     *
     * \see ReChannel::rc_switch_connector,
     *      ReChannel::rc_switch_connector<DynPortMap>::rc_switch_connector
     */
    #define _RC_Tmp_arg(i) \
        const typename internals::template arg< \
                rc_switch, (bool)(i < portmap_size)>& \
            s##i=internals::UNDEF

    /**
     * @brief Constructor.
     * @param name the name of the switch connector
     * @param s0 the first switch
     * @param sX (variable arguments)
     *
     * \remark The parameter count depends on the size of port map DynPortMap.
     * \see _RC_Tmp_arg()
     */
    inline rc_switch_connector(
        const char* name,
        rc_switch& s0,   _RC_Tmp_arg(1),  _RC_Tmp_arg(2),  _RC_Tmp_arg(3),
        _RC_Tmp_arg(4),  _RC_Tmp_arg(5),  _RC_Tmp_arg(6),  _RC_Tmp_arg(7),
        _RC_Tmp_arg(8),  _RC_Tmp_arg(9),  _RC_Tmp_arg(10), _RC_Tmp_arg(11),
        _RC_Tmp_arg(12), _RC_Tmp_arg(13), _RC_Tmp_arg(14), _RC_Tmp_arg(15),
        _RC_Tmp_arg(16), _RC_Tmp_arg(17), _RC_Tmp_arg(18), _RC_Tmp_arg(19),
        _RC_Tmp_arg(20), _RC_Tmp_arg(21), _RC_Tmp_arg(22), _RC_Tmp_arg(23),
        _RC_Tmp_arg(24), _RC_Tmp_arg(25), _RC_Tmp_arg(26), _RC_Tmp_arg(27),
        _RC_Tmp_arg(28), _RC_Tmp_arg(29), _RC_Tmp_arg(30), _RC_Tmp_arg(31),
        _RC_Tmp_arg(32), _RC_Tmp_arg(33), _RC_Tmp_arg(34), _RC_Tmp_arg(35),
        _RC_Tmp_arg(36), _RC_Tmp_arg(37), _RC_Tmp_arg(38), _RC_Tmp_arg(39),
        _RC_Tmp_arg(40), _RC_Tmp_arg(41), _RC_Tmp_arg(42), _RC_Tmp_arg(43),
        _RC_Tmp_arg(44), _RC_Tmp_arg(45), _RC_Tmp_arg(46), _RC_Tmp_arg(47),
        _RC_Tmp_arg(48), _RC_Tmp_arg(49), _RC_Tmp_arg(50), _RC_Tmp_arg(51),
        _RC_Tmp_arg(52), _RC_Tmp_arg(53), _RC_Tmp_arg(54), _RC_Tmp_arg(55),
        _RC_Tmp_arg(56), _RC_Tmp_arg(57), _RC_Tmp_arg(58), _RC_Tmp_arg(59),
        _RC_Tmp_arg(60), _RC_Tmp_arg(61), _RC_Tmp_arg(62))
        : base_type(name)
    {
        p_switch_arr[0] = &s0;
        // the following initialisation block can be optimised by compilers
        this->_rc_set(1,  s1);  this->_rc_set(2,  s2);
        this->_rc_set(3,  s3);  this->_rc_set(4,  s4);
        this->_rc_set(5,  s5);  this->_rc_set(6,  s6);
        this->_rc_set(7,  s7);  this->_rc_set(8,  s8);
        this->_rc_set(9,  s9);  this->_rc_set(10, s10);
        this->_rc_set(11, s11); this->_rc_set(12, s12);
        this->_rc_set(13, s13); this->_rc_set(14, s14);
        this->_rc_set(15, s15); this->_rc_set(16, s16);
        this->_rc_set(17, s17); this->_rc_set(18, s18);
        this->_rc_set(19, s19); this->_rc_set(20, s20);
        this->_rc_set(21, s21); this->_rc_set(22, s22);
        this->_rc_set(23, s23); this->_rc_set(24, s24);
        this->_rc_set(25, s25); this->_rc_set(26, s26);
        this->_rc_set(27, s27); this->_rc_set(28, s28);
        this->_rc_set(29, s29); this->_rc_set(30, s30);
        this->_rc_set(31, s31); this->_rc_set(32, s32);
        this->_rc_set(33, s33); this->_rc_set(34, s34);
        this->_rc_set(35, s35); this->_rc_set(36, s36);
        this->_rc_set(37, s37); this->_rc_set(38, s38);
        this->_rc_set(39, s39); this->_rc_set(40, s40);
        this->_rc_set(41, s41); this->_rc_set(42, s42);
        this->_rc_set(43, s43); this->_rc_set(44, s44);
        this->_rc_set(45, s45); this->_rc_set(46, s46);
        this->_rc_set(47, s47); this->_rc_set(48, s48);
        this->_rc_set(49, s49); this->_rc_set(50, s50);
        this->_rc_set(51, s51); this->_rc_set(52, s52);
        this->_rc_set(53, s53); this->_rc_set(54, s54);
        this->_rc_set(55, s55); this->_rc_set(56, s56);
        this->_rc_set(57, s57); this->_rc_set(58, s58);
        this->_rc_set(59, s59); this->_rc_set(60, s60);
		this->_rc_set(61, s61); this->_rc_set(62, s62);
        // init base class
        base_type::_rc_init(p_switch_arr, portmap_size);
    }
    #undef _RC_Tmp_arg

    /**
     * @brief check given port map for compatibility
     */
    virtual bool is_compatible(const rc_portmap_base& portmap) const
        { return (typeid(portmap) == typeid(DynPortMap)); }

private:

    /**
     * @brief Store the switch from a valid argument.
     */
    inline void _rc_set(int i_, const switch_arg& arg_)
        { p_switch_arr[i_] = arg_; }

    /**
     * @brief Undefined arguments are ignored.
     * \note (does nothing)
     */
    inline void _rc_set(int i_, const undef_arg& arg_) const
        { }

private:
    /**
     * @brief the array containing the pointers to the switches
     *
     * \note this array's size equals portmap_size
     */
    rc_switch* p_switch_arr[portmap_size];
};

} // namespace ReChannel

#endif // RC_SWITCH_CONNECTOR_H_

//
// $Id: rc_switch_connector.h,v 1.7 2007/12/06 09:37:47 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_switch_connector.h,v $
//
