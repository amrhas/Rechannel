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
 * @file    TripleDES.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/10/14 12:44:02 $
 * $Revision: 1.2 $
 */

#include <systemc.h>

// include ReChannel to allow use of RC_STATIC_ASSERT_T and RC_COUTL
#include <ReChannel.h>

template<int W>
SC_MODULE(TripleDES)
{
    RC_STATIC_ASSERT_T(TripleDES_bit_width_check, W >= 168);

    sc_fifo_in<sc_bv<W> >  key_in;
    sc_fifo_in<sc_bv<W> >  data_in;
    sc_fifo_out<sc_bv<W> > data_out;

    SC_CTOR(TripleDES)
    {
        SC_THREAD(proc);
    }

protected:

    void proc();

    void calc_TripleDES(
        const sc_bv<W>& key, const sc_bv<W>& data, sc_bv<W>& out) const;
};

template<int W>
void TripleDES<W>::proc()
{
    sc_bv<W> key;
    sc_bv<W> data;
    sc_bv<W> out;

    while(true) {
        if (data_in.num_available() == 0) {
            wait(data_in.data_written_event());
        }
        data_in.read(data);
        RC_COUTL("TripleDES: data read. (t=" << sc_time_stamp() << ")");

        if (key_in.num_available() == 0) {
            wait(key_in.data_written_event());
        }
        key_in.read(key);
        RC_COUTL("TripleDES: key read. (t=" << sc_time_stamp() << ")");

        RC_COUTL("TripleDES: calculating..."
            " (t=" << sc_time_stamp() << ")");
        calc_TripleDES(key, data, out);
        wait(25, SC_US);

        if (data_out.num_free() == 0) {
            wait(data_out.data_read_event());
        }
        data_out.write(out);
        RC_COUTL("TripleDES: data written. (t=" << sc_time_stamp() << ")");
    }
}

template<int W>
void TripleDES<W>::calc_TripleDES(
        const sc_bv<W>& key, const sc_bv<W>& data, sc_bv<W>& out) const
{
    // dummy function
    out = key ^ data;
}

//
// $Id: TripleDES.h,v 1.2 2007/10/14 12:44:02 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/fifo_filter_sync2/TripleDES.h,v $
//
