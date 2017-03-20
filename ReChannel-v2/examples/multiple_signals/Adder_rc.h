// vim:set et sts=4 ts=4 tw=75 sw=4 ai ci cin:
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
 * This ReChannel library is distributed in the hope that it will be
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
 * @file    Adder_rc.h
 * @author  Andreas H. Niers          <niers@cs.uni-bonn.de>
 * @brief   A simple adder for reconfiguration in UTF 
 *
 * \verbatim
 *  
 *      +---#---------#-----+     
 *      |summand1  summand2 |     
 *      |                   |     
 *      |     Adder_rc      |     
 *      |                   |     
 *      |      summe        |     
 *      +--------#----------+     
 *      
 * \endverbatim
 *
 */

#ifndef __ADDER_RC_H
#define __ADDER_RC_H

#include <systemc.h>
#include <ReChannel.h>

#include "Adder.h"

typedef rc_reconfigurable_module<Adder> Adder_rc;

#endif
