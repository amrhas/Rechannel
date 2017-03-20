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
 * @brief   Main C++ file of example project 'explicit_fifo_sync'.
 *
 * In constrast to example 'fifo_filter_dataflow', which demonstrates the use
 * of filters in pure data flow models, this example solves the same task by
 * the explict description of reconfigurable behaviour.
 *
 * ReChannel's resettable processes and macro RC_TRANSACTION are used to
 * ensure that no data gets lost by reconfiguration and that all involved
 * processes are suspended in a well-defined state.
 * The application of filters is not required if reconfigurable modules are
 * derived from resettable modules with internal transaction control.
 *
 * $Date: 2007/10/09 21:51:26 $
 * $Revision: 1.2 $
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
// $Id: main.cpp,v 1.2 2007/10/09 21:51:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/explicit_fifo_sync/main.cpp,v $
//
