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
 * @file    main.cpp
 * @author  Andreas H. Niers          <niers@cs.uni-bonn.de>
 * @brief   Mainroutine for multiple_fifios. The wiring for the modules stim,
 *          asunit and mon are done here. Furthermore the simulation is
 *          started here. 
 *
 * \verbatim
 *
 *                      +-----------------------------------+ 
 *                      |            Stimulator             |
 *                      |               stim                |
 *                      |                                   |      
 *                      | stim1  stim2      activate_adder  |
 *                      +---#-------#---------------#-------+
 *                          #       #               #
 *                    fifo1 #       # fifo2         #    
 *                          #       #               #
 *                          #       #               #
 *          +---------------#-------#---------------#---------------------------+
 *          |      operand1 +       + operand2        activate_modul_adder      | 
 *          |               +       #                                           |      
 *          |            (portal1)+++++X+++++++++++++++++++                        |                                       
 *          |            (    )     #                  +                        |
 *          |               +       #                  +                        |
 *          |               +     (portal2)+++++++++++++++x++++++++++              |
 *          |               +     (    )               +         +              | 
 *          |               +        +                +---#---------#-----+     |                               
 *          |               +        +               |summand1  summand2 |      |
 *          |               +        +               |                   |      |  
 *          |               +        +               |    Adder_rc       |      |                   
 *          |               +        +               |       add         |      |
 *          |               +        +    ---------->|                   |      | 
 *          |               +        +    |          |      summe        |      |      
 *          |               +        +    |          +--------#----------+      |                        
 *          |               +        +    |                   +                 |    
 *          |               +        +    |                   ++++++++++++++++  |         
 *          |               +        +    |                                  +  |
 *          |               +        +++++x+++++++++++++++++++++++           +  |
 *          |               +             |                      +           +  |
 *          |               ++++++++++++++x+++++++++++++         +           +  |
 *          |                             |            +         +           +  |
 *          |                             |        +---#---------#-----+     +  |                               
 *          |                  +------------+      |minuend  subtrahend|     +  |
 *          |                  | rc_control |----->|                   |     +  |  
 *          |                  |   ctrl     |      |    Subtractor_rc  |     +  |
 *          |                  +------------+      |                   |     +  |
 *          |       ASUnit                         |       sub         |     +  |
 *          |       asunit                         |                   |     +  | 
 *          |                                      |    differenz      |     +  |      
 *          |                                      +--------#----------+     +  |                        
 *          |                                               +                +  | 
 *          |                                               +                +  |          
 *          |                                               +                +  |   
 *          |                                               +    (portal3)+++++++  |     
 *          |                                               +++++(    )         |
 *          |                                                       +           |
 *          |                                 +++++++++++++++++++++++           |
 *          |                                 +  result                         |
 *          +---------------------------------#---------------------------------+
 *                                            #
 *                                            # fifo3
 *                                            #
 *                          +-----------------#-----------------+ 
 *                          |               mon1                |
 *                          |                                   |
 *                          |             Monitor               |
 *                          |               mon                 |      
 *                          +-----------------------------------+
 *
 *  legend:
 *  #   fifo, sc_fifo_in or sc_fifo_out
 *  +   direct connection
 *  x   crossing point 
 *  ()  portal
 *  -   is controlling activation
 *  
 * \endverbatim
 *
 */

#ifndef __FIRSTRECHANNELTEST_CPP
#define __FIRSTRECHANNELTEST_CPP

#include <systemc.h>
#include <ReChannel.h>

#include "Stimulator.h"
#include "Monitor.h"
#include "ASUnit.h"

int sc_main(int argc, char* argv[]){
	
	sc_fifo<sc_int<32> > fifo1(4), fifo2(4), fifo3(4);
    sc_signal<bool>      activate_adder;
    
	Monitor mon("mon");
	mon.mon1(fifo3);

	ASUnit asunit("asunit");
	asunit.operand1(fifo1);
    asunit.operand2(fifo2);
	asunit.result(fifo3);
    asunit.activate_module_adder(activate_adder);

	Stimulator stim("stim");
	stim.stim1(fifo1);
    stim.stim2(fifo2);
    stim.activate_adder(activate_adder);

	sc_start();
	
	return 0;
}

#endif

