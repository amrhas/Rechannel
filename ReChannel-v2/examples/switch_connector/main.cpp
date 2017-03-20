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
 * @file    main.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief   Main C++ file of example project 'switch_connector'.
 *
 * This example demonstrates the use of switch connectors and port maps.
 * A switch connector is used in file "Top.h" to ease the binding
 * of portals to the static and the dynamic side.
 *
 * \note The design is similar to that in example 'explicit_fifo_sync'.
 *
 * $Date: 2007/10/09 21:52:20 $
 * $Revision: 1.1 $
 */

#include <systemc.h>

#include "Top.h"

int main(int argc, char* argv[])
{
    Top* top = new Top("top");
    sc_start();
    delete top;
    return 0;
}

//
// $Id: main.cpp,v 1.1 2007/10/09 21:52:20 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/switch_connector/main.cpp,v $
//
