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
 * @file    rc_hash_map.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class rc_hash_map and class rc_hash_multimap.
 *
 * Some ReChannel classes require the availability of efficient unordered maps
 * (i.e. hash maps).
 * Since the hash maps currently are not officially part of the C++ standard
 * template library (STL) and existing implementations strongly differ in the
 * support for hash maps, a custom hash map class is simply composed from the
 * boost's multi_index library.
 *
 * $Date: 2007/10/09 00:32:22 $
 * $Revision: 1.7 $
 */

#ifndef RC_HASH_MAP_H_
#define RC_HASH_MAP_H_

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

namespace ReChannel {

namespace internals {
namespace hash_map {

/** @brief the value type used by hash (multi)maps.
 *
 * pair is a reimplementation of class std::pair.
 * A custom pair is needed, because boost::multi_index only possesses constant
 * iterators. To enable non-constant accesses to the second data element
 * it has to be declared "mutable".
 */
template<class T1, class T2>
struct pair
{
    typedef T1 first_type;
    typedef T2 second_type;

    pair()
        : first(T1()), second(T2())
        { }
    pair(const T1& f, const T2& s)
        : first(f), second(s)
        { }
    pair(const std::pair<T1, T2>& p)
        : first(p.first), second(p.second)
        { }

    operator std::pair<T1, T2>() const
        { return std::pair<T1, T2>(first, second); }

    T1         first;
    mutable T2 second;
};

} // namespace hash_map
} // namespace internals

/** @brief Hash map implementation.
 *
 * rc_hash_map is internally composed of a Boost multi_index container with
 * one hashed index. The class's template paramters, members and typedefs are
 * orientated on those of the standard map's implementation.
 * (see also SGI, http://www.sgi.com/).
 */
template<
    class key_, class data_,
    class hash_=boost::hash<key_>,
    class pred_=std::equal_to<key_>,
    class alloc_=std::allocator<internals::hash_map::pair<key_, data_> >
>
class rc_hash_map
{
private:
    typedef rc_hash_map<key_, data_, hash_, pred_, alloc_> this_type;

public:
    /** \brief The key type. */
    typedef key_  key_type;
    /** \brief The data type. */
    typedef data_ data_type;
    /** \brief The value type. */
    typedef typename internals::hash_map::pair<key_, data_> value_type;

private:
    /** \brief The tag of the first (and only) index.
     */
    struct index_1 {};
    /** \brief The type of the boost::multi_index container.
     */
    typedef typename boost::template multi_index_container<
              value_type,
              typename boost::multi_index::template indexed_by<
                typename boost::multi_index::template hashed_unique<
                  typename boost::multi_index::template tag<index_1>,
                  BOOST_MULTI_INDEX_MEMBER(value_type, key_type, first),
                  hash_, pred_
                >
              >,
              alloc_
            >
        hash_map_type;
    /** @brief the index type
     *
     * \note Only one index is used in the multi_index container.
     */
    typedef typename hash_map_type::template index<index_1>::type
        index_type;

public:
    typedef typename index_type::pointer         pointer;
    typedef typename index_type::const_pointer   const_pointer;
    typedef typename index_type::reference       reference;
    typedef typename index_type::const_reference const_reference;
    typedef typename index_type::size_type       size_type;
    typedef typename index_type::difference_type difference_type;
    typedef typename index_type::iterator        iterator;
    typedef typename index_type::const_iterator  const_iterator;
    typedef typename index_type::hasher          hasher;
    typedef typename index_type::key_equal       key_equal;
    typedef typename index_type::allocator_type  allocator_type;

public:

    rc_hash_map() { }
    rc_hash_map(const this_type& other)
        : p_hash_map(other)
        { }
    inline this_type& operator=(const this_type& other)
        { p_hash_map = other; }

    inline bool empty() const
        { return p_hash_map.template get<index_1>().empty(); }
    inline size_type size() const
        { return p_hash_map.template get<index_1>().size(); }
    inline size_type max_size() const
        { return p_hash_map.template get<index_1>().max_size(); }

