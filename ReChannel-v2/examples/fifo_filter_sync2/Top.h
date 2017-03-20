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
 * $Date: 2007/10/09 21:52:06 $
 * $Revision: 1.1 $
 */

#include <systemc.h>
#include <ReChannel.h>

#include "CryptoModule.h"
#include "AES.h"
#include "Blowfish.h"
#include "TripleDES.h"

// types of the reconfigurable modules
typedef CryptoModule<AES<192> >       AES_rc;
typedef CryptoModule<Blowfish<192> >  Blowfish_rc;
typedef CryptoModule<TripleDES<192> > TripleDES_rc;

SC_MODULE(Top)
{
    // channels
    sc_fifo<sc_bv<192> > key_in;
    sc_fifo<sc_bv<192> > data_in;
    sc_fifo<sc_bv<192> > data_out;
    sc_fifo<bool>        sync_in;
    sc_fifo<bool>        sync_out;

    // portals
    rc_portal<sc_fifo_in<sc_bv<192> > >  key_in_portal;
    rc_portal<sc_fifo_in<sc_bv<192> > >  data_in_portal;
    rc_portal<sc_fifo_out<sc_bv<192> > > data_out_portal;
    rc_portal<sc_fifo_in<bool> >         sync_in_portal;
    rc_portal<sc_fifo_out<bool> >        sync_out_portal;

    // reconfiguration control
    rc_control ctrl;

    // reconfigurable modules
    AES_rc       m1;
    Blowfish_rc  m2;
    TripleDES_rc m3;

    SC_CTOR(Top)
        : ctrl("ctrl"), m1("AES_rc"), m2("Blowfish_rc"),
          m3("TripleDES_rc"), active_module(NULL)
    {
        // connect portals with the static side
        key_in_portal.static_port(key_in);
        data_in_portal.static_port(data_in);
        data_out_portal.static_port(data_out);
        sync_in_portal.static_port(sync_in);
        sync_out_portal.static_port(sync_out);

        // connect portals with the dynamic side
        key_in_portal.dynamic_port(m1.key_in);
        data_in_portal.dynamic_port(m1.data_in);
        data_out_portal.dynamic_port(m1.data_out);
        sync_in_portal.dynamic_port(m1.sync_in);
        sync_out_portal.dynamic_port(m1.sync_out);

        key_in_portal.dynamic_port(m2.key_in);
        data_in_portal.dynamic_port(m2.data_in);
        data_out_portal.dynamic_port(m2.data_out);
        sync_in_portal.dynamic_port(m2.sync_in);
        sync_out_portal.dynamic_port(m2.sync_out);

        key_in_portal.dynamic_port(m3.key_in);
        data_in_portal.dynamic_port(m3.data_in);
        data_out_portal.dynamic_port(m3.data_out);
        sync_in_portal.dynamic_port(m3.sync_in);
        sync_out_portal.dynamic_port(m3.sync_out);

        // set module's loading times
        m1.rc_set_delay(RC_LOAD, sc_time(1, SC_MS));
        m2.rc_set_delay(RC_LOAD, sc_time(1, SC_MS));
        m3.rc_set_delay(RC_LOAD, sc_time(1, SC_MS));

        // set up the reconfiguration controller
        ctrl.add(m1 + m2 + m3);
        // m1 shall be initially active
        ctrl.activate(m1);
        active_module = &m1;

        // producer thread sending the test data
        SC_THREAD(producer_proc);

        // monitors the output of the modules
        SC_THREAD(monitor_proc);
    }

private:

    // unloads currently active module, activates given module
    void activate(rc_reconfigurable& module);

    // sends #count test words to be encrypted with key
    void test(const sc_bv<192>& key, int count);

    void producer_proc();

    void monitor_proc();

private:

    // currently active reconfigurable module
    rc_reconfigurable* active_module;
};

//
// $Id: Top.h,v 1.1 2007/10/09 21:52:06 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/fifo_filter_sync2/Top.h,v $
//
