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
 * @file    Top.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/09/28 13:23:13 $
 * $Revision: 1.1 $
 */

#include "Top.h"

void Top::activate(rc_reconfigurable& module)
{
    if (active_module != &module) {
        if (active_module != NULL) {
            std::cout << "Top: unloading module"
                " '" << active_module->rc_get_name() << "'..."
                " (t=" << sc_time_stamp() << ")\n";

            ctrl.unload(*active_module);

            std::cout << "Top: module '"
                << active_module->rc_get_name() << "' unloaded."
                " (t=" << sc_time_stamp() << ")\n\n";
        }

        std::cout << "Top: activating module"
            " '" << module.rc_get_name() << "'..."
            " (t=" << sc_time_stamp() << ")\n";

        ctrl.activate(module);
        active_module = &module;

        std::cout << "Top: module '"
            << module.rc_get_name() << "' activated."
            " (t=" << sc_time_stamp() << ")\n";
    }
}

void Top::producer_proc()
{
    for (int i=0; i < 50; ++i) {
        int x = rnd100();
        int y = rnd100();
        in1_fifo.write(x);
        in2_fifo.write(y);
        cout << "Top: input written (" << x << ", " << y << ")"
            " (t=" << sc_time_stamp() << ")\n";
        wait(10, SC_NS);
    }
}

void Top::control_proc()
{
    wait(32, SC_NS);

    activate(m2);
    wait(34, SC_NS);

    activate(m4);
    wait(24, SC_NS);

    activate(m3);
    wait(21, SC_NS);

    activate(m5);
    wait(38, SC_NS);

    activate(m2);
    wait(19, SC_NS);

    activate(m3);
    wait(23, SC_NS);

    activate(m1);
    wait(23, SC_NS);

    activate(m4);
    wait(38, SC_NS);

    activate(m3);
    wait(140, SC_NS);

    activate(m1);
}

void Top::monitor_proc()
{
    int i=0;
    while(true) {
        ++i;
        int x = out1_fifo.read();
        int y = out2_fifo.read();
        cout << "Top: " << i << ". output read (" << x << ", " << y << ")"
            " (t=" << sc_time_stamp() << ")\n";
    }
}

//
// $Id: Top.cpp,v 1.1 2007/09/28 13:23:13 raabe Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/test/fifo_filter_datapackage/Top.cpp,v $
//
