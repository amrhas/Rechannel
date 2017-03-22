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
 * @file    main.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @brief   Feature tests
 *
 *
 * $Date: 2007/12/06 12:27:21 $
 * $Revision: 1.7 $
 */

#include <ReChannel.h>

struct M_rc_channel_tag {};

typedef rc_portmap<
    M_rc_channel_tag,
    sc_in<bool>,
    sc_signal<bool>,
    sc_buffer<int>,
    sc_buffer<int>,
    sc_fifo<int>,
    sc_fifo<int>,
    sc_fifo<int>,
    sc_signal_rv<8>,
    sc_signal_rv<8> > M_rc_channel_pm;

RC_PORTMAP(
    M_rc_pm, 9,
    RC_PORT_TYPES(
        sc_in<bool>,
        sc_in<bool>,
        sc_in<int>,
        sc_out<int>,
        sc_fifo_in<int>,
        sc_fifo_in<int>,
        sc_fifo_out<int>,
        sc_in_rv<8>,
        sc_out_rv<8>
    )
);

RC_MODULE(M)
{
/* ports */

    rc_in<bool> clk;
    rc_in<bool> reset;
    rc_in<int>  a;
    rc_out<int> b;

    rc_fifo_in<int>  A;
    rc_fifo_in<int>  B;
    rc_fifo_out<int> C;

    rc_in_rv<8>  in;
    rc_out_rv<8> out;

    rc_port<sc_mutex_if> mutex;

/* channels */

    rc_fifo<int>       fifo;
    rc_signal<int>     signal;
    rc_signal<bool>    reset_sig;
    rc_buffer<bool>    signal2;
    rc_signal_rv<4>    signal3;
    rc_signal_resolved sig_res;
    rc_signal_rv<4>    scsigrv;

/* variables */

    RC_HAS_VAR(M);
    rc_resettable_var(int, j);
    rc_resettable_var(bool, mode);

/* events */

    rc_event e;
    rc_event rproc_start_event;

/* other */

    rc_process_reset preset;

/* constructor */

    RC_CTOR(M)
    {
        RC_METHOD(method_proc);
        sensitive << e;
        dont_initialize();

        RC_THREAD(proc);
        signal3 = "1111";

        RC_METHOD(proc2);
        sensitive << in;

        RC_CTHREAD(cproc, clk.pos());
        reset_signal_is(reset, true);

        RC_THREAD(rproc);
        reset_is(preset);

        RC_THREAD(rtrigger_proc);

        j = 0;
        signal = 1;
        signal3 = "Z1ZZ";
        mode = false;
    }

    void change_mode(bool new_mode)
    {
        mode = !mode;
        preset.trigger();
        wait(rproc_start_event);
    }

/* processes */

    void rtrigger_proc()
    {
        while(true) {
            wait(40, SC_NS);
            change_mode(true);
            wait(40, SC_NS);
            change_mode(false);
        }
    }

    void rproc()
    {
        rproc_start_event.notify();

        RC_COUTL("*** rproc() *** (t=" << sc_time_stamp() << ")");
        if (mode == false) {
            RC_COUTL("*** rproc() *** [reset mode 0]"
                " (t=" << sc_time_stamp() << ")");
        } else {
            RC_COUTL("*** rproc() *** [reset mode 1]"
                " (t=" << sc_time_stamp() << ")");
        }
        RC_NO_RESET {
            wait(41, SC_NS);
        }
        while(true) {
            wait(4000, SC_NS);
        }
    }

    void method_proc()
    {
        RC_COUTL("*** method_proc() ***"
            " (t=" << sc_time_stamp() << ")");
    }

    void proc()
    {
        e.notify(120, SC_NS);
        RC_COUTL("fifo: " << fifo.num_available() << "");
        RC_COUTL("SIGNAL: " << signal.read() << "");
        RC_COUTL("SIGNAL3: " << signal3.read() << "");
        RC_COUTL("SCSIGRV: " << scsigrv.read() << "");
        RC_COUTL("SIG_RES: " << sig_res.read() << "");
        while(true)
        {
            fifo.nb_write(2);
            signal = 5;
            signal3 = "1010";
            sig_res = SC_LOGIC_0;
            scsigrv = "1111";

            RC_FORK
                rc_spawn(rc_bind(&M::proc3, this), "spawned1"),
                rc_spawn(rc_bind(&M::proc3, this), "spawned2"),
            RC_JOIN

// blocking (variant one)
#if 1
            int a = A.read();
            //RC_TRANSACTION {
                RC_COUTL(basename() << ": A = " << a
                    << " (t=" << sc_time_stamp() << ")");
                int b = B.read();
                RC_COUTL(basename() << ": B = " << b
                    << " (t=" << sc_time_stamp() << ")");
                int c = a * b;
                RC_COUTL(basename() << ": calculating..."
                    " (t=" << sc_time_stamp() << ")");
                sig_res = SC_LOGIC_0;
                wait(50, SC_NS);
                RC_COUTL(basename() << ": calculated A * B = " << c
                    << " (t=" << sc_time_stamp() << ")");
                C.write(c);
            //}
#endif

// non-blocking (variant two)
#if 0
            int a, b, c = -1;
            if (A.num_available() <= 0) {
                wait(A.data_written_event());
            }
            A.nb_read(a);
            RC_COUTL(basename() << ": A = " << a << " (t=" << sc_time_stamp() << ")");
            if (B.num_available() <= 0) {
                wait(B.data_written_event());
            }
            B.nb_read(b);
            RC_COUTL(basename() << ": B = " << b << " (t=" << sc_time_stamp() << ")");
            c = a * b;
            RC_COUTL(basename() << ": calculating... (t=" << sc_time_stamp() << ")");
            wait(50, SC_NS);
            RC_COUTL(basename() << ": calculated A * B = " << c << " (t=" << sc_time_stamp() << ")");
            if (C.num_free() <= 0) {
                wait(C.data_read_event());
            }
            C.nb_write(c);
#endif

// non-blocking (variant three)
#if 0
            int a, b, c = -1;
            while (A.nb_read(a) == false) {
                wait(A.data_written_event());
            }
            RC_COUTL(basename() << ": A = " << a << " (t=" << sc_time_stamp() << ")");
            while (B.nb_read(b) == false) {
                wait(B.data_written_event());
            }
            RC_COUTL(basename() << ": B = " << b << " (t=" << sc_time_stamp() << ")");
            c = a * b;
            RC_COUTL(basename() << ": calculating... (t=" << sc_time_stamp() << ")");
            wait(50, SC_NS);
            RC_COUTL(basename() << ": calculated A * B = " << c << " (t=" << sc_time_stamp() << ")");
            while (C.nb_write(c) == false) {
                wait(C.data_read_event());
            }
#endif

            RC_COUTL("fifo: " << fifo.num_available() << "");
            RC_COUTL("SIGNAL: " << signal.read() << "");
            RC_COUTL("SIGNAL3: " << signal3.read() << "");
            RC_COUTL("SCSIGRV: " << scsigrv.read() << "");
            RC_COUTL("SIG_RES: " << sig_res.read() << "");
            RC_COUTL(j << "");
            j = 6;
        }
    }

    void proc2()
    {
        sc_lv<8> temp = in.read();
        out.write(temp);
    }

    void proc3()
    {
        rc_process_handle hproc = rc_get_current_process_handle();
        RC_COUTL("RC_THREAD '" << hproc.name() << "' called"
            " (t=" << sc_time_stamp() << ")");
        wait(1, SC_NS);
        RC_COUTL("RC_THREAD '" << hproc.name() << "' terminated"
            " (t=" << sc_time_stamp() << ")");
    }

    void cproc()
    {
        if (reset.read() == true) {
            RC_COUTL("cproc() reset (t=" << sc_time_stamp() << ")");
        }
        while(true) {
            RC_COUTL("cproc() step 1 (t=" << sc_time_stamp() << ")");
            b = -a;
            wait(1);
            RC_COUTL("cproc() step 2 (t=" << sc_time_stamp() << ")");
            b = 2*a;
            wait(1);
            RC_COUTL("cproc() step 3 (t=" << sc_time_stamp() << ")");
            b = a - 1;
            wait(1);
        }
    }
};

