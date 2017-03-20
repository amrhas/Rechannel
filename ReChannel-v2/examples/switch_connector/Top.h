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
 * @file    Top.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief
 *
 *
 * $Date: 2007/10/09 21:52:20 $
 * $Revision: 1.1 $
 */

#include <systemc.h>
#include <ReChannel.h>

/*
 * The reconfigurable modules and the dynamic side's port map
 * are declared in this file:
 */
#include "modules.h"

// The port map for automatic binding of the static side:
// (note: exports and channels can be mapped, too)

RC_PORTMAP(
    staticPortMap, 4,     // name and size of the port map
    RC_PORT_TYPES(        // a list of port/export or channel types
        sc_fifo<int>,     // channel 0
        sc_fifo<int>,     // channel 1
        sc_fifo<int>,     // channel 2
        sc_fifo<int>      // channel 3
    )
);

SC_MODULE(Top)
{
    // channels
    sc_fifo<int> in1_fifo;
    sc_fifo<int> in2_fifo;
    sc_fifo<int> out1_fifo;
    sc_fifo<int> out2_fifo;

    // portals
    rc_portal<sc_fifo_in<int> >  in1_portal;
    rc_portal<sc_fifo_in<int> >  in2_portal;
    rc_portal<sc_fifo_out<int> > out1_portal;
    rc_portal<sc_fifo_out<int> > out2_portal;

    // reconfiguration control
    rc_control ctrl;

    // reconfigurable modules
    Maximum_rc   m1;
    Serialize_rc m2;
    Swap_rc      m3;
    Repeat_rc    m4;
    Consume_rc   m5;

    // port map for static side binding
    staticPortMap stat_portmap;

    // switch connector
    rc_switch_connector<dynPortMap> connector;

    SC_CTOR(Top)
        : in1_fifo(1), in2_fifo(1), out1_fifo(1), out2_fifo(1),
          ctrl("ctrl"), m1("Maximum_rc"), m2("Serialize_rc"),
          m3("Swap_rc"), m4("Repeat_rc"), m5("Consume_rc"),
          // initialise the static port map
          stat_portmap(in1_fifo, in2_fifo, out1_fifo, out2_fifo),
          // initialise the switch connector
          connector("connector", in1_portal, in2_portal, out1_portal, out2_portal),
          active_module(NULL)
    {
        // automatically connect portals with the static side
        connector.bind_static(stat_portmap);

        // automatically connect portals with the dynamic side
        connector.bind_dynamic(m1);
        connector.bind_dynamic(m2);
        connector.bind_dynamic(m3);
        connector.bind_dynamic(m4);
        connector.bind_dynamic(m5);

        // set module's loading times
        m1.rc_set_delay(RC_LOAD, sc_time(100, SC_NS));
        m2.rc_set_delay(RC_LOAD, sc_time(100, SC_NS));
        m3.rc_set_delay(RC_LOAD, sc_time(100, SC_NS));
        m4.rc_set_delay(RC_LOAD, sc_time(100, SC_NS));
        m5.rc_set_delay(RC_LOAD, sc_time(100, SC_NS));

        // setup the reconfiguration controller
        ctrl.add(m1 + m2 + m3 + m4 + m5);

        // m1 shall be initially active
        ctrl.activate(m1);
        active_module = &m1;

        // producer thread sending the test data
        SC_THREAD(producer_proc);

        // reconfiguration control
        SC_THREAD(control_proc);

        // monitors the output of the modules
        SC_THREAD(monitor_proc);
    }

private:

    // unloads currently active module, activates given module
    void activate(rc_reconfigurable& module);

    // returns a random number in the range [0..99]
    inline int rnd100()
        { return rand() % 100; }

    void producer_proc();

    void control_proc();

    void monitor_proc();

private:

    // currently active reconfigurable module
    rc_reconfigurable* active_module;
};

//
// $Id: Top.h,v 1.1 2007/10/09 21:52:20 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/switch_connector/Top.h,v $
//
