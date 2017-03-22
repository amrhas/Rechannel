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
 * @file    rc_reconfigurable_module.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class rc_reconfigurable_module.
 *
 *
 * $Date: 2007/12/06 11:51:20 $
 * $Revision: 1.16 $
 */

#ifndef RC_RECONFIGURABLE_MODULE_H_
#define RC_RECONFIGURABLE_MODULE_H_

#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/core/rc_process_support.h"

#include <boost/type_traits/is_base_of.hpp>

namespace ReChannel {

#define RC_STATIC_ASSERT_VALID_MODULE(module_type) \
    RC_STATIC_ASSERT_T( \
        module_type_validity_check, \
        (boost::is_base_of<sc_module, module_type >::value \
         || boost::is_same<sc_module, module_type>::value) \
    )

template<
    class Module=sc_module,
    bool _rc_resettable=boost::is_base_of<rc_resettable, Module>::value
>
class rc_reconfigurable_module;

/**
 * @brief Class of a reconfigurable module.
 * 
 * rc_reconfigurable_module represents a reconfigurable module,
 * as it encapsulates an SC_MODULE and an rc_reconfigurable.
 *
 * Using rc_reconfigurable_module for the creation of reconfigurable modules is
 * more convenient and, furthermore, also is a requirement for some more
 * advanced ReChannel features to work.
 *
 * The first template parameter specified the module that has to be derived
 * from, as described in \ref sec_derive_reconf_module and
 *      \ref sec_reconf_module.
 *
 * The second boolean template paramter is internally used to detect whether
 * the inherited module already implements rc_resettable, or not.
 * The rc_reconfigurable_module is guaranteed to implement rc_resettable and
 * therefore always is "resettable".
 *
 * rc_reconfigurable_module automatically inherits all constructors of M with
 * up to ten arguments. This is made possible by ten templated constructor
 * variants with different parameter count.
 *
 * \remark The class is additionally derived from
 * internals::reconfigurable::begin_contruction to allow the identification of
 * the current reconfigurable context during a subsequent base class and member
 * construction.
 *
 * \note Resettable processes may be created within a module of this type.
 *
 * \see rc_reconfigurable, RC_RECONFIGURABLE_MODULE(),
 *      RC_RECONFIGURABLE_CTOR(),
 *      RC_RECONFIGURABLE_MODULE_DERIVED(),
 *      RC_RECONFIGURABLE_CTOR_DERIVED(), RC_HAS_PROCESS()
 */
template<class Module>
class rc_reconfigurable_module<Module, true>
    : public rc_reconfigurable,
      private internals::reconfigurable::begin_construction,
      public Module
{
    /**
     * @brief Static assertion to check if Module is a valid module.
     *
     * A valid module has to be derived from sc_core::sc_module.
     */
    RC_STATIC_ASSERT_VALID_MODULE(Module);

private:
    typedef typename internals::reconfigurable::begin_construction
        begin_construction;
public:

    /**
	 * @brief The type of the inherited module.
	 */
	typedef Module module_type;

public:
    /**
	 * @brief Default constructor.
	 */
    rc_reconfigurable_module();

    /**
	 * @brief Default module constructor.
	 */
    rc_reconfigurable_module(sc_module_name module_name);

    template<class A1>
    rc_reconfigurable_module(A1 a1);

    template<class A1, class A2>
    rc_reconfigurable_module(A1 a1, A2 a2);

    template<class A1, class A2, class A3>
    rc_reconfigurable_module(A1 a1, A2 a2, A3 a3);

    template<class A1, class A2, class A3, class A4>
    rc_reconfigurable_module(A1 a1, A2 a2, A3 a3, A4 a4);

    template<class A1, class A2, class A3, class A4, class A5>
    rc_reconfigurable_module(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

    template<class A1, class A2, class A3, class A4, class A5, class A6>
    rc_reconfigurable_module(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);

    template<class A1, class A2, class A3, class A4,
            class A5, class A6, class A7>
    rc_reconfigurable_module(
        A1 a1, A2 a2, A3 a3, A4 a4,
        A5 a5, A6 a6, A7 a7);

    template<class A1, class A2, class A3, class A4,
            class A5, class A6, class A7, class A8>
    rc_reconfigurable_module(
        A1 a1, A2 a2, A3 a3, A4 a4,
        A5 a5, A6 a6, A7 a7, A8 a8);

    template<class A1, class A2, class A3, class A4, class A5,
            class A6, class A7, class A8, class A9>
    rc_reconfigurable_module(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5,
        A6 a6, A7 a7, A8 a8, A9 a9);

    template<class A1, class A2, class A3, class A4, class A5,
            class A6, class A7, class A8, class A9, class A10>
    rc_reconfigurable_module(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5,
        A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);

protected:

    /**
	 * @brief Finds SystemC objects by their name.
     *
     * This method redirects to the global ::rc_find_object() function.
     * Relative name resolution is supported. If rel_obj is not provided
     * or equals NULL, the rc_reconfigurable_module instance, this method is
     * called for, is specified as the name resolution base.
     *
     * \see ::rc_find_object()
	 */
    inline sc_object* rc_find_object(
        const char* obj_name, const sc_object* rel_obj=NULL);

/* next_trigger(..) redirects */

    /**
	 * @brief Calls ReChannel::rc_next_trigger().
	 */
    inline void next_trigger()
        { rc_next_trigger(); }

    inline void next_trigger(const sc_event& e)
        { rc_next_trigger(e); }

    inline void next_trigger(sc_event_or_list& el)
        { rc_next_trigger(el); }

    inline void next_trigger(sc_event_and_list& el)
        { rc_next_trigger(el); }

    inline void next_trigger(const sc_time& t)
        { rc_next_trigger(t); }

    inline void next_trigger(double v, sc_time_unit tu)
        { rc_next_trigger(v, tu); }

    inline void next_trigger(const sc_time& t, const sc_event& e)
        { rc_next_trigger(t, e); }

    inline void next_trigger(double v, sc_time_unit tu, const sc_event& e)
        { rc_next_trigger(v, tu, e); }

    inline void next_trigger(const sc_time& t, sc_event_or_list& el)
        { rc_next_trigger(t, el); }

    inline void next_trigger(
        double v, sc_time_unit t, sc_event_or_list& el)
        { rc_next_trigger(v, t, el); }

    inline void next_trigger(const sc_time& t, sc_event_and_list& el)
        { rc_next_trigger(t, el); }

    inline void next_trigger(
        double v, sc_time_unit tu, sc_event_and_list& el)
        { rc_next_trigger(v, tu, el); }

/* wait(..) redirects */

    /**
	 * @brief Calls ReChannel::rc_wait().
	 */
    inline void wait()
        { rc_wait(); }

    inline void wait(int n)
        { rc_wait(n); }

    inline void wait(const sc_event& e)
        { rc_wait(e); }

    inline void wait(sc_event_or_list& el)
        { rc_wait(el); }

    inline void wait(sc_event_and_list& el)
        { rc_wait(el); }

    inline void wait(const sc_time& t)
        { rc_wait(t); }

    inline void wait(double v, sc_time_unit tu)
        { rc_wait(v, tu); }

    inline void wait(const sc_time& t, const sc_event& e)
        { rc_wait(t, e); }

    inline void wait(double v, sc_time_unit tu, const sc_event& e)
        { rc_wait(v, tu, e); }

    inline void wait(const sc_time& t, sc_event_or_list& el)
        { rc_wait(t, el); }

    inline void wait(double v, sc_time_unit t, sc_event_or_list& el)
        { rc_wait(v, t, el); }

    inline void wait(const sc_time& t, sc_event_and_list& el)
        { rc_wait(t, el); }

    inline void wait(double v, sc_time_unit tu, sc_event_and_list& el)
        { rc_wait(v, tu, el); }
};

/**
 * @brief Class of a reconfigurable module.
 * 
 * rc_reconfigurable_module represents a reconfigurable module,
 * as it encapsulates an SC_MODULE and an rc_reconfigurable.
 *
 * Using rc_reconfigurable_module for the creation of reconfigurable modules is
 * more convenient and, furthermore, also is a requirement for some more
 * advanced ReChannel features to work.
 *
 * The first template parameter specified the module that has to be derived
 * from, as described in \ref sec_derive_reconf_module and
 *      \ref sec_reconf_module.
 *
 * The second boolean template paramter is internally used to detect whether
 * the inherited module already implements rc_resettable, or not.
 * The rc_reconfigurable_module is guaranteed to implement rc_resettable and
 * therefore always is "resettable".
 *
 * rc_reconfigurable_module automatically inherits all constructors of M with
 * up to ten arguments. This is made possible by ten templated constructor
 * variants with different parameter count.
 *
 * \remark The class is additionally derived from
 * internals::reconfigurable::begin_contruction to allow the identification of
 * the current reconfigurable context during a subsequent base class and member
 * construction.
 *
 * \note Resettable processes may be created within a module of this type.
 *
 * \see rc_reconfigurable, RC_RECONFIGURABLE_MODULE(),
 *      RC_RECONFIGURABLE_CTOR(),
 *      RC_RECONFIGURABLE_MODULE_DERIVED(),
 *      RC_RECONFIGURABLE_CTOR_DERIVED(), RC_HAS_PROCESS()
 */
template<class Module>
class rc_reconfigurable_module<Module, false>
    : public rc_reconfigurable,
      private internals::reconfigurable::begin_construction,
      public Module,
      virtual public rc_resettable
{
    /**
     * @brief Static assertion to check if Module is a valid module.
     *
     * A valid module has to be derived from sc_core::sc_module.
     */
    RC_STATIC_ASSERT_VALID_MODULE(Module);

private:
    typedef typename internals::reconfigurable::begin_construction
        begin_construction;

public:
    /**
	 * @brief The type of the inherited module.
	 */
    typedef Module module_type;

public:
    /**
	 * @brief Default constructor.
	 */
    rc_reconfigurable_module();

    /**
	 * @brief Default module constructor.
	 */
    rc_reconfigurable_module(sc_module_name module_name);

    template<class A1>
    rc_reconfigurable_module(A1 a1);

    template<class A1, class A2>
    rc_reconfigurable_module(A1 a1, A2 a2);

    template<class A1, class A2, class A3>
    rc_reconfigurable_module(A1 a1, A2 a2, A3 a3);

    template<class A1, class A2, class A3, class A4>
    rc_reconfigurable_module(A1 a1, A2 a2, A3 a3, A4 a4);

    template<class A1, class A2, class A3, class A4, class A5>
    rc_reconfigurable_module(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

    template<class A1, class A2, class A3, class A4, class A5, class A6>
    rc_reconfigurable_module(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);

    template<class A1, class A2, class A3, class A4,
            class A5, class A6, class A7>
    rc_reconfigurable_module(
        A1 a1, A2 a2, A3 a3, A4 a4,
        A5 a5, A6 a6, A7 a7);

    template<class A1, class A2, class A3, class A4,
            class A5, class A6, class A7, class A8>
    rc_reconfigurable_module(
        A1 a1, A2 a2, A3 a3, A4 a4,
        A5 a5, A6 a6, A7 a7, A8 a8);

    template<class A1, class A2, class A3, class A4, class A5,
            class A6, class A7, class A8, class A9>
    rc_reconfigurable_module(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5,
        A6 a6, A7 a7, A8 a8, A9 a9);

    template<class A1, class A2, class A3, class A4, class A5,
            class A6, class A7, class A8, class A9, class A10>
    rc_reconfigurable_module(
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5,
        A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);

protected:
    inline virtual void rc_on_reset() {}

    inline virtual void rc_on_init_resettable() {}

    /**
	 * @brief Finds SystemC objects by their name.
     *
     * This method redirects to the global ::rc_find_object() function.
     * Relative name resolution is supported. If rel_obj is not provided
     * or equals NULL, the rc_reconfigurable_module instance, this method is
     * called for, is specified as the name resolution base.
     *
     * \see ::rc_find_object()
	 */
    inline sc_object* rc_find_object(
        const char* obj_name, const sc_object* rel_obj=NULL);

/* next_trigger(..) redirects */

    /**
	 * @brief Calls ReChannel::rc_next_trigger().
	 */
    inline void next_trigger()
        { rc_next_trigger(); }

    inline void next_trigger(const sc_event& e)
        { rc_next_trigger(e); }

    inline void next_trigger(sc_event_or_list& el)
        { rc_next_trigger(el); }

    inline void next_trigger(sc_event_and_list& el)
        { rc_next_trigger(el); }

    inline void next_trigger(const sc_time& t)
        { rc_next_trigger(t); }

    inline void next_trigger(double v, sc_time_unit tu)
        { rc_next_trigger(v, tu); }

    inline void next_trigger(const sc_time& t, const sc_event& e)
        { rc_next_trigger(t, e); }

    inline void next_trigger(double v, sc_time_unit tu, const sc_event& e)
        { rc_next_trigger(v, tu, e); }

    inline void next_trigger(const sc_time& t, sc_event_or_list& el)
        { rc_next_trigger(t, el); }

    inline void next_trigger(
        double v, sc_time_unit t, sc_event_or_list& el)
        { rc_next_trigger(v, t, el); }

    inline void next_trigger(const sc_time& t, sc_event_and_list& el)
        { rc_next_trigger(t, el); }

    inline void next_trigger(
        double v, sc_time_unit tu, sc_event_and_list& el)
        { rc_next_trigger(v, tu, el); }

/* wait(..) redirects */

    /**
	 * @brief Calls ReChannel::rc_wait().
	 */
    inline void wait()
        { rc_wait(); }

    inline void wait(int n)
        { rc_wait(n); }

    inline void wait(const sc_event& e)
        { rc_wait(e); }

    inline void wait(sc_event_or_list& el)
        { rc_wait(el); }

    inline void wait(sc_event_and_list& el)
        { rc_wait(el); }

    inline void wait(const sc_time& t)
        { rc_wait(t); }

    inline void wait(double v, sc_time_unit tu)
        { rc_wait(v, tu); }

    inline void wait(const sc_time& t, const sc_event& e)
        { rc_wait(t, e); }

    inline void wait(double v, sc_time_unit tu, const sc_event& e)
        { rc_wait(v, tu, e); }

    inline void wait(const sc_time& t, sc_event_or_list& el)
        { rc_wait(t, el); }

    inline void wait(double v, sc_time_unit t, sc_event_or_list& el)
        { rc_wait(v, t, el); }

    inline void wait(const sc_time& t, sc_event_and_list& el)
        { rc_wait(t, el); }

    inline void wait(double v, sc_time_unit tu, sc_event_and_list& el)
        { rc_wait(v, tu, el); }
};

/* template code */

// Module implements rc_resettable:

template<class Module>
rc_reconfigurable_module<Module, true>::rc_reconfigurable_module()
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type()
    { begin_construction::finish(static_cast<sc_module&>(*this)); }

template<class Module>
rc_reconfigurable_module<Module, true>::rc_reconfigurable_module(
    sc_module_name module_name)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(module_name)
    { begin_construction::finish(static_cast<sc_module&>(*this)); }

template<class Module>
template<class A1>
rc_reconfigurable_module<Module, true>::rc_reconfigurable_module(A1 a1)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1)
    { begin_construction::finish(static_cast<sc_module&>(*this)); }

template<class Module>
template<class A1, class A2>
rc_reconfigurable_module<Module, true>::rc_reconfigurable_module(
    A1 a1, A2 a2)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2)
    { begin_construction::finish(static_cast<sc_module&>(*this)); }

template<class Module>
template<class A1, class A2, class A3>
rc_reconfigurable_module<Module, true>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3)
    { begin_construction::finish(static_cast<sc_module&>(*this)); }

