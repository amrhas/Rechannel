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
 * @file    Top.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/10/14 12:44:02 $
 * $Revision: 1.3 $
 */

#include "Top.h"

data_type Top::calculate(
        ALUOp<data_type>& op,
        const data_type& operand1, const data_type& operand2)
{
    RC_COUTL(this->name() << ": calculating "
        << op.basename() << "("
        << operand1 << ", " << operand2 << ")..."
        " (t=" << sc_time_stamp() << ")");

    data_type result = alu.calculate(op, operand1, operand2);

    RC_COUTL(this->name() << ": result=" << result
        << " (t=" << sc_time_stamp() << ")\n");

    return result;
}

void Top::calc_proc()
{
    // test calculation series:

    // 23 + 12 = 35
    calculate(add, 23, 12);

    // 99 + 2 = 101
    calculate(add, 99, 2);

    // 7 * 3 = 21
    calculate(multiply, 7, 3);

    // compare((3 + 9), (4 * 3)) = 0 [equal]
    data_type result =
        calculate(compare,
            calculate(add, 3, 9),
            calculate(multiply, 4, 3));

    // 100 * 293 = 29300
    calculate(multiply, 100, 293);

    // 2 + -2 = 0
    calculate(add, 2, -2);

    // ((5 + 3) + 16) * 2 = 48
    calculate(multiply,
        calculate(add, calculate(add, 5, 3), 16), 2);
}

//
// $Id: Top.cpp,v 1.3 2007/10/14 12:44:02 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/reconf_channel/Top.cpp,v $
//
