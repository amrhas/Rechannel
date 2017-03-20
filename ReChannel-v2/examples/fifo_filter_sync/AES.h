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
 * @file    AES.h
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
SC_MODULE(AES)
{
    RC_STATIC_ASSERT_T(
        AES_bit_width_check, W == 128 || W == 192 || W == 256);

    sc_fifo_in<sc_bv<W> >  key_in;
    sc_fifo_in<sc_bv<W> >  data_in;
    sc_fifo_out<sc_bv<W> > data_out;

    SC_CTOR(AES)
    {
        SC_THREAD(proc);
    }

protected:

    void proc();

    void calc_AES(
        const sc_bv<W>& key, const sc_bv<W>& data, sc_bv<W>& out) const;
};

template<int W>
void AES<W>::proc()
{
    sc_bv<W> key;
    sc_bv<W> data;
    sc_bv<W> out;

    while(true) {
        key_in.read(key);
        RC_COUTL("AES: key read. (t=" << sc_time_stamp() << ")");
        data_in.read(data);
        RC_COUTL("AES: data read. (t=" << sc_time_stamp() << ")");
        RC_COUTL("AES: calculating..."
            " (t=" << sc_time_stamp() << ")");
        calc_AES(key, data, out);
        wait(15, SC_US);
        data_out.write(out);
        RC_COUTL("AES: data written. (t=" << sc_time_stamp() << ")");
    }
}

template<int W>
void AES<W>::calc_AES(
        const sc_bv<W>& key, const sc_bv<W>& data, sc_bv<W>& out) const
{
    // dummy function
    out = ~(key ^ data);
}

//
// $Id: AES.h,v 1.2 2007/10/14 12:44:02 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/fifo_filter_sync/AES.h,v $
//
