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
 * @file    reconf.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief
 *
 *
 * $Date: 2007/10/04 00:55:40 $
 * $Revision: 1.2 $
 */

#include "modules.h"
#include <ReChannel.h>

// (see description of module 'Maximum')
RC_RECONFIGURABLE_MODULE_DERIVED(Maximum_rc, Maximum)
{
    RC_RECONFIGURABLE_CTOR_DERIVED(Maximum_rc, Maximum),
        packetcount(0)
    {
        rc_add_filter(in1,  *new rc_fifo_in_filter<int> (tc1, +1));
        rc_add_filter(in2,  *new rc_fifo_in_filter<int> (tc1, +1));
        rc_add_filter(out1, *new rc_fifo_out_filter<int>(tc1, -1));
        rc_add_filter(out2, *new rc_fifo_out_filter<int>(tc1, -1));

        rc_add_filter(in1,
                *new rc_fifo_in_filter<int> (
                    RC_SYNC_CALLBACK(initcount),
                    RC_SYNC_CALLBACK(on_data_written)));
    }

    bool initcount(bool nb)
    {
        // appears to be double bookkeeping of number of accesses
        // but is not, due to additional filters manipulating tc1
        const unsigned packagesize=13;
        if (packetcount==0)
        {
            rc_possible_deactivation();
            tc1.modify(+packagesize);
            packetcount=packagesize;
            cout << "incr: " << packetcount << endl;
        }
        return true;
    }

    bool on_data_written(bool nb)
    {
        rc_assert(packetcount>0);
        tc1.modify(-1);
        --packetcount;
        cout << "decr: " << packetcount << endl;
        return true;
    }

private:
    unsigned packetcount;
    rc_transaction_counter tc1;
};

// (see description of module 'Serialize')
RC_RECONFIGURABLE_MODULE_DERIVED(Serialize_rc, Serialize)
{
    RC_RECONFIGURABLE_CTOR_DERIVED(Serialize_rc, Serialize)
    {
        rc_add_filter(in1,  *new rc_fifo_in_filter<int> (tc1, +2));
        rc_add_filter(in2,  *new rc_fifo_in_filter<int> (tc2, +2));
        rc_add_filter(out1, *new rc_fifo_out_filter<int>(tc1, -1));
        rc_add_filter(out2, *new rc_fifo_out_filter<int>(tc2, -1));
    }

private:
    rc_transaction_counter tc1, tc2;
};

// (see description of module 'Swap')
RC_RECONFIGURABLE_MODULE_DERIVED(Swap_rc, Swap)
{
    RC_RECONFIGURABLE_CTOR_DERIVED(Swap_rc, Swap),
        tcr1(tc1, 2, tc2, -1), tcr2(tc1, -1, tc2, 2)
    {
        rc_add_filter(in1,  *new rc_fifo_in_filter<int> (tcr1));
        rc_add_filter(in2,  *new rc_fifo_in_filter<int> (tcr2));
        rc_add_filter(out1, *new rc_fifo_out_filter<int>(tc1, -1));
        rc_add_filter(out2, *new rc_fifo_out_filter<int>(tc2, -1));
    }

private:
    rc_transaction_counter tc1, tc2, tcr1, tcr2;
};

// (see description of module 'Repeat')
RC_RECONFIGURABLE_MODULE_DERIVED(Repeat_rc, Repeat)
{
    RC_RECONFIGURABLE_CTOR_DERIVED(Repeat_rc, Repeat),
        tcr1(tc1, 3, tc2, -1), tcr2(tc1, -1, tc2, 3)
    {
        rc_add_filter(in1,  *new rc_fifo_in_filter<int> (tcr1));
        rc_add_filter(in2,  *new rc_fifo_in_filter<int> (tcr2));
        rc_add_filter(out1, *new rc_fifo_out_filter<int>(tc1, -1));
        rc_add_filter(out2, *new rc_fifo_out_filter<int>(tc2, -1));
    }

private:
    rc_transaction_counter tc1, tc2, tcr1, tcr2;
};

// (see description of module 'Consume')
RC_RECONFIGURABLE_MODULE_DERIVED(Consume_rc, Consume)
{
    RC_RECONFIGURABLE_CTOR_DERIVED(Consume_rc, Consume),
        tcr1(tc1, 1, tc2, -1), tcr2(tc1, -1, tc2, 1)
    {
        rc_add_filter(in1, *new rc_fifo_in_filter<int>(tcr1));
        rc_add_filter(in2, *new rc_fifo_in_filter<int>(tcr2));
    }

private:
    rc_transaction_counter tc1, tc2, tcr1, tcr2;
};

//
// $Id: reconf_modules.h,v 1.2 2007/10/04 00:55:40 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/test/fifo_filter_datapackage/reconf_modules.h,v $
//