RC_RECONFIGURABLE_MODULE_DERIVED(M_rc, M)
{
    M_rc_pm portmap;

    RC_RECONFIGURABLE_CTOR_DERIVED(M_rc, M),
        portmap(clk, reset, a, b, A, B, C, in, out),
        dc(2),
        filterA(tc, +1, 1),
        filterB(tc, +1, 1),
        filterC(NULL, RC_SYNC_CALLBACK(on_C_written)),
        i1(tc, +1), i2(tc, +1), i3(tc, +1),
        o1(tc, -1), o2(tc, -1), o3(tc, -1)
    {
        this->rc_add_portmap(portmap);

        this->rc_add_filter(A, i1);
        this->rc_add_filter(C, o1);

        this->rc_add_filter(A, filterA);
        this->rc_add_filter(B, filterB);
        this->rc_add_filter(C, filterC);

        this->rc_add_filter(A, i2);
        this->rc_add_filter(A, i3);

        this->rc_add_filter(C, o2);
        this->rc_add_filter(C, o3);

        this->rc_add_filter(mutex, *new rc_mutex_filter(tc));

        RC_THREAD(proc);
    }

    void proc()
    {
        mutex->lock();
        wait(200, SC_NS);
        mutex->unlock();
    }

private:

    void on_C_written(bool nb);

private:

    RC_HAS_VAR(M_rc);
    rc_resettable_var(int, dc);

    rc_transaction_counter  tc;
    rc_fifo_in_filter<int>  filterA;
    rc_fifo_in_filter<int>  filterB;
    rc_fifo_out_filter<int> filterC;

    rc_fifo_in_filter<int>  i1;
    rc_fifo_in_filter<int>  i2;
    rc_fifo_in_filter<int>  i3;
    rc_fifo_out_filter<int> o1;
    rc_fifo_out_filter<int> o2;
    rc_fifo_out_filter<int> o3;
};

