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
 * @file    ALUOp.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief
 *
 *
 * $Date: 2007/11/23 13:37:48 $
 * $Revision: 1.2 $
 */

#include <ReChannel.h>
#include "ALUOp_if.h"

#ifndef ALUOP_H_INCLUDED_
#define ALUOP_H_INCLUDED_

template<class T>
class ALUOp
    : public sc_object,
      public rc_reconfigurable,
      virtual public ALUOp_if<T>
{
protected:
    ALUOp(const char* name_)
        : sc_object(name_),
          rc_reconfigurable(static_cast<sc_object*>(this))
    { }
};

#endif // ALUOP_H_INCLUDED_

//
// $Id: ALUOp.h,v 1.2 2007/11/23 13:37:48 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/reconf_channel/ALUOp.h,v $
//
