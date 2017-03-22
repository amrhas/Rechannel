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
 * @file    rc_transaction_counter.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Class rc_transaction_counter
 *
 *
 * $Date: 2007/12/20 20:42:13 $
 * $Revision: 1.11 $
 */

#ifndef RC_TRANSACTION_COUNTER_H_
#define RC_TRANSACTION_COUNTER_H_

#include "ReChannel/core/rc_reconfigurable.h"

namespace ReChannel {

/**
 * @brief Class rc_transaction_counter
 *
 * rc_transaction_counter is used to modify the transaction count of
 * the reconfigurable context. If the transaction count of a rc_reconfigurable
 * instance is greater than zero, it cannot be deactivated due to the pending
 * transactions that have to be finished first.
 *
 * Transaciont counter can be declared as a member of a reconfigurable module
 * to obtain control over the transaction count. This is often used in
 * combination with interface filters to synchronise reconfiguration.
 *
 * In constrast to rc_reconfigurable's transaction count, a transaction counter
 * may also count into the negativity. The effective transaction count will be
 * the absolute value of this counter. Only if a transaction counter equals
 * zero the reconfigurable context may be deactivated.
 *
 * At its instantiation a transaction counter can be declared to be a relay
 * counter. Therefore a list of children counters is specified. If the
 * relay counter is modified it actually only all of its children are modified.
 * For each child a integer valued relay factor can be set, that differs from 1
 * even may be negative, to weigh the children's modification amount.
 *
 * \remark If rc_transaction_counter is created at simulation time, it is linked
 * to the reconfigurable context of the executing process.
 *
 * \note If rc_transaction_counter is created in a non-reconfigurable context,
 * it can be used as usual, but all modifications to it will not have any
 * effect.
 */
class rc_transaction_counter
    : public sc_object
{
private:
    typedef rc_transaction_counter this_type;

public:

    /**
     * @brief Default constructor.
     */
    rc_transaction_counter();

    /**
     * @brief Constructor (constructs a relay counter).
     *
     * @param children a vector of child transaction counters
     */
    explicit rc_transaction_counter(std::vector<this_type*>& children);

    /**
     * @brief Returns the current count.
     */
    inline int count() const
        { return p_count; }

    /**
     * @brief Modifies the counter by the given amount.
     * @param amount of the modification
     */
    void modify(int amount);

    /**
     * @brief Reset the counter.
     *
     * The counter is reset to zero. All hold transactions are released.
     * \note Will have no effect in case of a relay counter.
     */
    void reset();

    /**
     * @brief Destructor.
     *
     * Calls reset() before destruction.
     */
    ~rc_transaction_counter()
        { this->reset(); }

    /**
     * @brief Returns the associated reconfigurable context (if exists)
     */
    inline rc_reconfigurable* get_reconfigurable() const
        { return p_reconf; }

    /**
     * @brief Returns the number of children
     *
     * \note Will return 0 if it is not a relay counter.
     */
    inline int get_children_count() const
        { return (int)p_children.size(); }

    /**
     * @brief Returns the relay factor of child of the given index.
     */
    int get_relay_factor(int index) const;

    /**
     * @brief Sets the relay factor for the child of the given index.
     */
    void set_relay_factor(int index, int factor);

    /**
     * @brief Sets the relay factor for the given child.
     */
    void set_relay_factor(rc_transaction_counter& child, int factor);

/* convenience operators */

    /**
     * @brief Increase the counter by one.
     */
    inline const this_type& operator++()
        { this->modify(+1); return *this; }

    /**
     * @brief Decrease the counter by one.
     */
    inline const this_type& operator--()
        { this->modify(-1); return *this; }

    /**
     * @brief Increase the counter by a particular amount.
     */
    inline const this_type& operator+=(int amount)
        { this->modify(amount); return *this; }

    /**
     * @brief Decrease the counter by a particular amount.
     */
    inline const this_type& operator-=(int amount)
        { this->modify(-amount); return *this; }

/* convenience constructors */

    /**
     * @brief Convenience constructor (constructs a relay counter)
     * @param tc1 a child counter
     * @param factor1 the relay factor of the given child counter
     */
    explicit rc_transaction_counter(this_type& tc1, int factor1);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4,
        this_type& tc5, int factor5);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4,
        this_type& tc5, int factor5, this_type& tc6, int factor6);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4,
        this_type& tc5, int factor5, this_type& tc6, int factor6,
        this_type& tc7, int factor7);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4,
        this_type& tc5, int factor5, this_type& tc6, int factor6,
        this_type& tc7, int factor7, this_type& tc8, int factor8);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4,
        this_type& tc5, int factor5, this_type& tc6, int factor6,
        this_type& tc7, int factor7, this_type& tc8, int factor8,
        this_type& tc9, int factor9);

    rc_transaction_counter(
        this_type& tc1, int factor1, this_type& tc2, int factor2,
        this_type& tc3, int factor3, this_type& tc4, int factor4,
        this_type& tc5, int factor5, this_type& tc6, int factor6,
        this_type& tc7, int factor7, this_type& tc8, int factor8,
        this_type& tc9, int factor9, this_type& tc10, int factor10);

private:

    /**
     * @brief relays a modification to the childs
     */
    void _rc_relay(int amount);

    /**
     * @brief adds a child
     * @param child the child to be added
     * @param factor the relay factor of the child
     *
     * \note A transaction counter with children is regarded as a relay counter
     * and will only count at the child objects.
     */
    void _rc_add_child(rc_transaction_counter& child, int factor=1);

private:

    /**
     * @brief The associated reconfigurable context.
     */
    rc_reconfigurable*                       p_reconf;
    /**
     * @brief The current transaction number.
     */
    int                                      p_count;
    /**
     * @brief The children vector
     *
     * \note (relay counters only)
     */
    std::vector<std::pair<this_type*, int> > p_children;

private:
    // disabled
    /** @brief (disabled) */
    rc_transaction_counter(const this_type& orig);
    /** @brief (disabled) */
    this_type& operator=(const this_type& orig);
};

} // namespace ReChannel

#endif //RC_TRANSACTION_COUNTER_H_

//
// $Id: rc_transaction_counter.h,v 1.11 2007/12/20 20:42:13 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_transaction_counter.h,v $
//
