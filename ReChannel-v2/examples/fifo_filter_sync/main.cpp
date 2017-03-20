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
 * @brief   Main C++ file of example project 'fifo_filter_sync'.
 *
 * This example demonstrates the use of ReChannel's FIFO filters for the
 * purpose of synchronizing the reconfiguration of system level modules. In
 * data flow models, filters allow starting transactions when new input
 * data is read and, respectively, end them when data has been processed
 * and written (see also example 'fifo_filter_dataflow'). The challenge is
 * to determine the right moment for reconfiguration. Which is important in
 * cases where it is required that a data packet is processed by a specific
 * module.
 *
 * FIFOs are continuously filled with test series adressed to a specific
 * module. This imposes the existence of a synchronization mechanism which
 * ensures that all data is processed correctly by the respective module
 * and that reconfiguration does not take place while a test series has not
 * yet finished.
 *
 * Main subject of this example is the class template 'CryptoModule' which
 * can be parameterized with different implementations of encryption
 * modules. 'CryptoModule' augments an existing implementation with the
 * property "reconfigurable" as well as a handshake mechanism for
 * synchronization with the outside world's reconfiguration control.
 *
 * The dummy modules used in this example, 'AES', 'Blowfish' and
 * 'TripleDES', are implemented differently in respect of
 * blocking/non-blocking FIFO accesses and the choice of process type.
 * CryptoModule's synchronization mechanism is designed to handle all these
 * cases.
 *
 * $Date: 2007/08/21 13:59:08 $
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
// $Id: main.cpp,v 1.2 2007/08/21 13:59:08 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/fifo_filter_sync/main.cpp,v $
//