template<class Module>
template<class A1, class A2, class A3, class A4>
rc_reconfigurable_module<Module, true>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4)
    { begin_construction::finish(static_cast<sc_module&>(*this)); }

template<class Module>
template<class A1, class A2, class A3, class A4, class A5>
rc_reconfigurable_module<Module, true>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4, a5)
    { begin_construction::finish(static_cast<sc_module&>(*this)); }

template<class Module>
template<class A1, class A2, class A3, class A4, class A5, class A6>
rc_reconfigurable_module<Module, true>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4, a5, a6)
    { begin_construction::finish(static_cast<sc_module&>(*this)); }

template<class Module>
template<class A1, class A2, class A3, class A4,
        class A5, class A6, class A7>
rc_reconfigurable_module<Module, true>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4, a5, a6, a7)
    { begin_construction::finish(static_cast<sc_module&>(*this)); }

template<class Module>
template<class A1, class A2, class A3, class A4,
        class A5, class A6, class A7, class A8>
rc_reconfigurable_module<Module, true>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4, a5, a6, a7, a8)
    { begin_construction::finish(static_cast<sc_module&>(*this)); }

template<class Module>
template<class A1, class A2, class A3, class A4, class A5,
        class A6, class A7, class A8, class A9>
rc_reconfigurable_module<Module, true>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4, a5, a6, a7, a8, a9)
    { begin_construction::finish(static_cast<sc_module&>(*this)); }

