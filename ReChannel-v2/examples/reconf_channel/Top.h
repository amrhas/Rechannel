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
 * @file    Top.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief
 *
 *
 * $Date: 2007/10/01 00:35:02 $
 * $Revision: 1.2 $
 */

#include <cstdlib>

#include <ReChannel.h>

#include "ALU.h"
#include "ALUOps.h"

#define WORD_BITSIZE 16
typedef sc_int<WORD_BITSIZE> data_type;

SC_MODULE(Top)
{
    // the ALU module
    ALU<data_type> alu;

    // the reconfigurable ALUOps
    AddOp<data_type> add;
    MulOp<data_type> multiply;
    CmpOp<data_type> compare;

    SC_CTOR(Top)
        : alu("alu"),
          add("add"), multiply("multiply"), compare("compare")
    {
        // add the operations
        alu.add_op(add);
        alu.add_op(multiply);
        alu.add_op(compare);

        // thread process performing the test calculations
        SC_THREAD(calc_proc);
    }

private:

    // calculate
    data_type calculate(
        ALUOp<data_type>& op,
        const data_type& operand1, const data_type& operand2);

    void calc_proc();
};

//
// $Id: Top.h,v 1.2 2007/10/01 00:35:02 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/reconf_channel/Top.h,v $
//
