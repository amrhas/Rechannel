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
 * @file    CryptoModule.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/10/14 05:32:26 $
 * $Revision: 1.6 $
 */

#include <systemc.h>
#include <ReChannel.h>

template<class M>
RC_RECONFIGURABLE_MODULE_DERIVED(CryptoModule, M)
{
    using M::key_in;
    using M::data_in;
    using M::data_out;

    // sync. handshake
    rc_fifo_in<int>   sync_in;
    rc_fifo_out<bool> sync_out;

    // constructor
    RC_RECONFIGURABLE_CTOR_DERIVED(CryptoModule, M),
        key_in_filter  (tc, +1,  0), // (initial read limit set to zero)
        data_in_filter (tc, +1,  0), // (initial read limit set to zero)
        data_out_filter(NULL, RC_SYNC_CALLBACK(on_data_written))
    {
        // assign filters to the ports
        rc_add_filter(key_in,   key_in_filter);
        rc_add_filter(data_in,  data_in_filter);
        rc_add_filter(data_out, data_out_filter);

        // process responsible for synchronization
        RC_THREAD(sync_proc);

        // number of data to be read by this module (initially set to zero)
        dc = 0;
    }

private:

    // called directly after an output word is written
    void on_data_written(bool nb);

    void sync_proc();

    RC_HAS_VAR(CryptoModule);
    // counter dc shall be reset on reconfiguration
    rc_resettable_var(int, dc);

    // transaction counter prevents untimely reconfigurations
    rc_transaction_counter          tc;

    // FIFO-filters
    rc_fifo_in_filter<sc_bv<192> >  key_in_filter;
    rc_fifo_in_filter<sc_bv<192> >  data_in_filter;
    rc_fifo_out_filter<sc_bv<192> > data_out_filter;

    // notified when all work is done (=> dc reaches zero)
    sc_event finished_event;
};

template<class M>
void CryptoModule<M>::on_data_written(bool nb)
{
    rc_assert(dc > 0);

    // decrement transaction counter
    tc.modify(-2);
    // decrement dc
    --dc;
    // is all work done?
    if (dc == 0) {
        finished_event.notify();
    }
}

template<class M>
void CryptoModule<M>::sync_proc()
{
    while(true) {
        // how many incoming requests shall be processed?
        int req = sync_in.read();
        // there are #req requests to be processed
        if (req > 0) {
            // increase dc
            dc += req;
            // increase read limit of the input filters
            key_in_filter.incr_read_limit(req);
            data_in_filter.incr_read_limit(req);

        // deactivation command received
        } else if (req < 0) {
            // wait until work is done
            if (dc > 0) {
                wait(finished_event);
            }
        }
        // send ack
        sync_out.write(true);
        // deactivation command received?
        if (req < 0) {
            // terminate process (no problem: will resurrect from the dead)
            return;
        }
    }
}

//
// $Id: CryptoModule.h,v 1.6 2007/10/14 05:32:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/fifo_filter_sync/CryptoModule.h,v $
//
