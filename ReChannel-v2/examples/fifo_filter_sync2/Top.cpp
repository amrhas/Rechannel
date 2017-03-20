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
 * $Date: 2007/10/14 12:44:02 $
 * $Revision: 1.2 $
 */

#include "Top.h"

void Top::activate(rc_reconfigurable& module)
{
    if (active_module != &module) {
        if (active_module != NULL) {
            // sync handshake
            // send deactivation command
            sync_in.write(false);
            // wait ack
            sync_out.read();

            RC_COUTL("Top: unloading module"
                " '" << active_module->rc_get_name() << "'..."
                " (t=" << sc_time_stamp() << ")");

            ctrl.unload(*active_module);

            RC_COUTL("Top: module '"
                << active_module->rc_get_name() << "' unloaded."
                " (t=" << sc_time_stamp() << ")");
        }

        RC_COUTL("Top: activating module"
            " '" << module.rc_get_name() << "'..."
            " (t=" << sc_time_stamp() << ")");

        ctrl.activate(module);
        active_module = &module;

        RC_COUTL("Top: module '"
            << module.rc_get_name() << "' activated."
            " (t=" << sc_time_stamp() << ")");
    }
}

void Top::test(const sc_bv<192>& key, int count)
{
    RC_COUTL("\nTestbench: start testing '"
        << active_module->rc_get_name() << "'"
        " (" << count << " key/data pairs)..."
        " (t=" << sc_time_stamp() << ")");

    // send #count random words together with the key
    sc_bv<192> data;
    for (int i=0; i < count; ++i) {
        for (int i=0; i < 192; ++i) {
            data[i] = rand() % 2;
        }
        // the data to be encrypted
        key_in.write(key);
        data_in.write(data);

        // handshake
        // tell active module to encrypt the data/key pair
        sync_in.write(true);
        // wait ack
        sync_out.read();
     }
}

void Top::producer_proc()
{
    // test series:

    activate(m1);
    test(453453, 8);

    activate(m2);
    test(23423, 5);

    activate(m1);
    test(90445, 4);

    activate(m2);
    test(10023823, 7);

    activate(m3);
    test(73342534, 12);
}

void Top::monitor_proc()
{
    sc_bv<192> data;
    while(true) {
        data_out.read(data);
        RC_COUTL("Top: encrypted output read."
                " (t=" << sc_time_stamp() << ")");
    }
}

//
// $Id: Top.cpp,v 1.2 2007/10/14 12:44:02 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/fifo_filter_sync2/Top.cpp,v $
//
