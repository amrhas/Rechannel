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
 * @file    Stimulator.h
 * @author  Andreas H. Niers          <niers@cs.uni-bonn.de>
 * @brief   Stimulator for multiple_fifos
 *
 * \verbatim
 *  
 *  +---------------+
 *  |  Stimulator   |
 *  |               ##   <- stim1:          sc_fifo_out<sc_int<32> >
 *  |               |
 *  |               ##   <- stim2:          sc_fifo_out<sc_int<32> >
 *  |               |
 *  |               ##   <- activate_adder: sc_fifo_out<bool>          
 *  |               |
 *  +---------------+
 * 
 * \endverbatim
 */

#ifndef STIMULATOR_H
#define STIMULATOR_H

#include <systemc.h>

SC_MODULE(Stimulator){
	sc_fifo_out<sc_int<32> >  stim1, stim2;
    sc_out<bool>         activate_adder;

	void stimulate(void);

	SC_CTOR(Stimulator){
		SC_THREAD(stimulate);
	}
};

#endif
