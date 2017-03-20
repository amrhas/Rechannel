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
 * @file    virtex4ctrl.h
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief
 *
 *
 * $Date: 2007/10/09 21:52:35 $
 * $Revision: 1.1 $
 */

#include <ReChannel.h>

#ifndef _VIRTEX4_CONTROL_INCLUDED_
#define _VIRTEX4_CONTROL_INCLUDED_

/**
 * A Virtex4 property class.
 */
class virtex4module {
public:
     virtual unsigned v4_bitfilesize() const =0;

protected:
     virtual ~virtex4module() {};
};

/**
 * A controller that mimicks Xilinx Virtex4's
 * reconfiguration behaviour is derived from rc_control
 * It calculates loading times of virtex4 targetted modules from their
 * bitfilesize in the overloaded takes_time() member function.
 *
 * Loading time is additionally influenced by the frequency the Virtex4's
 * configuration unit is running on ICAPFreq and if it runs in 32bit mode.
 */
class virtex4ctrl : public rc_control
{
public:
    // implement constructor and set members to sane defaults
    virtex4ctrl(  ) : rc_control(), Mode32(true), ICAPFreq(100) {};
    virtex4ctrl( const char* _name ) :
        rc_control(_name), Mode32(true), ICAPFreq(100) { }

    sc_time takes_time(
        const rc_reconfigurable& reconf,
        rc_reconfigurable::action_type action) const
    {
        const virtex4module* v4mod = DCAST<const virtex4module*>(&reconf);
        if (0!=v4mod) {                           // if module is virtex4 module
          switch (action) {
            case (rc_reconfigurable::LOAD):       // if module is loaded
            {
                double block=1.0;                 // respect 32Bit mode
                if (Mode32) block=4.0;
                return sc_time(                   // and calculate time
                     ((double)((*v4mod).v4_bitfilesize()) / block)
                     / ((double) ( ICAPFreq ) * 1000000.0 ), SC_SEC);
            }
            default: // in all other cases use times set with rc_set_delay()
                return reconf.rc_get_default_delay(action);
          }
        } else // if module is no virtex4 module use times set with rc_set_delay()
            return reconf.rc_get_default_delay(action);
    }
    // switch ICAP's 32Bit Mode on/off
    void set32BitMode(bool i_Mode32) {Mode32=i_Mode32;}
    
    // set frequency ICAP is running on
    void setICAPFreqMHz(unsigned i_ICAPFreq) {ICAPFreq=i_ICAPFreq;}

private:
    bool     Mode32;
    unsigned ICAPFreq;
};

#endif // _VIRTEX4_CONTROL_INCLUDED_

//
// $Id: virtex4ctrl.h,v 1.1 2007/10/09 21:52:35 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/examples/takes_time/virtex4ctrl.h,v $
//