template<class Module>
template<class A1, class A2, class A3, class A4, class A5,
        class A6, class A7, class A8, class A9, class A10>
rc_reconfigurable_module<Module, true>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)
    { begin_construction::finish(static_cast<sc_module&>(*this)); }

/* inline code */

template<class Module>
inline
sc_object* rc_reconfigurable_module<Module, true>::rc_find_object(
    const char* obj_name, const sc_object* rel_obj)
{
    if (rel_obj == NULL) { rel_obj = this; }
    return ::ReChannel::rc_find_object(obj_name, rel_obj);
}

// Module does not implement rc_resettable:

template<class Module>
rc_reconfigurable_module<Module, false>::rc_reconfigurable_module()
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(sc_module_name())
{
    rc_reconfigurable::rc_register_resettable(*this);
    begin_construction::finish(static_cast<sc_module&>(*this));
}

template<class Module>
rc_reconfigurable_module<Module, false>::rc_reconfigurable_module(
    sc_module_name module_name)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(module_name)
{
    rc_reconfigurable::rc_register_resettable(*this);
    begin_construction::finish(static_cast<sc_module&>(*this));
}

template<class Module>
template<class A1>
rc_reconfigurable_module<Module, false>::rc_reconfigurable_module(A1 a1)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1)
{
    rc_reconfigurable::rc_register_resettable(*this);
    begin_construction::finish(static_cast<sc_module&>(*this));
}

