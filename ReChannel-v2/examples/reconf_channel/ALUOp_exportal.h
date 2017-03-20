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
 * @file    ALUOp_exportal.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:37:48 $
 * $Revision: 1.4 $
 */

#include "ALUOp_accessor.h"

#ifndef ALUOP_EXPORTAL_H_INCLUDED_
#define ALUOP_EXPORTAL_H_INCLUDED_

namespace ReChannel {

// (note: defining a default exportal without modifications is optional,
//        so it also may be omitted)
template<class T>
RC_EXPORTAL_TEMPLATE(ALUOp_if<T>)
{
    RC_EXPORTAL_TEMPLATE_CTOR(ALUOp_if<T>) { }
};

} // namespace ReChannel

#endif // ALUOP_EXPORTAL_H_INCLUDED_

//
// $Id: ALUOp_exportal.h,v 1.4 2007/11/23 13:37:48 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/reconf_channel/ALUOp_exportal.h,v $
//
