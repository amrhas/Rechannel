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
 * @file    rc_portmap.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.4 $
 */

#ifndef RC_PORTMAP_H_
#define RC_PORTMAP_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_report_ids.h"
#include "ReChannel/util/rc_object_handle.h"
#include "ReChannel/util/rc_port_handle.h"
#include "ReChannel/util/rc_export_handle.h"

#include <boost/type_traits/is_base_of.hpp>

namespace ReChannel {

/**
 * @brief Base class of a port map.
 */
class rc_portmap_base
{
private:
    typedef rc_portmap_base this_type;

public:
    enum { portmap_size = 0 };

protected:
    rc_portmap_base()
        : p_arr(NULL), p_size(0)
        { }

public:
    int size() const
        { return p_size; }

    rc_object_handle get(int index) const;

    rc_object_handle operator[](int index) const
        { return this->get(index); }

    sc_object& get_object(int index) const;

    sc_interface* get_interface(int index) const;

    virtual bool is_compatible(this_type& portmap) const = 0;

protected:

    virtual ~rc_portmap_base() {}

/* for internal use only */

    void _rc_init(rc_object_handle arr[], int size);

private:
    rc_object_handle* p_arr;
    int               p_size;
};

/**
 * @brief The class of a port map.
 */
template<
    class ID, class P0,
    class P1=void,  class P2=void,  class P3=void,  class P4=void,
    class P5=void,  class P6=void,  class P7=void,  class P8=void,
    class P9=void,  class P10=void, class P11=void, class P12=void,
    class P13=void, class P14=void, class P15=void, class P16=void,
    class P17=void, class P18=void, class P19=void, class P20=void,
    class P21=void, class P22=void, class P23=void, class P24=void,
    class P25=void, class P26=void, class P27=void, class P28=void,
    class P29=void, class P30=void, class P31=void, class P32=void,
    class P33=void, class P34=void, class P35=void, class P36=void,
    class P37=void, class P38=void, class P39=void, class P40=void,
    class P41=void, class P42=void, class P43=void, class P44=void,
    class P45=void, class P46=void, class P47=void, class P48=void,
    class P49=void, class P50=void, class P51=void, class P52=void,
    class P53=void, class P54=void, class P55=void, class P56=void,
    class P57=void, class P58=void, class P59=void, class P60=void,
    class P61=void, class P62=void>
class rc_portmap
    : public rc_portmap_base
{
private:
    typedef rc_portmap<
                ID,
                P0,  P1,  P2,  P3,  P4,  P5,  P6,  P7,  P8,  P9,
                P10, P11, P12, P13, P14, P15, P16, P17, P18, P19,
                P20, P21, P22, P23, P24, P25, P26, P27, P28, P29,
                P30, P31, P32, P33, P34, P35, P36, P37, P38, P39,
                P40, P41, P42, P43, P44, P45, P46, P47, P48, P49,
                P50, P51, P52, P53, P54, P55, P56, P57, P58, P59,
                P60, P61, P62> this_type;
    typedef rc_portmap_base base_type;

    #define _RC_TMP_undef_check(class_name) \
        !(boost::is_base_of<sc_port_base, class_name>::value == true \
          || boost::is_base_of<sc_export_base, class_name>::value == true \
          || (boost::is_base_of<sc_object, class_name>::value == true \
            && boost::is_base_of<sc_interface, class_name>::value == true))
    enum {
        N = (_RC_TMP_undef_check(P0) ?  0 :(_RC_TMP_undef_check(P1) ? 1  :
            (_RC_TMP_undef_check(P2) ?  2 :(_RC_TMP_undef_check(P3) ? 3  :
            (_RC_TMP_undef_check(P4) ?  4 :(_RC_TMP_undef_check(P5) ? 5  :
            (_RC_TMP_undef_check(P6) ?  6 :(_RC_TMP_undef_check(P7) ? 7  :
            (_RC_TMP_undef_check(P8) ?  8 :(_RC_TMP_undef_check(P9) ? 9  :
            (_RC_TMP_undef_check(P10)? 10 :(_RC_TMP_undef_check(P11)? 11 :
            (_RC_TMP_undef_check(P12)? 12 :(_RC_TMP_undef_check(P13)? 13 :
            (_RC_TMP_undef_check(P14)? 14 :(_RC_TMP_undef_check(P15)? 15 :
            (_RC_TMP_undef_check(P16)? 16 :(_RC_TMP_undef_check(P17)? 17 :
            (_RC_TMP_undef_check(P18)? 18 :(_RC_TMP_undef_check(P19)? 19 :
            (_RC_TMP_undef_check(P20)? 20 :(_RC_TMP_undef_check(P21)? 21 :
            (_RC_TMP_undef_check(P22)? 22 :(_RC_TMP_undef_check(P23)? 23 :
            (_RC_TMP_undef_check(P24)? 24 :(_RC_TMP_undef_check(P25)? 25 :
            (_RC_TMP_undef_check(P26)? 26 :(_RC_TMP_undef_check(P27)? 27 :
            (_RC_TMP_undef_check(P28)? 28 :(_RC_TMP_undef_check(P29)? 29 :
            (_RC_TMP_undef_check(P30)? 30 :(_RC_TMP_undef_check(P31)? 31 :
            (_RC_TMP_undef_check(P32)? 32 :(_RC_TMP_undef_check(P33)? 33 :
            (_RC_TMP_undef_check(P34)? 34 :(_RC_TMP_undef_check(P35)? 35 :
            (_RC_TMP_undef_check(P36)? 36 :(_RC_TMP_undef_check(P37)? 37 :
            (_RC_TMP_undef_check(P38)? 38 :(_RC_TMP_undef_check(P39)? 39 :
            (_RC_TMP_undef_check(P40)? 40 :(_RC_TMP_undef_check(P41)? 41 :
            (_RC_TMP_undef_check(P42)? 42 :(_RC_TMP_undef_check(P43)? 43 :
            (_RC_TMP_undef_check(P44)? 44 :(_RC_TMP_undef_check(P45)? 45 :
            (_RC_TMP_undef_check(P46)? 46 :(_RC_TMP_undef_check(P47)? 47 :
            (_RC_TMP_undef_check(P48)? 48 :(_RC_TMP_undef_check(P49)? 49 :
            (_RC_TMP_undef_check(P50)? 50 :(_RC_TMP_undef_check(P51)? 51 :
            (_RC_TMP_undef_check(P52)? 52 :(_RC_TMP_undef_check(P53)? 53 :
            (_RC_TMP_undef_check(P54)? 54 :(_RC_TMP_undef_check(P55)? 55 :
            (_RC_TMP_undef_check(P56)? 56 :(_RC_TMP_undef_check(P57)? 57 :
            (_RC_TMP_undef_check(P58)? 58 :(_RC_TMP_undef_check(P59)? 59 :
            (_RC_TMP_undef_check(P60)? 60 :(_RC_TMP_undef_check(P61)? 61 :
            (_RC_TMP_undef_check(P62)? 62 : 63
	        )))))))))))))))))))))))))))))))
            ))))))))))))))))))))))))))))))))
    };
    #undef _RC_TMP_undef_check

public:
    enum { portmap_size = N };
    RC_STATIC_ASSERT_T(portmap_size_check, portmap_size > 0);

public:

    #define _RC_TMP_arg(i) \
         const typename internals::template arg<P##i, (bool)(i < N)>& \
            p##i=internals::UNDEF

    inline rc_portmap(
        P0& p0,
        _RC_TMP_arg(1),  _RC_TMP_arg(2),  _RC_TMP_arg(3),  _RC_TMP_arg(4),
        _RC_TMP_arg(5),  _RC_TMP_arg(6),  _RC_TMP_arg(7),  _RC_TMP_arg(8),
        _RC_TMP_arg(9),  _RC_TMP_arg(10), _RC_TMP_arg(11), _RC_TMP_arg(12),
        _RC_TMP_arg(13), _RC_TMP_arg(14), _RC_TMP_arg(15), _RC_TMP_arg(16),
        _RC_TMP_arg(17), _RC_TMP_arg(18), _RC_TMP_arg(19), _RC_TMP_arg(20),
        _RC_TMP_arg(21), _RC_TMP_arg(22), _RC_TMP_arg(23), _RC_TMP_arg(24),
        _RC_TMP_arg(25), _RC_TMP_arg(26), _RC_TMP_arg(27), _RC_TMP_arg(28),
        _RC_TMP_arg(29), _RC_TMP_arg(30), _RC_TMP_arg(31), _RC_TMP_arg(32),
        _RC_TMP_arg(33), _RC_TMP_arg(34), _RC_TMP_arg(35), _RC_TMP_arg(36),
        _RC_TMP_arg(37), _RC_TMP_arg(38), _RC_TMP_arg(39), _RC_TMP_arg(40),
        _RC_TMP_arg(41), _RC_TMP_arg(42), _RC_TMP_arg(43), _RC_TMP_arg(44),
        _RC_TMP_arg(45), _RC_TMP_arg(46), _RC_TMP_arg(47), _RC_TMP_arg(48),
        _RC_TMP_arg(49), _RC_TMP_arg(50), _RC_TMP_arg(51), _RC_TMP_arg(52),
        _RC_TMP_arg(53), _RC_TMP_arg(54), _RC_TMP_arg(55), _RC_TMP_arg(56),
        _RC_TMP_arg(57), _RC_TMP_arg(58), _RC_TMP_arg(59), _RC_TMP_arg(60),
        _RC_TMP_arg(61), _RC_TMP_arg(62))
    {
        p_arr[0] = p0;
        this->_rc_set(1,  p1);  this->_rc_set(2,  p2);
        this->_rc_set(3,  p3);  this->_rc_set(4,  p4);
        this->_rc_set(5,  p5);  this->_rc_set(6,  p6);
        this->_rc_set(7,  p7);  this->_rc_set(8,  p8);
        this->_rc_set(9,  p9);  this->_rc_set(10, p10);
        this->_rc_set(11, p11); this->_rc_set(12, p12);
        this->_rc_set(13, p13); this->_rc_set(14, p14);
        this->_rc_set(15, p15); this->_rc_set(16, p16);
        this->_rc_set(17, p17); this->_rc_set(18, p18);
        this->_rc_set(19, p19); this->_rc_set(20, p20);
        this->_rc_set(21, p21); this->_rc_set(22, p22);
        this->_rc_set(23, p23); this->_rc_set(24, p24);
        this->_rc_set(25, p25); this->_rc_set(26, p26);
        this->_rc_set(27, p27); this->_rc_set(28, p28);
        this->_rc_set(29, p29); this->_rc_set(30, p30);
        this->_rc_set(31, p31); this->_rc_set(32, p32);
        this->_rc_set(33, p33); this->_rc_set(34, p34);
        this->_rc_set(35, p35); this->_rc_set(36, p36);
        this->_rc_set(37, p37); this->_rc_set(38, p38);
        this->_rc_set(39, p39); this->_rc_set(40, p40);
        this->_rc_set(41, p41); this->_rc_set(42, p42);
        this->_rc_set(43, p43); this->_rc_set(44, p44);
        this->_rc_set(45, p45); this->_rc_set(46, p46);
        this->_rc_set(47, p47); this->_rc_set(48, p48);
        this->_rc_set(49, p49); this->_rc_set(50, p50);
        this->_rc_set(51, p51); this->_rc_set(52, p52);
        this->_rc_set(53, p53); this->_rc_set(54, p54);
        this->_rc_set(55, p55); this->_rc_set(56, p56);
        this->_rc_set(57, p57); this->_rc_set(58, p58);
        this->_rc_set(59, p59); this->_rc_set(60, p60);
        this->_rc_set(61, p61); this->_rc_set(62, p62);
        base_type::_rc_init(p_arr, N);
    }
    #undef _RC_TMP_arg

    virtual bool is_compatible(base_type& portmap) const
        { return (typeid(portmap) == typeid(*this)); }

private:
    template<class T>
    inline void _rc_set(
        int i_, const typename internals::template arg<T, true>& arg_)
    {
        T* value_ = arg_;
        if (value_ != NULL) {
            p_arr[i_] = *value_;
        }
    }

    template<class T>
    inline void _rc_set(
        int i_,
        const typename internals::template arg<T, false>& arg_) const
    { }

    inline void _rc_set(
        int i_,
        const typename internals::template arg<void, true>& arg_) const
    { }

private:
    rc_object_handle p_arr[(N > 0 ? N : 1)];
};

} // namespace ReChannel

