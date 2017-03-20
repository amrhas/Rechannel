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
 * @brief   Main C++ file of example project 'reconf_channel'.
 *
 * This example demonstrates how to reconfigure channels by the use of
 * ReChannel's exportal switches.
 *
 * $Date: 2007/09/30 18:37:58 $
 * $Revision: 1.1 $
 */

#include <cstdlib>
#include <ctime>

#include <systemc.h>

#include "Top.h"

int main(int argc, char* argv[])
{
    // initialize random seed
    srand((unsigned int)time(NULL));

    // create top level module
    Top* top = new Top("top");

    // start simulation
    sc_start();

    // clean up and exit
    delete top;
    return 0;
}

//
// $Id: main.cpp,v 1.1 2007/09/30 18:37:58 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/reconf_channel/main.cpp,v $
//
