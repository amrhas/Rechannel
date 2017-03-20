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
 */
/**
 * @file    ALU.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief
 *
 *
 * $Date: 2007/10/14 12:44:02 $
 * $Revision: 1.4 $
 */

#include "ALU_if.h"
#include "ALUOp.h"
#include "ALUOp_exportal.h"

#ifndef ALU_H_INCLUDED_
#define ALU_H_INCLUDED_

template<class T>
class ALU
    : protected rc_control,
      virtual public ALU_if<T>
{
public:

    ALU(const sc_module_name& name_)
        : rc_control(name_), active_op(NULL)
    { }

    void add_op(ALUOp<T>& op_);

    virtual T calculate(
        ALUOp<T>& op, const T& operand1, const T& operand2);

protected:

    virtual sc_time takes_time(
        const rc_reconfigurable& reconf,
        rc_reconfigurable::action_type action) const;

protected:

    // exportal
    rc_exportal<ALUOp_if<T> > exportal;

    // mutex
    sc_mutex mutex;

private:

    // currently active operation
    ALUOp<T>* active_op;
};

/* template code */

template<class T>
void ALU<T>::add_op(ALUOp<T>& op_)
{
    this->add(op_);

    exportal.bind_dynamic(op_);
}

template<class T>
T ALU<T>::calculate(ALUOp<T>& op, const T& operand1, const T& operand2)
{
    mutex.lock();

    if (active_op != &op) {
        if (active_op != NULL) {
            RC_COUTL(this->name() << ": unloading module"
                " '" << active_op->name() << "'..."
                " (t=" << sc_time_stamp() << ")");

            this->unload(*active_op);

            RC_COUTL(this->name() << ": module '"
                << active_op->name() << "' unloaded."
                " (t=" << sc_time_stamp() << ")");
        }
        RC_COUTL(this->name() << ": activating module"
            " '" << op.name() << "'..."
            " (t=" << sc_time_stamp() << ")");

        this->activate(op);
        active_op = &op;

        RC_COUTL(this->name() << ": module '"
            << op.name() << "' activated."
            " (t=" << sc_time_stamp() << ")");
    }

    RC_COUTL(this->name() << ": calculate..."
            " (t=" << sc_time_stamp() << ")");
    T result = exportal->calculate(operand1, operand2);
    RC_COUTL(this->name() << ": calculation finished"
            " (t=" << sc_time_stamp() << ")");

    mutex.unlock();
    return result;
}

template<class T>
sc_time ALU<T>::takes_time(
    const rc_reconfigurable& reconf,
    rc_reconfigurable::action_type action) const
{
    if (action == RC_LOAD) {
        return sc_time(10, SC_NS);
    } else {
        return SC_ZERO_TIME;
    }
}

#endif // ALU_H_INCLUDED_

//
// $Id: ALU.h,v 1.4 2007/10/14 12:44:02 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/reconf_channel/ALU.h,v $
//
