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
 * $Revision: 1.2 $
 */

#include <systemc.h>
#include <ReChannel.h>

template<class M>
RC_RECONFIGURABLE_MODULE_DERIVED(CryptoModule, M)
{
    // sync. handshake
    rc_fifo_in<bool>  sync_in;
    rc_fifo_out<bool> sync_out;

    // constructor
    RC_RECONFIGURABLE_CTOR_DERIVED(CryptoModule, M),
        key_in_filter  (NULL, NULL, 0), // (no callbacks. initial read limit set to zero)
        data_in_filter (NULL, NULL, 0), // (no callbacks. initial read limit set to zero)
        data_out_filter(NULL, RC_SYNC_CALLBACK(on_data_written)) // (callback after access)
    {
        // assign filters to the ports
        rc_add_filter(this->key_in,   key_in_filter);
        rc_add_filter(this->data_in,  data_in_filter);
        rc_add_filter(this->data_out, data_out_filter);

        // process responsible for synchronization
        RC_THREAD(sync_proc);
    }

private:

    // called directly after an output word is written
    void on_data_written(bool nb);

    void sync_proc();

    // FIFO-filters
    rc_fifo_in_filter<sc_bv<192> >  key_in_filter;
    rc_fifo_in_filter<sc_bv<192> >  data_in_filter;
    rc_fifo_out_filter<sc_bv<192> > data_out_filter;

    // notified when output data has been written
    sc_event data_written_event;
};

template<class M>
void CryptoModule<M>::on_data_written(bool nb)
{
    // notify data_written_event
    data_written_event.notify();
}

template<class M>
void CryptoModule<M>::sync_proc()
{
    RC_TRANSACTION { // begin transaction

        while(true) {
            // read the next sync command
            bool sync_cmd = sync_in.read();

            if (sync_cmd == false) { // deactivation command received
                // send ok
                sync_out.write(true);
                // terminate process
                return; // (no problem: will resurrect from the dead)
            }

            // increase read limit of the input filters by one
            key_in_filter.incr_read_limit(1);
            data_in_filter.incr_read_limit(1);

            // wait for output data has been written
            wait(data_written_event);

            // send ok
            sync_out.write(true);
        }

    } // end transaction
}

//
// $Id: CryptoModule.h,v 1.2 2007/10/14 05:32:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/fifo_filter_sync2/CryptoModule.h,v $
//
