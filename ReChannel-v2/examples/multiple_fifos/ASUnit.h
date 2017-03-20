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
 * @file    ASUnit.h
 * @author  Andreas H. Niers          <niers@cs.uni-bonn.de>
 * @brief   A(dder)S(ubstractor)Unit: In this module the two reconfigurable
 *          modules add und sub are defined. If activate_module_adder is
 *          set to true the adder module is activated and the subtractor is 
 *          deactivated, if it is set to false the subtractor module is
 *          activated and the adder module is deactivated.
 *
 * \verbatim
 *
 *          +---------------#-------#----------------------------#--------------+
 *          |      operand1 #       # operand2        activate_modul_adder      | 
 *          |               #       #                                           |      
 *          |            (portal1)++X+++++++++++++++++++                        |                                       
 *          |            (    )     #                  +                        |
 *          |               +       #                  +                        |
 *          |               +     (portal2)++++++++++++x++++++++++              |
 *          |               +     (    )               +         +              | 
 *          |               +        +               +-#---------#-------+      |                               
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
 *          |                  +------------+      |        sub        |     +  |
 *          |       ASUnit                         |                   |     +  |
 *          |       asunti                         |    differenz      |     +  |      
 *          |                                      +--------#----------+     +  |                        
 *          |                                               +                +  | 
 *          |                                               +                +  |          
 *          |                                               +                +  |   
 *          |                                               +    (portal3)++++  |     
 *          |                                               +++++(    )         |
 *          |                                                       +           |
 *          |                                 +++++++++++++++++++++++           |
 *          |                                 +  result                         |
 *          +---------------------------------#---------------------------------+
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
 
#ifndef __ASUNIT_H
#define __ASUNIT_H

#include <systemc.h>
#include <ReChannel.h>

#include "Adder_rc.h"
#include "Subtractor_rc.h"


SC_MODULE(ASUnit){
    
    sc_fifo_in<sc_int<32> >	    operand1, operand2;
    sc_in<bool>                 activate_module_adder;
	sc_fifo_out<sc_int<32> >    result;

	void initialize(void); 
    void change_module(void);

	Adder_rc		    adder;
    Subtractor_rc       subtractor;
	rc_control		    ctrl;

	rc_fifo_in_portal<sc_int<32> >	portal1, portal2;
	rc_fifo_out_portal<sc_int<32> >	portal3;

    //datatypes and variales for control
    enum modules{ADDER, SUBTRACTOR} active_module;
    
	SC_CTOR(ASUnit)
		: adder("add"), subtractor("sub"), ctrl("ctrl"), portal1("portal1"), portal2("portal2"), portal3("portal3")
	{        
        //first bind the inputs
        portal1.static_port(operand1);
		portal1.dynamic_port(adder.summand1);
        portal1.dynamic_port(subtractor.minuend);
        
        portal2.static_port(operand2);
        portal2.dynamic_port(adder.summand2);
        portal2.dynamic_port(subtractor.subtrahend);   

        //then bind the outputs
		portal3.static_port(result);
		portal3.dynamic_port(adder.summe);
        portal3.dynamic_port(subtractor.differenz);

        //add both modules to the controller
		ctrl.add(adder + subtractor);

        //changes the active module depending on activate_module_adder
        //if activate_module adder is true the adder is active else the
        //subtractor is activated
        SC_THREAD(change_module);
        sensitive <<  activate_module_adder;
	}
};

#endif

