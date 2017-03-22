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
 * @file    synthesis_header.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Defines macros to allow synthesis of RC_MODULEs.
 *
 * Replaces "rc_.." by "sc_.." keywords for the use with a synthesis tool.
 *
 * $Date: 2007/10/14 12:42:29 $
 * $Revision: 1.5 $
 */

#ifndef RC_SYNTHESIS_HEADER_H_
#define RC_SYNTHESIS_HEADER_H_

#include <systemc.h>

#if !defined(RC_MAKE_DOXYGEN_DOCS)

#define ReChannel sc_core

#define rc_module                        sc_module
#define rc_behavior                      sc_behavior
#define rc_channel                       sc_channel
#define rc_module_name                   sc_module_name
#define RC_MODULE(user_module_name)      SC_MODULE(user_module_name)
#define RC_CTOR(user_module_name)        SC_CTOR(user_module_name)
#define RC_HAS_PROCESS(user_module_name) SC_HAS_PROCESS(user_module_name)
#define RC_METHOD(func)                  SC_METHOD(func)
#define RC_THREAD(func)                  SC_THREAD(func)
#define RC_CTHREAD(func, edge)           SC_CTHREAD(func)

#define rc_signal             sc_signal
#define rc_buffer             sc_buffer
#define rc_signal_resolved    sc_signal_resolved
#define rc_signal_rv          sc_signal_rv
#define rc_fifo               sc_fifo

#define rc_sc_signal          sc_signal
#define rc_sc_buffer          sc_buffer
#define rc_sc_signal_resolved sc_signal_resolved
#define rc_sc_signal_rv       sc_signal_rv
#define rc_sc_fifo            sc_fifo

#define rc_port            sc_port
#define rc_in              sc_in
#define rc_out             sc_out
#define rc_inout           sc_inout
#define rc_fifo_in         sc_fifo_in
#define rc_fifo_out        sc_fifo_out
#define rc_in_resolved     sc_in_resolved
#define rc_out_resolved    sc_out_resolved
#define rc_inout_resolved  sc_inout_resolved
#define rc_in_rv           sc_in_rv
#define rc_out_rv          sc_out_rv
#define rc_inout_rv        sc_inout_rv

#define rc_export          sc_export

#define rc_join            sc_join
#define RC_FORK            SC_FORK
#define RC_JOIN            SC_JOIN

#define rc_event           sc_event

#define RC_HAS_VAR()               /*ignore*/
#define RC_VAR(type, name)         type name
#define rc_var(type, name)         type name
#define RC_DECLARE_VAR(type, name) /*ignore*/
#define rc_declare_var(type, name) /*ignore*/

#define RC_TRANSACTION             /*ignore*/

#define rc_spawn           sc_spawn
#define rc_spawn_options   sc_spawn_options
#define rc_bind            sc_bind
#define rc_ref             sc_ref
#define rc_cref            sc_cref
#define rc_get_current_process_handle sc_get_current_process
#define rc_process_handle  sc_process_handle
#define rc_wait            wait
#define rc_next_trigger    next_trigger

#define RC_REPORT_INFO     SC_REPORT_INFO
#define RC_REPORT_WARNING  SC_REPORT_WARNING
#define RC_REPORT_ERROR    SC_REPORT_ERROR
#define RC_REPORT_FATAL    SC_REPORT_FATAL
#define rc_assert          sc_assert

#define RC_STATIC_ASSERT(name_, assertion_)   /*ignore*/
#define RC_STATIC_ASSERT_T(name_, assertion_) /*ignore*/

#define RC_REPORT_DEBUG(msg)   /*ignore*/
#define RC_REPORT_DEBUG_1(msg) /*ignore*/
#define RC_REPORT_DEBUG_2(msg) /*ignore*/
#define RC_REPORT_DEBUG_3(msg) /*ignore*/
#define RC_REPORT_DEBUG_4(msg) /*ignore*/
#define RC_REPORT_DEBUG_5(msg) /*ignore*/

#define RC_COUT(msg)  /*ignore*/
#define RC_COUTL(msg) /*ignore*/

#endif // !defined(RC_MAKE_DOXYGEN_DOCS)

#endif // RC_SYNTHESIS_HEADER_H_

//
// $Id: synthesis_header.h,v 1.5 2007/10/14 12:42:29 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/synthesis_header.h,v $
//