template<class Module>
template<class A1, class A2>
rc_reconfigurable_module<Module, false>::rc_reconfigurable_module(
    A1 a1, A2 a2)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2)
{
    rc_reconfigurable::rc_register_resettable(*this);
    begin_construction::finish(static_cast<sc_module&>(*this));
}

template<class Module>
template<class A1, class A2, class A3>
rc_reconfigurable_module<Module, false>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3)
{
    rc_reconfigurable::rc_register_resettable(*this);
    begin_construction::finish(static_cast<sc_module&>(*this));
}

template<class Module>
template<class A1, class A2, class A3, class A4>
rc_reconfigurable_module<Module, false>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4)
{
    rc_reconfigurable::rc_register_resettable(*this);
    begin_construction::finish(static_cast<sc_module&>(*this));
}

template<class Module>
template<class A1, class A2, class A3, class A4, class A5>
rc_reconfigurable_module<Module, false>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4, a5)
{
    rc_reconfigurable::rc_register_resettable(*this);
    begin_construction::finish(static_cast<sc_module&>(*this));
}

template<class Module>
template<class A1, class A2, class A3, class A4, class A5, class A6>
rc_reconfigurable_module<Module, false>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4, a5, a6)
{
    rc_reconfigurable::rc_register_resettable(*this);
    begin_construction::finish(static_cast<sc_module&>(*this));
}

