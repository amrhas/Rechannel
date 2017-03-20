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
 * @file    modules.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief
 *
 *
 * $Date: 2007/10/09 21:51:26 $
 * $Revision: 1.2 $
 */

#include <systemc.h>
#include <ReChannel.h>

/* Module 'Maximum' reads the data from its input and outputs the greatest
 * of the two integers, i.e. input (in1=x, in2=y) produces
 * (out1=max(x,y), out2=max(x,y)).
 */
// note: both input fifos are read before output is produced
RC_MODULE(Maximum)
{
    rc_fifo_in<int>  in1;
    rc_fifo_in<int>  in2;
    rc_fifo_out<int> out1;
    rc_fifo_out<int> out2;

    RC_CTOR(Maximum)
    {
        RC_THREAD(proc);
    }

private:
    void proc();
};

/* Module 'Serialize' reads the data from its input in parallel and
 * outputs it sequentially, i.e. input (in1=x, in2=y) produces
 * (out1=x, out2=x) and then (out1=y, out2=y).
 */
// note: both input fifos are read before second output is produced
RC_MODULE(Serialize)
{
    rc_fifo_in<int>  in1;
    rc_fifo_in<int>  in2;
    rc_fifo_out<int> out1;
    rc_fifo_out<int> out2;

    RC_CTOR(Serialize)
    {
        RC_THREAD(proc);
    }

private:
    void proc();
};

/* Module 'Swap' swaps the channel of the read data, i.e. input
 * (in1=x, in2=y) results in (out1=y, out2=x).
 */
// note: output is produced as soon as one fifo has been read
RC_MODULE(Swap)
{
    rc_fifo_in<int>  in1;
    rc_fifo_in<int>  in2;
    rc_fifo_out<int> out1;
    rc_fifo_out<int> out2;

    RC_CTOR(Swap)
    {
        RC_THREAD(proc);
    }

private:
    void proc();
};

/* Module 'Repeat' doubles all read data, i.e. input
 * (in1=x, in2=y) produces (out1=x, out2=y) twice.
 */
// note: output is produced as soon as one fifo has been read
RC_MODULE(Repeat)
{
    rc_fifo_in<int>  in1;
    rc_fifo_in<int>  in2;
    rc_fifo_out<int> out1;
    rc_fifo_out<int> out2;

    RC_CTOR(Repeat)
    {
        RC_THREAD(proc);
    }

private:
    void proc();
};

/* Module 'Consume' discards all read data.
 */
// note: both fifos are read, but no output is produced
RC_MODULE(Consume)
{
    rc_fifo_in<int>  in1;
    rc_fifo_in<int>  in2;
    rc_fifo_out<int> out1;
    rc_fifo_out<int> out2;

    RC_CTOR(Consume)
    {
        RC_THREAD(proc);
    }

private:
    void proc();
};

//
// $Id: modules.h,v 1.2 2007/10/09 21:51:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/explicit_fifo_sync/modules.h,v $
//