// check for variadic macro support
#if (defined(_MSC_VER) && _MSC_VER < 1400) /* Visual Studio .NET 2003 */

#include "ReChannel/core/rc_portmap_macros.h"

// note: 'port_list_' is a bracketed list of comma separated (ex)port types
#define _RC_DECLARE_PORTMAP(name_, portmap_size_, port_list_) \
    struct _RC_PORTMAP_TYPE_ID_##name_; \
    typedef ::ReChannel::rc_portmap< \
            _RC_PORTMAP_TYPE_ID_##name_, \
            _RC_PM_PARAMS_##portmap_size_ port_list_ \
        > name_/*;*/

// note: 'port_list_' is a bracketed list of comma separated (ex)port types
#define _RC_DECLARE_PORTMAP_T(name_, portmap_size_, port_list_) \
    struct _RC_PORTMAP_TYPE_ID_##name_; \
    typedef typename ::ReChannel::template rc_portmap< \
            _RC_PORTMAP_TYPE_ID_##name_, \
            _RC_PM_PARAMS_##portmap_size_ port_list_ \
        > name_/*;*/

#else /* other compilers */

// note: 'port_list' is a bracketed list of comma separated (ex)port types
#define _RC_DECLARE_PORTMAP(name_, portmap_size_, port_types_) \
    struct _RC_PORTMAP_TYPE_ID_##name_; \
    typedef ::ReChannel::rc_portmap< \
            _RC_PORTMAP_TYPE_ID_##name_, \
            _RC_PORTMAP_REMOVE_BRACKETS port_types_ \
        > name_; \
    RC_STATIC_ASSERT(portmap_validity_check, \
            (portmap_size_ == name_::portmap_size))/*;*/