template<class Module>
template<class A1, class A2, class A3, class A4,
        class A5, class A6, class A7>
rc_reconfigurable_module<Module, false>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4, a5, a6, a7)
{
    rc_reconfigurable::rc_register_resettable(*this);
    begin_construction::finish(static_cast<sc_module&>(*this));
}

template<class Module>
template<class A1, class A2, class A3, class A4,
        class A5, class A6, class A7, class A8>
rc_reconfigurable_module<Module, false>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4, a5, a6, a7, a8)
{
    rc_reconfigurable::rc_register_resettable(*this);
    begin_construction::finish(static_cast<sc_module&>(*this));
}

template<class Module>
template<class A1, class A2, class A3, class A4, class A5,
        class A6, class A7, class A8, class A9>
rc_reconfigurable_module<Module, false>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4, a5, a6, a7, a8, a9)
{
    rc_reconfigurable::rc_register_resettable(*this);
    begin_construction::finish(static_cast<sc_module&>(*this));
}

template<class Module>
template<class A1, class A2, class A3, class A4, class A5,
        class A6, class A7, class A8, class A9, class A10>
rc_reconfigurable_module<Module, false>::rc_reconfigurable_module(
    A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
    : begin_construction(static_cast<rc_reconfigurable&>(*this)),
      module_type(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)
{
    rc_reconfigurable::rc_register_resettable(*this);
    begin_construction::finish(static_cast<sc_module&>(*this));
}

/* inline code */

template<class Module>
inline
sc_object* rc_reconfigurable_module<Module, false>::rc_find_object(
    const char* obj_name, const sc_object* rel_obj)
{
    if (rel_obj == NULL) {
        rel_obj = this;
    }
    return ::ReChannel::rc_find_object(obj_name, rel_obj);
}

} // namespace ReChannel

