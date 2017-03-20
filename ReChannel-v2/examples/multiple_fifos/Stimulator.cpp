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
 * @file    Stimulator.cpp
 * @author  Andreas H. Niers          <niers@cs.uni-bonn.de>
 * @brief   Stimulator for multiple_fifos
 *
 */

#include "Stimulator.h"

void Stimulator::stimulate(void){
    
    //define number of testdata
    const unsigned beginFirstRun    = 0;
    const unsigned endFirstRun      = 4;
    const unsigned endSecondRun     = 8;
    const unsigned endThirdRun      = 12;
    
    //activate adder
    activate_adder.write(true);     
    wait(1,SC_NS);

    //test adder
    cout << "Stimulator::stimulate RUN NUMBER: 1 " << endl;
	for (unsigned counter=beginFirstRun; counter < endFirstRun; counter++) {            
            stim1.write(counter);
            stim2.write(0);          
		}
    
    //deactivate adder and activate the subtractor
    activate_adder.write(false);
    
    //waiting for reconfiguration
    wait(1,SC_NS);

    //test subtractor
    cout << "Stimulator::stimulate RUN NUMBER: 2 " << endl;
	for (unsigned counter=endFirstRun; counter < endSecondRun; counter++) {            
            stim1.write((-1) * counter);
            stim2.write(0);
		}

    //deactivate subtractor and activate the adder     
    activate_adder.write(true);
    
    //waiting for reconfiguration
    wait(1,SC_NS);
    
    //stimulate the adder again
    cout << "Stimulator::stimulate RUN NUMBER: 3 " << endl;
	for (unsigned counter=endSecondRun; counter <endThirdRun; counter++) {       
            stim1.write(counter);    
            stim2.write(0);
	}
}

