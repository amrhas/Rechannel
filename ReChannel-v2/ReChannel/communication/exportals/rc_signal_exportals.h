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
 * Authors: Andreas Raabe and Armin Felke. Implementation by Armin Felke.
 *          {raabe, felke}@cs.uni-bonn.de
 */
/**
 * @file    rc_signal_exportals.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Exportals for sc_fifo channels.
 *
 * This file contains the exportals for the sc_signal_inout_if<T>
 * and sc_signal_in_if<T> interfaces.
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.8 $
 */

#ifndef RC_SIGNAL_EXPORTALS_H_
#define RC_SIGNAL_EXPORTALS_H_

#include "ReChannel/communication/exportals/rc_exportal.h"
#include "ReChannel/communication/accessors/rc_signal_accessors.h"

namespace ReChannel {

/**
 * @brief Exportal switch for a signal (input).
 */
template<class T>
RC_EXPORTAL_TEMPLATE(sc_signal_in_if<T>)
{
    RC_EXPORTAL_TEMPLATE_CTOR(sc_signal_in_if<T>)
    {
        RC_EXPORTAL_FORWARD_EVENT(value_changed_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_CLOSE()
    {
        this->notify_event("value_changed_event");
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
        
    }
};

/**
 * @brief Exportal switch for signal<bool> (input).
 */
RC_EXPORTAL(sc_signal_in_if<bool>)
{
    RC_EXPORTAL_CTOR(sc_signal_in_if<bool>)
    {
        RC_EXPORTAL_FORWARD_EVENT(value_changed_event);
        RC_EXPORTAL_FORWARD_EVENT(posedge_event);
        RC_EXPORTAL_FORWARD_EVENT(negedge_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_CLOSE()
    {
        this->notify_event("value_changed_event");
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
        if_type* const interface_ = this->get_interface();
        if (interface_ != NULL) {
            if (interface_->read() == true) {
                this->notify_event("posedge_event");
            } else {
                this->notify_event("negedge_event");
            }
        }
    }
};

/**
 * @brief Exportal switch for signal<sc_logic> (input).
 */
RC_EXPORTAL(sc_signal_in_if<sc_logic>)
{
    RC_EXPORTAL_CTOR(sc_signal_in_if<sc_logic>)
    {
        RC_EXPORTAL_FORWARD_EVENT(value_changed_event);
        RC_EXPORTAL_FORWARD_EVENT(posedge_event);
        RC_EXPORTAL_FORWARD_EVENT(negedge_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_CLOSE()
    {
        this->notify_event("value_changed_event");
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
        if_type* const interface_ = this->get_interface();
        assert(interface_ != NULL);
        if (interface_ != NULL) {
            const sc_logic value = interface_->read();
            if (value == SC_LOGIC_1) {
                this->notify_event("posedge_event");
            } else if (value == SC_LOGIC_0) {
                this->notify_event("negedge_event");
            }
        }
    }
};

/**
 * @brief Exportal switch for signal<sc_lv<W>> (input).
 */
template<int W>
RC_EXPORTAL_TEMPLATE(sc_signal_in_if<sc_lv<W> >)
{
    RC_EXPORTAL_TEMPLATE_CTOR(sc_signal_in_if<sc_lv<W> >)
    {
        RC_EXPORTAL_FORWARD_EVENT(value_changed_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_CLOSE()
    {
        this->notify_event("value_changed_event");
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
    }
};

/**
 * @brief Exportal switch for a signal (input+output).
 */
template<class T>
RC_EXPORTAL_TEMPLATE(sc_signal_inout_if<T>)
{
    RC_EXPORTAL_TEMPLATE_CTOR(sc_signal_inout_if<T>)
    {
        RC_EXPORTAL_FORWARD_EVENT(value_changed_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_CLOSE()
    {
        this->notify_event("value_changed_event");
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
    }
};

/**
 * @brief Exportal switch for signal<bool> (input+output).
 */
RC_EXPORTAL(sc_signal_inout_if<bool>)
{
    RC_EXPORTAL_CTOR(sc_signal_inout_if<bool>)
    {
        RC_EXPORTAL_FORWARD_EVENT(value_changed_event);
        RC_EXPORTAL_FORWARD_EVENT(posedge_event);
        RC_EXPORTAL_FORWARD_EVENT(negedge_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_CLOSE()
    {
        this->notify_event("value_changed_event");
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
        if_type* const interface_ = this->get_interface();
        assert(interface_ != NULL);
        if (interface_ != NULL) {
            if (interface_->read() == true) {
                this->notify_event("posedge_event");
            } else {
                this->notify_event("negedge_event");
            }
        }
    }
};

/**
 * @brief Exportal switch for signal<sc_logic> (input+output).
 */
RC_EXPORTAL(sc_signal_inout_if<sc_logic>)
{
    RC_EXPORTAL_CTOR(sc_signal_inout_if<sc_logic>)
    {
        RC_EXPORTAL_FORWARD_EVENT(value_changed_event);
        RC_EXPORTAL_FORWARD_EVENT(posedge_event);
        RC_EXPORTAL_FORWARD_EVENT(negedge_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_CLOSE()
    {
        // write 'Z' to the channel (for each registered driver)
        rc_interface_wrapper<if_type>* const ifw =
            this->get_interface_wrapper();
        assert(ifw != NULL);
        if (ifw != NULL) {
            const int drv_count = ifw->get_nb_driver_count();
            for (int i=0; i < drv_count; i++) {
                ifw->get_nb_driver_access(i)->call(
                    &if_type::write, SC_LOGIC_Z);
            }
        }
        this->notify_event("value_changed_event");
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
        if_type* const interface_ = this->get_interface();
        assert(interface_ != NULL);
        if (interface_ != NULL) {
            const sc_logic value = interface_->read();
            if (value == SC_LOGIC_1) {
                this->notify_event("posedge_event");
            } else if (value == SC_LOGIC_0) {
                this->notify_event("negedge_event");
            }
        }
    }
};

/**
 * @brief Exportal switch for signal<sc_lv<W>> (input+output).
 */
template<int W>
RC_EXPORTAL_TEMPLATE(sc_signal_inout_if<sc_lv<W> >)
{
    RC_EXPORTAL_TEMPLATE_CTOR(sc_signal_inout_if<sc_lv<W> >)
    {
        RC_EXPORTAL_FORWARD_EVENT(value_changed_event);
    }

    RC_ON_OPEN()
    {
        this->refresh_notify();
    }

    RC_ON_CLOSE()
    {
        // write 'Z..Z' to the static channel (for each registered driver)
        rc_interface_wrapper<if_type>* const ifw =
            this->get_interface_wrapper();
        assert(ifw != NULL);
        if (ifw != NULL) {
            const int drv_count = ifw->get_nb_driver_count();
            for (int i=0; i < drv_count; i++) {
                ifw->get_nb_driver_access(i)->call(
                    &if_type::write, s_ZZZ);
            }
        }
        this->notify_event("value_changed_event");
    }

    RC_ON_REFRESH_NOTIFY()
    {
        this->notify_event("value_changed_event");
    }

private:
    static const sc_dt::sc_lv<W> s_ZZZ;
};

template<int W>
const typename sc_dt::sc_lv<W>
rc_exportal<sc_signal_inout_if<sc_lv<W> > >::s_ZZZ = sc_lv<W>('Z');

} // namespace ReChannel

#endif // RC_SIGNAL_EXPORTALS_H_
//
// $Id: rc_signal_exportals.h,v 1.8 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/exportals/rc_signal_exportals.h,v $
//