// note: 'port_list_' is a bracketed list of comma separated (ex)port types
#define _RC_DECLARE_PORTMAP_T(name_, portmap_size_, port_types_) \
    struct _RC_PORTMAP_TYPE_ID_##name_; \
    typedef typename ::ReChannel::template rc_portmap< \
            _RC_PORTMAP_TYPE_ID_##name_, \
            _RC_PORTMAP_REMOVE_BRACKETS port_types_ \
        > name_; \
    RC_STATIC_ASSERT_T(portmap_validity_check, \
            (portmap_size_ == name_::portmap_size))/*;*/

#define _RC_PORTMAP_REMOVE_BRACKETS(...) __VA_ARGS__

#endif // (defined(_MSC_VER) && _MSC_VER < 1400)

#define RC_PORTMAP(name_, portmap_size_, port_types_) \
    _RC_DECLARE_PORTMAP(name_, portmap_size_, port_types_)

#define RC_PORTMAP_T(name_, portmap_size_, port_types_) \
    _RC_DECLARE_PORTMAP_T(name_, portmap_size_, port_types_)

#define RC_PORT_TYPES /*just a keyword*/

#define RC_PORT_TYPES_T /*just a keyword*/

#endif // RC_PORTMAP_H_

//
// $Id: rc_portmap.h,v 1.4 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_portmap.h,v $
//
