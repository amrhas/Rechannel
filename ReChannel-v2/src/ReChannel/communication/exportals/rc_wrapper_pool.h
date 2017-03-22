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
 * @file    rc_wrapper_pool.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Definition of internal class wrapper_pool.
 *
 *
 * $Date: 2008/01/01 13:46:06 $
 * $Revision: 1.2 $
 */

#ifndef RC_WRAPPER_POOL_H_
#define RC_WRAPPER_POOL_H_

#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/communication/rc_interface_wrapper.h"

namespace ReChannel {
namespace internals {
namespace exportal {

/**
 * @brief ABI representing an interface wrapper factory.
 */
class wrapper_factory
{
public:
    virtual rc_interface_wrapper_base* create_interface_wrapper(
        sc_interface& wrapped_if) = 0;

protected:
    virtual ~wrapper_factory() {}
};

class wrapper_handle;

/**
 * @brief This class represents an interface wrapper handle pool.
 */
class wrapper_pool
{
    friend class wrapper_handle;

private:
    typedef std::set<wrapper_handle*> wrapper_handle_set; 
    typedef std::multimap<
                sc_interface*,
                rc_interface_wrapper_base*
            > if_wrapper_multimap;
    typedef std::pair<
                if_wrapper_multimap::iterator,
                if_wrapper_multimap::iterator
            > multimap_range;
public:

    wrapper_handle get(
        sc_interface& wrapped_if, wrapper_factory* factory=NULL);

    void add(rc_interface_wrapper_base& wrapper);

    unsigned int size() const
    { return p_if_wrapper_multimap.size() + p_num_taken; }

    unsigned int available() const
        { return p_if_wrapper_multimap.size(); }

    unsigned int available(sc_interface& wrapped_if) const
        { return p_if_wrapper_multimap.count(&wrapped_if); }

    ~wrapper_pool();

private:
    void release(wrapper_handle& wrapper_handle_);

private:

    /** @brief multimap: interface -> wrappper */
    if_wrapper_multimap p_if_wrapper_multimap;

    /** @brief number of wrappers taken from the pool */
    unsigned int p_num_taken;
};

/**
 * @brief This class represents an interface wrapper handle
 *        (with auto_ptr behavior).
 */
class wrapper_handle
{
    friend class wrapper_pool;

public:
    wrapper_handle(const wrapper_handle& orig)
        : p_wrapper_pool(orig.p_wrapper_pool), p_wrapper(orig.p_wrapper)
        { orig.p_wrapper = NULL; }

    wrapper_handle(rc_interface_wrapper_base* wrapper=NULL)
        : p_wrapper_pool(NULL), p_wrapper(wrapper)
        { }

    inline bool valid() const
        { return (p_wrapper != NULL); }

    inline rc_interface_wrapper_base* get_object() const
        { return p_wrapper; }

    inline rc_interface_wrapper_base& operator*()
        { return *p_wrapper; }

    inline rc_interface_wrapper_base* operator->()
        { return p_wrapper; }

    inline rc_interface_wrapper_base* operator->() const
        { return p_wrapper; }

    inline void release();

    wrapper_handle& operator=(wrapper_handle& orig);

    wrapper_handle& operator=(rc_interface_wrapper_base* wrapper);

    inline ~wrapper_handle()
        { this->release(); }

private:
    wrapper_handle(
        wrapper_pool& wrapper_pool_, rc_interface_wrapper_base* wrapper_)
        : p_wrapper_pool(&wrapper_pool_), p_wrapper(wrapper_)
        { }

private:
    mutable wrapper_pool*              p_wrapper_pool;
    mutable rc_interface_wrapper_base* p_wrapper;
};

inline void wrapper_handle::release()
{
    if (p_wrapper_pool != NULL) {
        p_wrapper_pool->release(*this);
    }
    p_wrapper = NULL;
}

} // namespace exportal
} // namespace internals
} // namespace ReChannel

#endif // RC_WRAPPER_POOL_H_
//
// $Id: rc_wrapper_pool.h,v 1.2 2008/01/01 13:46:06 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/exportals/rc_wrapper_pool.h,v $
//
