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
 * @file    rc_member_func.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class family rc_member_func<...>
 *
 * The class family rc_member_func<...> represents a method call object.
 * rc_member_func can be called like a function, as the object (the first
 * parameter) is implicitly bound to the call.
 *
 * \remarks This is a convenience function to allow functions to return
 * arbitrary method call objects. Boost::bind is not appropriate for this
 * task, since the type of a boost::bind object is lenghty and thus extremely
 * unhandy.
 *
 * $Date: 2007/10/09 00:22:26 $
 * $Revision: 1.3 $
 */

#ifndef RC_MEMBER_FUNC_H_
#define RC_MEMBER_FUNC_H_

namespace ReChannel {

template<typename METHOD_PTR> struct rc_member_func;

template<class R, class T>
struct rc_member_func<R (T::*)()>
{
    typedef R (T::*method_ptr_type)();
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()() const
        { return (p_obj.*p_mp)(); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T>
struct rc_member_func<R (T::*)() const>
{
    typedef R (T::*method_ptr_type)() const;
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()() const
        { return (p_obj.*p_mp)(); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1>
struct rc_member_func<R (T::*)(A1)>
{
    typedef R (T::*method_ptr_type)(A1);
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(A1 a1) const
        { return (p_obj.*p_mp)(a1); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1>
struct rc_member_func<R (T::*)(A1) const>
{
    typedef R (T::*method_ptr_type)(A1) const;
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(A1 a1) const
        { return (p_obj.*p_mp)(a1); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2>
struct rc_member_func<R (T::*)(A1, A2)>
{
    typedef R (T::*method_ptr_type)(A1, A2);
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(A1 a1, A2 a2) const
        { return (p_obj.*p_mp)(a1, a2); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2>
struct rc_member_func<R (T::*)(A1, A2) const>
{
    typedef R (T::*method_ptr_type)(A1, A2) const;
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(A1 a1, A2 a2) const
        { return (p_obj.*p_mp)(a1, a2); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3>
struct rc_member_func<R (T::*)(A1, A2, A3)>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3);
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(
        A1 a1, A2 a2, A3 a3) const
        { return (p_obj.*p_mp)(a1, a2, a3); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3>
struct rc_member_func<R (T::*)(A1, A2, A3) const>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3) const;
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(
        A1 a1, A2 a2, A3 a3) const
        { return (p_obj.*p_mp)(a1, a2, a3); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4>
struct rc_member_func<R (T::*)(A1, A2, A3, A4)>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3, A4);
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(A1 a1, A2 a2, A3 a3, A4 a4) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4>
struct rc_member_func<R (T::*)(A1, A2, A3, A4) const>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3, A4) const;
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(A1 a1, A2 a2, A3 a3, A4 a4) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4,
    class A5>
struct rc_member_func<R (T::*)(A1, A2, A3, A4, A5)>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3, A4, A5);
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4, a5); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4,
    class A5>
struct rc_member_func<R (T::*)(A1, A2, A3, A4, A5) const>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3, A4, A5) const;
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4, a5); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4,
    class A5, class A6>
struct rc_member_func<R (T::*)(A1, A2, A3, A4, A5, A6)>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3, A4, A5, A6);
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4, a5, a6); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4,
    class A5, class A6>
struct rc_member_func<R (T::*)(A1, A2, A3, A4, A5, A6) const>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3, A4, A5, A6) const;
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4, a5, a6); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4,
    class A5, class A6, class A7>
struct rc_member_func<R (T::*)(A1, A2, A3, A4, A5, A6, A7)>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3, A4, A5, A6, A7);
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4, a5, a6, a7); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4,
    class A5, class A6, class A7>
struct rc_member_func<R (T::*)(A1, A2, A3, A4, A5, A6, A7) const>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3, A4, A5, A6, A7) const;
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4, a5, a6, a7); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4,
    class A5, class A6, class A7, class A8>
struct rc_member_func<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8)>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3, A4, A5, A6, A7, A8);
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4, a5, a6, a7, a8); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4,
    class A5, class A6, class A7, class A8>
struct rc_member_func<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8) const>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3, A4, A5, A6, A7, A8) const;
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4, a5, a6, a7, a8); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4,
    class A5, class A6, class A7, class A8, class A9>
struct rc_member_func<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9)>
{
    typedef R (T::*method_ptr_type)(A1, A2, A3, A4, A5, A6, A7, A8, A9);
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8,
        A9 a9) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4, a5, a6, a7, a8, a9); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4,
    class A5, class A6, class A7, class A8, class A9>
struct rc_member_func<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9) const>
{
    typedef R (T::*method_ptr_type)(
        A1, A2, A3, A4, A5, A6, A7, A8, A9) const;
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8,
        A9 a9) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4, a5, a6, a7, a8, a9); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4,
    class A5, class A6, class A7, class A8, class A9, class A10>
struct
rc_member_func<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)>
{
    typedef R (T::*method_ptr_type)(
        A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8,
        A9 a9, A10 a10) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

template<class R, class T, class A1, class A2, class A3, class A4,
    class A5, class A6, class A7, class A8, class A9, class A10>
struct
rc_member_func<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) const>
{
    typedef R (T::*method_ptr_type)(
        A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) const;
    rc_member_func(T& obj, method_ptr_type mp)
        : p_obj(obj), p_mp(mp)
        { }
    inline R operator()(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8,
        A9 a9, A10 a10) const
        { return (p_obj.*p_mp)(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
private:
    T& p_obj;
    method_ptr_type p_mp;
};

} // namespace ReChannel

#endif // RC_MEMBER_FUNC_H_

//
// $Id: rc_member_func.h,v 1.3 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_member_func.h,v $
//