void M_rc::on_C_written(bool nb)
{
    tc.modify(-2);
    --dc;
    if (dc > 0) {
        filterA.incr_read_limit();
        filterB.incr_read_limit();
    }
}

SC_MODULE(test)
{
    sc_in<int>  in;
    sc_out<int> out;

    SC_CTOR(test) {
        SC_METHOD(proc);
        sensitive << in;
    }
    void proc() {
        out.write(in.read() + 1);
    }
};

typedef rc_reconfigurable_module<test> test_rc;

SC_MODULE(wrapper)
{
    sc_in<int>  in;
    sc_out<int> out;

    rc_in_portal<int>  pin;
    rc_out_portal<int> pout;

    test_rc A;
    test_rc B;

    rc_control ctrl;

    SC_CTOR(wrapper)
        : A("A"), B("B")
    {
        pin.bind_static(in);
        pout.bind_static(out);

        pin.bind_dynamic(A.in);
        pout.bind_dynamic(A.out);

        pin.bind_dynamic(B.in);
        pout.bind_dynamic(B.out);

        ctrl.add(A + B);
        ctrl.remove(A + B);
        ctrl.add(A);
        ctrl.add(B);

        SC_THREAD(proc);
    }

    void proc() {
		wait(10, SC_NS);
        ctrl.activate(A);
        RC_COUTL("A has been activated.");
        wait(10, SC_NS);
        ctrl.unload(A);
        ctrl.activate(B);
        RC_COUTL("B has been activated.");
    }
};

