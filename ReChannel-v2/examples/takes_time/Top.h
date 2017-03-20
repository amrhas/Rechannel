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
 * $Date: 2007/10/09 21:52:35 $
 * $Revision: 1.1 $
 */

#include <systemc.h>
#include <ReChannel.h>

#include "modules.h"
#include "virtex4ctrl.h"

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

    // reconfiguration control (Virtex 4)
    virtex4ctrl v4ctrl;

    // reconfigurable modules
    Maximum_rc   m1;
    Serialize_rc m2;
    Swap_rc      m3;
    Repeat_rc    m4;
    Consume_rc   m5;

    SC_CTOR(Top)
        : in1_fifo(1), in2_fifo(1), out1_fifo(1), out2_fifo(1),
          v4ctrl("ctrl"), m1("Maximum_rc"), m2("Serialize_rc"),
          m3("Swap_rc"), m4("Repeat_rc"), m5("Consume_rc"),
          active_module(NULL)
    {
        // connect portals with the static side
        in1_portal.static_port(in1_fifo);
        in2_portal.static_port(in2_fifo);
        out1_portal.static_port(out1_fifo);
        out2_portal.static_port(out2_fifo);

        // connect portals with the dynamic side
        in1_portal.dynamic_port(m1.in1);
        in2_portal.dynamic_port(m1.in2);
        out1_portal.dynamic_port(m1.out1);
        out2_portal.dynamic_port(m1.out2);

        in1_portal.dynamic_port(m2.in1);
        in2_portal.dynamic_port(m2.in2);
        out1_portal.dynamic_port(m2.out1);
        out2_portal.dynamic_port(m2.out2);

        in1_portal.dynamic_port(m3.in1);
        in2_portal.dynamic_port(m3.in2);
        out1_portal.dynamic_port(m3.out1);
        out2_portal.dynamic_port(m3.out2);

        in1_portal.dynamic_port(m4.in1);
        in2_portal.dynamic_port(m4.in2);
        out1_portal.dynamic_port(m4.out1);
        out2_portal.dynamic_port(m4.out2);

        in1_portal.dynamic_port(m5.in1);
        in2_portal.dynamic_port(m5.in2);
        out1_portal.dynamic_port(m5.out1);
        out2_portal.dynamic_port(m5.out2);

        // set v4ctrl's properties
        v4ctrl.set32BitMode(true);
        v4ctrl.setICAPFreqMHz(100); // 100MHz

        // setup the reconfiguration controller
        v4ctrl.add(m1 + m2 + m3 + m4 + m5);

        // m1 shall be initially active
        v4ctrl.activate(m1);
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
// $Id: Top.h,v 1.1 2007/10/09 21:52:35 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/takes_time/Top.h,v $
//
