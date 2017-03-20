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
 * @file    ALUOp_accessor.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/10/01 00:35:02 $
 * $Revision: 1.3 $
 */

#include <ReChannel.h>
#include "ALUOp_if.h"

#ifndef ALUOP_ACCESSOR_H_INCLUDED_
#define ALUOP_ACCESSOR_H_INCLUDED_

namespace ReChannel {

template<class T>
RC_ACCESSOR_TEMPLATE(ALUOp_if<T>)
{
    RC_ACCESSOR_TEMPLATE_CTOR(ALUOp_if<T>) { }

    virtual T calculate(const T& operand1, const T& operand2)
    {
        return this->rc_forward(&if_type::calculate, operand1, operand2);
    }
};

} // namespace ReChannel

#endif // ALUOP_ACCESSOR_H_INCLUDED_

//
// $Id: ALUOp_accessor.h,v 1.3 2007/10/01 00:35:02 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/reconf_channel/ALUOp_accessor.h,v $
//
