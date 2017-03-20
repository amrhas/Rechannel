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
 * @file    ALU_if.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief
 *
 *
 * $Date: 2007/10/01 00:35:02 $
 * $Revision: 1.1 $
 */

#include "ALUOp.h"

#ifndef ALU_IF_H_INCLUDED_
#define ALU_IF_H_INCLUDED_

template<class T>
class ALU_if
    : virtual public sc_interface
{
public:
    // returns the result of operation "<operand1> op <operand2>"
    virtual T calculate(
        ALUOp<T>& op, const T& operand1, const T& operand2) = 0;
};

#endif // ALU_IF_H_INCLUDED_

//
// $Id: ALU_if.h,v 1.1 2007/10/01 00:35:02 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/reconf_channel/ALU_if.h,v $
//
