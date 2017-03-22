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
 * $Date: 2007/09/28 13:23:13 $
 * $Revision: 1.1 $
 */

#include "modules.h"

void Maximum::proc()
{
    int i = 0;
    while(true) {
        ++i;
        int data1 = in1.read();
        cout << "Maximum: 1. input read: " << data1
            << " (t=" << sc_time_stamp() << ")\n";
        cout << "Maximum: START WORKING #" << i
            << " (t=" << sc_time_stamp() << ")\n";
        int data2 = in2.read();
        cout << "Maximum: 2. input read: " << data2
            << " (t=" << sc_time_stamp() << ")\n";
        int maximum = (data1 >= data2 ? data1 : data2);
        wait(10, SC_NS);
        out1.write(maximum);
        out2.write(maximum);
        cout << "Maximum: FINISHED #" << i
            << " (t=" << sc_time_stamp() << ")\n";
    }
}

void Serialize::proc()
{
    int i = 0;
    while(true) {
        ++i;
        int data1 = in1.read();
        cout << "Serialize: 1. input read: " << data1
            << " (t=" << sc_time_stamp() << ")\n";
        cout << "Serialize: START WORKING #" << i
            << " (t=" << sc_time_stamp() << ")\n";
        wait(10, SC_NS);
        out1.write(data1);
        out2.write(data1);
        int data2 = in2.read();
        cout << "Serialize: 2. input read: " << data2
            << " (t=" << sc_time_stamp() << ")\n";
        wait(10, SC_NS);
        out1.write(data2);
        out2.write(data2);
        cout << "Serialize: FINISHED #" << i
            << " (t=" << sc_time_stamp() << ")\n";
    }
}

void Swap::proc()
{
    int i = 0;
    while(true) {
        ++i;
        int data = in1.read();
        cout << "Swap: 1. input read: " << data
            << " (t=" << sc_time_stamp() << ")\n";
        cout << "Swap: START WORKING #" << i
            << " (t=" << sc_time_stamp() << ")\n";
        wait(5, SC_NS);
        out2.write(data);

        data = in2.read();
        cout << "Swap: 2. input read: " << data
            << " (t=" << sc_time_stamp() << ")\n";
        wait(5, SC_NS);
        out1.write(data);
        cout << "Swap: FINISHED #" << i
            << " (t=" << sc_time_stamp() << ")\n";
    }
}

void Repeat::proc()
{
    int i = 0;
    while(true) {
        ++i;
        int data1 = in1.read();
        cout << "Repeat: 1. input read: " << data1
            << " (t=" << sc_time_stamp() << ")\n";
        cout << "Repeat: START WORKING #" << i
            << " (t=" << sc_time_stamp() << ")\n";
        int data2 = in2.read();
        cout << "Repeat: 2. input read: " << data2
            << " (t=" << sc_time_stamp() << ")\n";
        wait(5, SC_NS);
        out1.write(data1);
        out2.write(data2);
        wait(5, SC_NS);
        out1.write(data1);
        out2.write(data2);
        cout << "Repeat: FINISHED #" << i
            << " (t=" << sc_time_stamp() << ")\n";
    }
}

void Consume::proc()
{
    int i = 0;
    while(true) {
        ++i;
        int data1 = in1.read();
        cout << "Consume: 1. input read: " << data1
            << " (t=" << sc_time_stamp() << ")\n";
        cout << "Consume: START WORKING #" << i
            << " (t=" << sc_time_stamp() << ")\n";
        int data2 = in2.read();
        cout << "Consume: 2. input read: " << data2
            << " (t=" << sc_time_stamp() << ")\n";
        cout << "Consume: FINISHED #" << i
            << " (t=" << sc_time_stamp() << ")\n";
    }
}

//
// $Id: modules.cpp,v 1.1 2007/09/28 13:23:13 raabe Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/test/fifo_filter_datapackage/modules.cpp,v $
//
