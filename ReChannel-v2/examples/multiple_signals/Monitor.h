// vim:set et sts=4 ts=4 tw=75 sw=4 ai ci cin:
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
 * This ReChannel library is distributed in the hope that it will be
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
 * @file    Monitor.h
 * @author  Andreas H. Niers          <niers@cs.uni-bonn.de>
 * @brief   Just monotoring...
 *
 * \verbatim
 *
 *      +---------------+
 *      |  Monitor      |
 *      +               |   <- mon1:   sc_in<sc_int<32> >
 *      |               |
 *      +---------------+
 *
 * \endverbatim
 */


#ifndef __MONITOR_H
#define __MONITOR_H

#include <systemc.h>

SC_MODULE(Monitor){
	sc_in<sc_int<32> >  mon1;

	void monitor(void);

	SC_CTOR(Monitor){
		SC_THREAD(monitor);
        sensitive << mon1;
	}
};

#endif

