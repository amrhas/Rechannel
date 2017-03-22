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
 * @file    rc_var.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.14 $
 */

#ifndef RC_VAR_H_
#define RC_VAR_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_resettable.h"
#include "ReChannel/core/rc_reconfigurable.h"

namespace ReChannel {
namespace internals {
namespace resettable_var {

/**
 * @brief Base class of a resettable variable.
 */
class resettable_var_base
    : virtual public rc_resettable
{
protected:
    resettable_var_base();

    template<class P> void init();

    void report_invalid_use_error();

protected:
    // one void field for the parent to save memory
    // may only be changed by constructors!
    // caution: must be interpreted as P* after call of init<P>()!!!
    void* m_parent;

private:
    class tmp_object : public sc_object {};

private:
    // disabled
    resettable_var_base(const resettable_var_base& orig);
    resettable_var_base& operator=(const resettable_var_base& orig);
};

/**
 * @brief Base class of a resettable variable.
 */
template<class T>
class resettable_var_b
    : public resettable_var_base
{
protected:
    resettable_var_b();

    template<class P> void reset(T& (P::*get_ref)(void));

    template<class P> void init_reset(T& (P::*get_ref)(void));

private:
    T m_reset_value;
};

/**
 * @brief This class represents a resettable variable.
 */
template<class P, class T, T& (P::*get_ref)(void)>
class resettable_var
    : public resettable_var_b<T>
{
public:
    resettable_var()
        : resettable_var_b<T>()
    {
        this->template init<P>();
    }

protected:

// MSVC .NET 2003 compiler bug workaround
#if (defined(_MSC_VER) && _MSC_VER < 1400) /* Visual Studio .NET 2003 */

    virtual void rc_on_reset()
	    { this->template reset<P>(get_ref); }

    virtual void rc_on_init_resettable()
	    { this->template init_reset<P>(get_ref); }

#else // all other compilers

	virtual void rc_on_reset();

    virtual void rc_on_init_resettable();

#endif // (defined(_MSC_VER) && _MSC_VER < 1400)
};

/* template code */

template<class P>
void resettable_var_base::init()
{
    sc_object* parent_obj = reinterpret_cast<sc_object*>(m_parent);
    m_parent = dynamic_cast<P*>(parent_obj);
    if (m_parent == NULL) {
        report_invalid_use_error();
    }
}

template<class T>
resettable_var_b<T>::resettable_var_b()
    : resettable_var_base()
{ }

template<class T>
template<class P>
void resettable_var_b<T>::reset(T& (P::*get_ref)(void))
{
    (reinterpret_cast<P*>(m_parent)->*get_ref)() = m_reset_value;
}

template<class T>
template<class P>
void resettable_var_b<T>::init_reset(T& (P::*get_ref)(void))
{
    m_reset_value = (reinterpret_cast<P*>(m_parent)->*get_ref)();
}

// MSVC .NET 2003 compiler bug workaround
#if (defined(_MSC_VER) && _MSC_VER < 1400) /* Visual Studio .NET 2003 */

/* definition of rc_on_reset() and rc_on_init_resettable moved
   to class declaration */

#else // all other compilers

template<class P, class T, T& (P::*get_ref)(void)>
void resettable_var<P, T, get_ref>::rc_on_reset()
{
    this->template reset<P>(get_ref);
}

template<class P, class T, T& (P::*get_ref)(void)>
void resettable_var<P, T, get_ref>::rc_on_init_resettable()
{
    this->template init_reset<P>(get_ref);
}

#endif // (defined(_MSC_VER) && _MSC_VER < 1400)

} // namespace resettable_var
} // namespace internals
} // namespace ReChannel

// MSVC .NET 2003 compiler bug workaround
#if (defined(_MSC_VER) && _MSC_VER < 1400) /* Visual Studio .NET 2003 */

/* MSVC .NET 2003 seems to ignore the member access restrictions anyway */
#define RC_HAS_RECONFIGURABLE_VAR(user_module_name) \
    typedef user_module_name RC_CURRENT_VAR_CONTAINER

#else // all other compilers

#define RC_HAS_RECONFIGURABLE_VAR(user_module_name) \
    template< \
        class _rc_var_P, \
        class _rc_var_T, \
        _rc_var_T& (_rc_var_P::*_rc_var_get_ref)(void) \
    > \
    friend class ::ReChannel::internals::resettable_var::resettable_var; \
    typedef user_module_name RC_CURRENT_VAR_CONTAINER

#endif // (defined(_MSC_VER) && _MSC_VER < 1400)

#define RC_DECLARE_RECONFIGURABLE_VAR(var_type, var_name) \
    var_type& _rc_var_get_##var_name() { return this->var_name; } \
    ::ReChannel::internals::resettable_var::resettable_var< \
        RC_CURRENT_VAR_CONTAINER, var_type, \
        &RC_CURRENT_VAR_CONTAINER::_rc_var_get_##var_name> \
            _rc_var_##var_name

#define RC_RECONFIGURABLE_VAR(var_type, var_name) \
    var_type var_name; \
    RC_DECLARE_RECONFIGURABLE_VAR(var_type, var_name)

#define RC_HAS_VAR(user_module_name) \
    RC_HAS_RECONFIGURABLE_VAR(user_module_name)

#define RC_DECLARE_VAR(var_type, var_name) \
    RC_DECLARE_RECONFIGURABLE_VAR(var_type, var_name)
#define rc_declare_var(var_type, var_name) \
    RC_DECLARE_RECONFIGURABLE_VAR(var_type, var_name)

#define RC_VAR(var_type, var_name) \
    RC_RECONFIGURABLE_VAR(var_type, var_name)
#define rc_var(var_type, var_name) \
    RC_RECONFIGURABLE_VAR(var_type, var_name)

#define RC_RESETTABLE_VAR(var_type, var_name) \
    RC_RECONFIGURABLE_VAR(var_type, var_name)
#define rc_resettable_var(var_type, var_name) \
    RC_RECONFIGURABLE_VAR(var_type, var_name)

#define RC_PRESERVABLE_VAR(var_type, var_name) \
    var_type var_name;
#define rc_preservable_var(var_type, var_name) \
    var_type var_name;

#define RC_DECLARE_RESETTABLE_VAR(var_type, var_name) \
    RC_DECLARE_RECONFIGURABLE_VAR(var_type, var_name)
#define rc_declare_resettable_var(var_type, var_name) \
    RC_DECLARE_RECONFIGURABLE_VAR(var_type, var_name)

#define RC_DECLARE_PRESERVABLE_VAR(var_type, var_name) \
    /* nothing to do */
#define rc_declare_preservable_var(var_type, var_name) \
    /* nothing to do */

#endif // RC_VAR_H_

//
// $Id: rc_var.h,v 1.14 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_var.h,v $
//
