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
 * @file    rc_portmap_macros.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/10/09 00:22:26 $
 * $Revision: 1.2 $
 */

#ifndef RC_PORTMAP_MACROS_H_
#define RC_PORTMAP_MACROS_H_

#define _RC_PM_PARAMS_1(p1) \
    p1
#define _RC_PM_PARAMS_2(p1, p2) \
    p1, p2
#define _RC_PM_PARAMS_3(p1, p2, p3) \
    p1, p2, p3
#define _RC_PM_PARAMS_4(p1, p2, p3, p4) \
    p1, p2, p3, p4
#define _RC_PM_PARAMS_5(p1, p2, p3, p4, p5) \
    p1, p2, p3, p4, p5
#define _RC_PM_PARAMS_6(p1, p2, p3, p4, p5, p6) \
    p1, p2, p3, p4, p5, p6
#define _RC_PM_PARAMS_7(p1, p2, p3, p4, p5, p6, p7) \
    p1, p2, p3, p4, p5, p6, p7
#define _RC_PM_PARAMS_8(p1, p2, p3, p4, p5, p6, p7, p8) \
    p1, p2, p3, p4, p5, p6, p7, p8
#define _RC_PM_PARAMS_9(p1, p2, p3, p4, p5, p6, p7, p8, p9) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9
#define _RC_PM_PARAMS_10(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10
#define _RC_PM_PARAMS_11(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11
#define _RC_PM_PARAMS_12( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12
#define _RC_PM_PARAMS_13( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13
#define _RC_PM_PARAMS_14( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14
#define _RC_PM_PARAMS_15( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15
#define _RC_PM_PARAMS_16( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16
#define _RC_PM_PARAMS_17( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17
#define _RC_PM_PARAMS_18( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18
#define _RC_PM_PARAMS_19( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19
#define _RC_PM_PARAMS_20( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20
#define _RC_PM_PARAMS_21( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21
#define _RC_PM_PARAMS_22( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22
#define _RC_PM_PARAMS_23( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23
#define _RC_PM_PARAMS_24( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24
#define _RC_PM_PARAMS_25( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25
#define _RC_PM_PARAMS_26( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26
#define _RC_PM_PARAMS_27( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27
#define _RC_PM_PARAMS_28( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28
#define _RC_PM_PARAMS_29( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29
#define _RC_PM_PARAMS_30( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30
#define _RC_PM_PARAMS_31( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31
#define _RC_PM_PARAMS_32( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32
#define _RC_PM_PARAMS_33( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33
#define _RC_PM_PARAMS_34( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34
#define _RC_PM_PARAMS_35( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35
#define _RC_PM_PARAMS_36( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36
#define _RC_PM_PARAMS_37( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37
#define _RC_PM_PARAMS_38( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38
#define _RC_PM_PARAMS_39( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39
#define _RC_PM_PARAMS_40( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40
#define _RC_PM_PARAMS_41( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41
#define _RC_PM_PARAMS_42( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42
#define _RC_PM_PARAMS_43( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43
#define _RC_PM_PARAMS_44( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44
#define _RC_PM_PARAMS_45( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45
#define _RC_PM_PARAMS_46( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46
#define _RC_PM_PARAMS_47( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47
#define _RC_PM_PARAMS_48( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48
#define _RC_PM_PARAMS_49( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49
#define _RC_PM_PARAMS_50( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50
#define _RC_PM_PARAMS_51( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51
#define _RC_PM_PARAMS_52( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52
#define _RC_PM_PARAMS_53( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53
#define _RC_PM_PARAMS_54( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54
#define _RC_PM_PARAMS_55( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55
#define _RC_PM_PARAMS_56( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56
#define _RC_PM_PARAMS_57( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57
#define _RC_PM_PARAMS_58( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57, p58) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57, p58
#define _RC_PM_PARAMS_59( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57, p58, p59) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57, p58, p59
#define _RC_PM_PARAMS_60( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57, p58, p59, p60) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57, p58, p59, p60
#define _RC_PM_PARAMS_61( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57, p58, p59, p60, p61) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57, p58, p59, p60, p61
#define _RC_PM_PARAMS_62( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57, p58, p59, p60, p61, p62) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57, p58, p59, p60, p61, p62
#define _RC_PM_PARAMS_63( \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57, p58, p59, p60, p61, p62, p63) \
    p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, \
    p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, \
    p29, p30, p31, p32, p33, p34, p35, p36, p37, p38, p39, p40, p41, \
    p42, p43, p44, p45, p46, p47, p48, p49, p50, p51, p52, p53, p54, \
    p55, p56, p57, p58, p59, p60, p61, p62, p63

#endif //RC_PORTMAP_MACROS_H_

//
// $Id: rc_portmap_macros.h,v 1.2 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_portmap_macros.h,v $
//
