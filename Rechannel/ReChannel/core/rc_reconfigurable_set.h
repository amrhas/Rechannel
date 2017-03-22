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
 * @file    rc_reconfigurable_set.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class rc_reconfigurable_set.
 *
 *
 * $Date: 2007/12/20 20:39:40 $
 * $Revision: 1.7 $
 */

#ifndef RC_RECONFIGURABLE_SET_H_
#define RC_RECONFIGURABLE_SET_H_

#include "ReChannel/core/rc_common_header.h"

namespace ReChannel {

class rc_reconfigurable;

/**
 * @brief Implemenation of a set of rc_reconfigurable objects.
 *
 * This set class is used by several methods of rc_control to
 * allow passing of multiple rc_reconfigurable objects at once.
 *
 * \remark rc_reconfigurable_set is derived from a std::set container.
 *
 * \note For convenience, an rc_reconfigurable possesses an implicit casting
 * operator to a rc_reconfigurable_set which returns a set that only contains
 * this rc_reconfigurable instance.
 *
 * \see rc_control, rc_reconfigurable
 */
class rc_reconfigurable_set
    : private std::set<rc_reconfigurable*>
{
    /**
     * @brief Grant rc_control full access to the set.
     */
    friend class rc_control;

private:
    /**
     * @brief The base type is a std::set of rc_reconfigurable pointers.
     */
    typedef std::set<rc_reconfigurable*> base_type;

public:
    /**
     * @brief The iterator type of this set.
     */
    typedef base_type::iterator       iterator;
    /**
     * @brief The constant iterator type of this set.
     */
    typedef base_type::const_iterator const_iterator;

public:
    /**
     * @brief Default constructor.
     *
     * Creates an empty set.
     */
    rc_reconfigurable_set() { }

    /**
     * @brief Copy constructor.
     */
    rc_reconfigurable_set(const rc_reconfigurable_set& reconf_set)
        : base_type(reconf_set)
    { }

    /**
     * @brief Assigns the contents of another set.
     */
    inline rc_reconfigurable_set& operator=(
        const rc_reconfigurable_set& reconf_set)
        { base_type::operator=(reconf_set); return *this; }

    /**
     * @brief Returns the number of inserted set elements.
     */
    inline int size() const
        { return (int)base_type::size(); }

    /**
     * @brief Inserts an rc_reconfigurable.
     */
    inline void insert(rc_reconfigurable& reconf)
        { base_type::insert(&reconf); }

    /**
     * @brief Inserts the contents of another rc_reconfigurable_set.
     */
    inline void insert(const rc_reconfigurable_set& reconf_set)
        { base_type::insert(reconf_set.begin(), reconf_set.end()); }

    /**
     * @brief Removes an rc_reconfigurable from the set.
     */
    inline void erase(rc_reconfigurable& reconf)
        { base_type::erase(&reconf); }

    /**
     * @brief Removes multiple elements from the set.
     */
    void erase(const rc_reconfigurable_set& reconf_set);

    /**
     * @brief Checks if a rc_reconfigurable is contained.
     */
    inline bool contains(const rc_reconfigurable& reconf) const
    {
        return (base_type::find(const_cast<rc_reconfigurable*>(&reconf))
            != base_type::end());
    }

    /**
     * @brief Checks if the given set is contained.
     */
    bool contains(const rc_reconfigurable_set& reconf_set) const;

    /**
     * @brief Finds an rc_reconfigurable.
     */
    inline iterator find(rc_reconfigurable& reconf)
        { return base_type::find(&reconf); }

    /**
     * @brief Finds an rc_reconfigurable.
     */
    inline const_iterator find(const rc_reconfigurable& reconf) const
    {
        return base_type::find(
            const_cast<rc_reconfigurable*>(&reconf));
    }

    /**
     * @brief Returns an iterator beginning at the first element.
     */
    inline iterator begin()
        { return base_type::begin(); }

    /**
     * @brief Returns an constant iterator beginning at the first element.
     */
    inline const_iterator begin() const
        { return base_type::begin(); }

    /**
     * @brief Returns the end of the set.
     */
    inline iterator end()
        { return base_type::end(); }

    /**
     * @brief Returns the end of the set.
     */
    inline const_iterator end() const
        { return base_type::end(); }

    inline rc_reconfigurable_set& operator+=(
        rc_reconfigurable& reconf)
        { this->insert(reconf); return *this; }

    inline rc_reconfigurable_set& operator+=(
        const rc_reconfigurable_set& reconf_set)
        { this->insert(reconf_set); return *this; }

    inline rc_reconfigurable_set operator+(
        rc_reconfigurable& reconf) const
        { return (rc_reconfigurable_set(*this) += reconf); }

    inline rc_reconfigurable_set operator+(
        const rc_reconfigurable_set& reconf_set) const
        { return (rc_reconfigurable_set(*this) += reconf_set); }

    inline rc_reconfigurable_set& operator-=(
        rc_reconfigurable& reconf)
        { this->erase(reconf); return *this; }

    inline rc_reconfigurable_set& operator-=(
        const rc_reconfigurable_set& reconf_set)
        { this->erase(reconf_set); return *this; }

    inline rc_reconfigurable_set operator-(
        rc_reconfigurable& reconf) const
        { return (rc_reconfigurable_set(*this) -= reconf); }

    inline rc_reconfigurable_set operator-(
        const rc_reconfigurable_set& reconf_set) const
        { return (rc_reconfigurable_set(*this) -= reconf_set); }
};

/* global functions */

inline rc_reconfigurable_set operator+(
    rc_reconfigurable& dyn_object1, rc_reconfigurable& dyn_object2)
{
    return ((rc_reconfigurable_set() += dyn_object1) += dyn_object2);
}
inline rc_reconfigurable_set operator+(
    rc_reconfigurable& reconf,
    const rc_reconfigurable_set& reconf_set)
{
    return (rc_reconfigurable_set(reconf_set) += reconf);
}

} // namespace ReChannel

#endif // RC_RECONFIGURABLE_SET_H_

//
// $Id: rc_reconfigurable_set.h,v 1.7 2007/12/20 20:39:40 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_reconfigurable_set.h,v $
//