/* construction and helper macros */

/**
 * @brief Declaration macro for a reconfigurable module.
 * 
 * This macro is provided for convenience to
 * simplify the declaration of reconfigurable modules for the designer.
 * 
 * \see rc_reconfigurable_module, RC_RECONFIGURABLE_CTOR(),
 *      \ref sec_reconf_module
 */
#define RC_RECONFIGURABLE_MODULE(reconf_module_name) \
    struct reconf_module_name \
        : public ::ReChannel::rc_reconfigurable_module<>

/**
 * @brief Declaration macro for a reconfigurable module's constructor.
 * 
 * This macro is provided for convenience to
 * simplify the declaration of reconfigurable modules for the designer.
 * 
 * \see rc_reconfigurable_module, RC_RECONFIGURABLE(),
 *      \ref sec_reconf_module
 */
#define RC_RECONFIGURABLE_CTOR(reconf_module_name) \
    RC_HAS_PROCESS(reconf_module_name); \
    reconf_module_name(const sc_module_name& name_) \
        : rc_reconfigurable_module<>(name_)

/**
 * @brief Declaration macro for a derived reconfigurable module.
 * 
 * This macro is provided for convenience to
 * simplify the declaration of reconfigurable modules for the designer.
 * 
 * \see rc_reconfigurable_module, RC_RECONFIGURABLE_CTOR_DERIVED(),
 *      \ref sec_derive_reconf_module
 */
#define RC_RECONFIGURABLE_MODULE_DERIVED(reconf_module_name, user_module_name) \
    struct reconf_module_name \
        : public ::ReChannel::rc_reconfigurable_module<user_module_name >

/**
 * @brief Declaration macro for a derived reconfigurable module's constructor.
 * 
 * This macro is provided for convenience to
 * simplify the declaration of reconfigurable modules for the designer.
 * 
 * \see rc_reconfigurable_module, RC_RECONFIGURABLE_MODULE_DERIVED(),
 *      \ref sec_derive_reconf_module
 */
#define RC_RECONFIGURABLE_CTOR_DERIVED(reconf_module_name, user_module_name) \
    RC_HAS_PROCESS(reconf_module_name); \
    reconf_module_name(const sc_module_name& name_) \
        : rc_reconfigurable_module<user_module_name >(name_)

#endif // RC_RECONFIGURABLE_MODULE_H_

//
// $Id: rc_reconfigurable_module.h,v 1.16 2007/12/06 11:51:20 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_reconfigurable_module.h,v $
//
