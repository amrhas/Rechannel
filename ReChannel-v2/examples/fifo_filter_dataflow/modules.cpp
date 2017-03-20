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
 * @file    modules.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/10/14 12:44:02 $
 * $Revision: 1.2 $
 */

#include "modules.h"

// include ReChannel to allow use of RC_COUTL
#include <ReChannel.h>

void Maximum::proc()
{
    int i = 0;
    while(true) {
        ++i;
        int data1 = in1.read();
        RC_COUTL("Maximum: 1. input read: " << data1
            << " (t=" << sc_time_stamp() << ")");
        RC_COUTL("Maximum: START WORKING #" << i
            << " (t=" << sc_time_stamp() << ")");
        int data2 = in2.read();
        RC_COUTL("Maximum: 2. input read: " << data2
            << " (t=" << sc_time_stamp() << ")");
        int maximum = (data1 >= data2 ? data1 : data2);
        wait(10, SC_NS);
        out1.write(maximum);
        out2.write(maximum);
        RC_COUTL("Maximum: FINISHED #" << i
            << " (t=" << sc_time_stamp() << ")");
    }
}

void Serialize::proc()
{
    int i = 0;
    while(true) {
        ++i;
        int data1 = in1.read();
        RC_COUTL("Serialize: 1. input read: " << data1
            << " (t=" << sc_time_stamp() << ")");
        RC_COUTL("Serialize: START WORKING #" << i
            << " (t=" << sc_time_stamp() << ")");
        wait(10, SC_NS);
        out1.write(data1);
        out2.write(data1);
        int data2 = in2.read();
        RC_COUTL("Serialize: 2. input read: " << data2
            << " (t=" << sc_time_stamp() << ")");
        wait(10, SC_NS);
        out1.write(data2);
        out2.write(data2);
        RC_COUTL("Serialize: FINISHED #" << i
            << " (t=" << sc_time_stamp() << ")");
    }
}

void Swap::proc()
{
    int i = 0;
    while(true) {
        ++i;
        int data = in1.read();
        RC_COUTL("Swap: 1. input read: " << data
            << " (t=" << sc_time_stamp() << ")");
        RC_COUTL("Swap: START WORKING #" << i
            << " (t=" << sc_time_stamp() << ")");
        wait(5, SC_NS);
        out2.write(data);

        data = in2.read();
        RC_COUTL("Swap: 2. input read: " << data
            << " (t=" << sc_time_stamp() << ")");
        wait(5, SC_NS);
        out1.write(data);
        RC_COUTL("Swap: FINISHED #" << i
            << " (t=" << sc_time_stamp() << ")");
    }
}

void Repeat::proc()
{
    int i = 0;
    while(true) {
        ++i;
        int data1 = in1.read();
        RC_COUTL("Repeat: 1. input read: " << data1
            << " (t=" << sc_time_stamp() << ")");
        RC_COUTL("Repeat: START WORKING #" << i
            << " (t=" << sc_time_stamp() << ")");
        int data2 = in2.read();
        RC_COUTL("Repeat: 2. input read: " << data2
            << " (t=" << sc_time_stamp() << ")");
        wait(5, SC_NS);
        out1.write(data1);
        out2.write(data2);
        wait(5, SC_NS);
        out1.write(data1);
        out2.write(data2);
        RC_COUTL("Repeat: FINISHED #" << i
            << " (t=" << sc_time_stamp() << ")");
    }
}

void Consume::proc()
{
    int i = 0;
    while(true) {
        ++i;
        int data1 = in1.read();
        RC_COUTL("Consume: 1. input read: " << data1
            << " (t=" << sc_time_stamp() << ")");
        RC_COUTL("Consume: START WORKING #" << i
            << " (t=" << sc_time_stamp() << ")");
        int data2 = in2.read();
        RC_COUTL("Consume: 2. input read: " << data2
            << " (t=" << sc_time_stamp() << ")");
        RC_COUTL("Consume: FINISHED #" << i
            << " (t=" << sc_time_stamp() << ")");
    }
}

//
// $Id: modules.cpp,v 1.2 2007/10/14 12:44:02 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/fifo_filter_dataflow/modules.cpp,v $
//