SC_MODULE(testbench)
{
/* channels */

    sc_in<bool>     clk;
    sc_signal<bool> reset;
    sc_buffer<int>  asig;
    sc_buffer<int>  bsig;
    sc_fifo<int>    A;
    sc_fifo<int>    B;
    sc_fifo<int>    C;
    sc_signal_rv<8> in;
    sc_signal_rv<8> out;
    sc_mutex mutex;

    sc_in<bool>     clk_2;
    sc_signal<bool> reset_2;
    sc_buffer<int>  asig_2;
    sc_buffer<int>  bsig_2;
    sc_fifo<int>    A_2;
    sc_fifo<int>    B_2;
    sc_fifo<int>    C_2;
    sc_signal_rv<8> in_2;
    sc_signal_rv<8> out_2;
    sc_mutex mutex_2;

    sc_signal<int>  test_in;
    sc_signal<int>  test_out;

/* switches */

    rc_portal<sc_in<bool> >      pclk;
    rc_portal<sc_in<bool> >      preset;
    rc_portal<sc_in<int> >       pa;
    rc_portal<sc_out<int> >      pb;
    rc_portal<sc_fifo_in<int> >  pA;
    rc_portal<sc_fifo_in<int> >  pB;
    rc_portal<sc_fifo_out<int> > pC;
    rc_portal<sc_in_rv<8> >      pin;
    rc_portal<sc_out_rv<8> >     pout;

    rc_switch_connector<M_rc_pm> connector;

    rc_portal<sc_in<bool> >      pclk_2;
    rc_portal<sc_in<bool> >      preset_2;
    rc_portal<sc_in<int> >       pa_2;
    rc_portal<sc_out<int> >      pb_2;
    rc_portal<sc_fifo_in<int> >  pA_2;
    rc_portal<sc_fifo_in<int> >  pB_2;
    rc_portal<sc_fifo_out<int> > pC_2;
    rc_portal<sc_in_rv<8> >      pin_2;
    rc_portal<sc_out_rv<8> >     pout_2;

    rc_switch_connector<M_rc_pm> connector_2;

    rc_mutex_portal pmutex;

    rc_control ctrl;
    rc_control ctrl_2;

    M_rc m1;
    M_rc m2;

    wrapper test_wrapper;

    SC_CTOR(testbench)
        : A(1), B(1), C(1), A_2(1), B_2(1), C_2(1),
          connector(
            "connector", pclk, preset, pa, pb, pA,
            pB, pC, pin, pout),
          connector_2(
            "connector_2", pclk_2, preset_2, pa_2,
            pb_2, pA_2, pB_2, pC_2, pin_2, pout_2),
          ctrl("ctrl"), ctrl_2("ctrl_2"), m1("m1"), m2("m2"),
          test_wrapper("test_wrapper")
    {
        connector.bind_static(
            M_rc_channel_pm(
                clk, reset, asig, bsig, A, B, C, in, out));

        connector_2.bind_static(
            M_rc_channel_pm(
                clk_2, reset_2, asig_2, bsig_2,
                A_2, B_2, C_2, in_2, out_2));

        connector.bind_dynamic(m1);
        connector.bind_dynamic(m2);

        pmutex.bind_static(mutex);
        pmutex.bind_dynamic(m1.mutex);
        pmutex.bind_dynamic(m2.mutex);

        m1.rc_set_delay(RC_LOAD, sc_time(10, SC_NS));
        m2.rc_set_delay(RC_LOAD, sc_time(10, SC_NS));

        ctrl.add(m1 + m2);

        SC_THREAD(stimulator);

        SC_THREAD(controller);

        SC_THREAD(monitor);

        SC_METHOD(monitor2);
        sensitive << out;

        SC_METHOD(monitor3);
        sensitive << bsig;

        test_wrapper.in(test_in);
        test_wrapper.out(test_out);
    }

    void stimulator()
    {
        asig = 8;
        in = "11011010";
        RC_COUTL("stimulator: in := 11011010"
            " (t=" << sc_time_stamp() << ")");

        A.write(10);
        B.write(12);

        A.write(24);
        B.write(23);

        asig = 16;
        in = "00001111";
        RC_COUTL("stimulator: in := 00001111"
            " (t=" << sc_time_stamp() << ")");

        B.write(47);
        A.write(1);

        A.write(4);
        B.write(35);

        asig = 32;
        in = "ZZZZZZZZ";
        RC_COUTL("stimulator: in := ZZZZZZZZ"
            " (t=" << sc_time_stamp() << ")");

        RC_COUTL("stimulator: reset = true"
            " (t=" << sc_time_stamp() << ")");
        reset = true;

        B.write(354);
        A.write(234);

        asig = 64;
        in = "1010ZZ00";
        RC_COUTL("stimulator: in := 1010ZZ00"
            " (t=" << sc_time_stamp() << ")");

        A.write(82);
        B.write(32);

        RC_COUTL("stimulator: reset = false"
            " (t=" << sc_time_stamp() << ")");
        reset = false;

        B.write(93);
        A.write(34);        
    }

    void controller()
    {
        ctrl.activate(m1);
        RC_COUTL("controller: m1 has been activated"
            " (t=" << sc_time_stamp() << ")");
        wait(59, SC_NS);
        ctrl.unload(m1);
        RC_COUTL("controller: m1 has been unloaded"
            " (t=" << sc_time_stamp() << ")");

        // testing rc_control::move() and rc_control::remove()
        ctrl.move(m1, connector_2);
        ctrl.remove(m1);
        ctrl_2.add(m1);
        ctrl_2.move(m1, connector);
        ctrl_2.load(m1);
        ctrl_2.unload(m1);
        ctrl_2.remove(m1);
        ctrl.add(m1);
        // proceed as normal

        ctrl.activate(m2);
        RC_COUTL("controller: m2 has been activated"
            " (t=" << sc_time_stamp() << ")");
        wait(59, SC_NS);
        ctrl.unload(m2);
        RC_COUTL("controller: m2 has been unloaded"
            " (t=" << sc_time_stamp() << ")");
        ctrl.activate(m1);
        RC_COUTL("controller: m1 has been activated"
            " (t=" << sc_time_stamp() << ")");
        RC_COUTL("bsig = " << bsig.read());
        wait(59, SC_NS);
        ctrl.unload(m1);
        RC_COUTL("controller: m1 has been unloaded"
            " (t=" << sc_time_stamp() << ")");
        ctrl.activate(m2);
        RC_COUTL("controller: m2 has been activated"
            " (t=" << sc_time_stamp() << ")");
    }

    void monitor()
    {
        while(true) {
            int c = C.read();
            RC_COUTL("monitor: C = " << c
                << " (t=" << sc_time_stamp() << ")");
        }
    }

    void monitor2()
    {
        sc_lv<8> value = out.read();
        RC_COUTL("monitor2: out = " << value
            << " (t=" << sc_time_stamp() << ")");
    }

    void monitor3()
    {
        int value = bsig.read();
        RC_COUTL("monitor3: bsig = " << value
            << " (t=" << sc_time_stamp() << ")");
    }
};

//
// $Id: main.h,v 1.7 2007/12/06 12:27:21 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/test/overalltest/main.h,v $
//