    inline iterator begin()
        { return p_hash_map.template get<index_1>().begin(); }
    inline iterator end()
        { return p_hash_map.template get<index_1>().end(); }
    inline const_iterator begin() const
        { return p_hash_map.template get<index_1>().begin(); }
    inline const_iterator end() const
        { return p_hash_map.template get<index_1>().end(); }

    inline std::pair<iterator, bool> insert(const value_type& x)
        { return p_hash_map.template get<index_1>().insert(x); }
    inline iterator insert(iterator position, const value_type& x)
        { return p_hash_map.template get<index_1>().insert(position, x); }
    template<typename InputIterator>
    inline void insert(InputIterator first, InputIterator last)
        { p_hash_map.template get<index_1>().insert(first, last); }

    inline iterator erase(iterator position)
        { return p_hash_map.template get<index_1>().erase(position); }
    inline size_type erase(const key_type& x)
        { return p_hash_map.template get<index_1>().erase(x); }
    inline iterator erase(iterator first, iterator last)
        { return p_hash_map.template get<index_1>().erase(first, last); }

    inline void clear()
        { p_hash_map.template get<index_1>().clear(); }
    inline void swap(this_type& x)
    {
        p_hash_map.template get<index_1>().swap(
            x.p_hash_map.template get<index_1>());
    }

    inline hasher hash_funct() const
        { return p_hash_map.template get<index_1>().hash_function(); }
    inline key_equal key_eq() const
        { return p_hash_map.template get<index_1>().key_eq(); }

    inline iterator find(const key_type& k) const
        { return p_hash_map.template get<index_1>().find(k); }

    inline size_type count(const key_type& k) const
        { return p_hash_map.template get<index_1>().count(k); }
    inline std::pair<iterator, iterator> equal_range(
        const key_type& k) const
        { return p_hash_map.template get<index_1>().equal_range(k); }

    inline size_type bucket_count() const
        { return p_hash_map.template get<index_1>().bucket_count(); }
    inline void resize(size_type n)
        { p_hash_map.template get<index_1>().rehash(n); }

    data_type& operator[](const key_type& k);

private:
    hash_map_type p_hash_map;
};

/** @brief Hash multimap implementation.
 *
 * rc_hash_multimap is internally composed of a Boost multi_index container with
 * one hashed index. The class's template paramters, members and typedefs are
 * orientated on those of the standard multimap's implementation.
 * (see also SGI, http://www.sgi.com/).
 */
template<
    class key_,
    class data_,
    class hash_=boost::hash<key_>,
    class pred_=std::equal_to<key_>,
    class alloc_=std::allocator<internals::hash_map::pair<key_, data_> >
>
class rc_hash_multimap
{
private:
    typedef rc_hash_map<key_, data_, hash_, pred_, alloc_> this_type;

public:
    /** \brief The key type. */
    typedef key_  key_type;
    /** \brief The data type. */
    typedef data_ data_type;
    /** \brief The value type. */
    typedef typename internals::hash_map::pair<key_, data_> value_type;

private:
    /** \brief The tag of the first (and only) index.
     */
    struct index_1 {};
    /** \brief The type of the boost::multi_index container.
     */
    typedef typename boost::template multi_index_container<
              value_type,
              typename boost::multi_index::template indexed_by<
                typename boost::multi_index::template hashed_non_unique<
                  typename boost::multi_index::template tag<index_1>,
                  BOOST_MULTI_INDEX_MEMBER(value_type, key_type, first),
                  hash_, pred_
                >
              >,
              alloc_
            >
        hash_multimap_type;
    /** @brief the index type
     *
     * \note Only one index is used in the multi_index container.
     */
    typedef typename hash_multimap_type::template index<index_1>::type
        index_type;

public:
    typedef typename index_type::pointer         pointer;
    typedef typename index_type::const_pointer   const_pointer;
    typedef typename index_type::reference       reference;
    typedef typename index_type::const_reference const_reference;
    typedef typename index_type::size_type       size_type;      
    typedef typename index_type::difference_type difference_type;
    typedef typename index_type::iterator        iterator;
    typedef typename index_type::const_iterator  const_iterator;
    typedef typename index_type::hasher          hasher;
    typedef typename index_type::key_equal       key_equal;
    typedef typename index_type::allocator_type  allocator_type;

public:

    rc_hash_multimap() { }
    rc_hash_multimap(const this_type& other)
        : p_hash_multimap(other)
        { }
    inline this_type& operator=(const this_type& other)
        { p_hash_multimap = other; }

    inline bool empty() const
        { return p_hash_multimap.template get<index_1>().empty(); }
    inline size_type size() const
        { return p_hash_multimap.template get<index_1>().size(); }
    inline size_type max_size() const
        { return p_hash_multimap.template get<index_1>().max_size(); }

    inline iterator begin()
        { return p_hash_multimap.template get<index_1>().begin(); }
    inline iterator end()
        { return p_hash_multimap.template get<index_1>().end(); }
    inline const_iterator begin() const
        { return p_hash_multimap.template get<index_1>().begin(); }
    inline const_iterator end() const
        { return p_hash_multimap.template get<index_1>().end(); }

    inline std::pair<iterator, bool> insert(const value_type& x)
        { return p_hash_multimap.template get<index_1>().insert(x); }
    inline iterator insert(iterator position, const value_type& x)
    { return p_hash_multimap.template get<index_1>().insert(position, x); }
    template<typename InputIterator>
    inline void insert(InputIterator first, InputIterator last)
        { p_hash_multimap.template get<index_1>().insert(first, last); }

    inline iterator erase(iterator position)
        { return p_hash_multimap.template get<index_1>().erase(position); }
    inline size_type erase(const key_type& x)
        { return p_hash_multimap.template get<index_1>().erase(x); }
    inline iterator erase(iterator first, iterator last)
    { return p_hash_multimap.template get<index_1>().erase(first, last); }

    inline void clear()
        { p_hash_multimap.template get<index_1>().clear(); }
    inline void swap(this_type& x)
    {
        p_hash_multimap.template get<index_1>().swap(
            x.p_hash_multimap.template get<index_1>());
    }

    inline hasher hash_funct() const
        { return p_hash_multimap.template get<index_1>().hash_function(); }
    inline key_equal key_eq() const
        { return p_hash_multimap.template get<index_1>().key_eq(); }

    inline iterator find(const key_type& k) const
        { return p_hash_multimap.template get<index_1>().find(k); }

    inline size_type count(const key_type& k) const
        { return p_hash_multimap.template get<index_1>().count(k); }
    inline std::pair<iterator, iterator> equal_range(
        const key_type& k) const
        { return p_hash_multimap.template get<index_1>().equal_range(k); }

    inline size_type bucket_count() const
        { return p_hash_multimap.template get<index_1>().bucket_count(); }
    inline void resize(size_type n)
        { p_hash_multimap.template get<index_1>().rehash(n); }

private:
    hash_multimap_type p_hash_multimap;
};

/* template code */

template<class key_, class data_, class hash_, class pred_, class alloc_>
typename rc_hash_map<key_, data_, hash_, pred_, alloc_>::data_type&
rc_hash_map<key_, data_, hash_, pred_, alloc_>::operator[](
    const key_type& k)
{
    iterator it = this->find(k);
    if (it != this->end()) {
        return it->second;
    } else {
        typename std::template pair<iterator,bool> ret =
            this->insert(value_type(k, data_type()));
        return ret.first->second;
    }
}

} // namespace ReChannel

#endif // RC_HASH_MAP_H_

//
// $Id: rc_hash_map.h,v 1.7 2007/10/09 00:32:22 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_hash_map.h,v $
//
