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
 * @file    ASUnit.cpp
 * @author  Andreas H. Niers          <niers@cs.uni-bonn.de>
 * @brief   Implemantation of the A(dder)S(ubtractor)Unit
 *
 */

#ifndef __ASUNIT_CPP
#define __ASUNIT_CPP

#include "ASUnit.h"


void
ASUnit::change_module(void)
{

    bool do_activate_adder;
    bool first_activation = true;
    active_module = SUBTRACTOR;

    while(true){

        wait();

        do_activate_adder = activate_module_adder.read();

        cout << sc_time_stamp() << "\tASUnit::change_module checking... " << endl;

        if( (do_activate_adder == true) && (active_module != ADDER))
        {
            if(!first_activation){
                cout << sc_time_stamp() << "\tASUnit::change_module(): trying to unload subtractor" << endl;
                ctrl.unload(subtractor);
                cout << sc_time_stamp() << "\tASUnit::change_module(): deactivated subtractor" << endl;
            }
            first_activation = false;

            cout << sc_time_stamp() << "\tASUnit::change_module(): trying to activate adder" << endl;
            ctrl.activate(adder);
            active_module = ADDER;
            cout << sc_time_stamp() << "\tASUnit::change_module(): activated adder" << endl;
        }

        if( (do_activate_adder == false) && (active_module != SUBTRACTOR))
        {
            if(!first_activation){
                cout << sc_time_stamp() << "\tASUnit::change_module(): trying to unload adder" << endl;
                ctrl.unload(adder);
                cout << sc_time_stamp() << "\tASUnit::change_module(): deactivated adder" << endl;
            }
            first_activation = false;

            cout << sc_time_stamp() << "\tASUnit::change_module(): trying to activate subtractor" << endl;
            ctrl.activate(subtractor);
            active_module = SUBTRACTOR;
            cout << sc_time_stamp() << "\tASUnit::change_module(): activated subtractor" << endl << flush;
        }
    }

}

#endif

