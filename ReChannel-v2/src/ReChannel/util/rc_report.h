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
 * @file    rc_report.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   ReChannel's error reporting and assertion functionality.
 *
 *
 * $Date: 2007/11/23 20:10:50 $
 * $Revision: 1.10 $
 */

#ifndef RC_REPORT_H_
#define RC_REPORT_H_

#include <cstring>
#include <sstream>

#include <systemc.h>

#if !defined(RC_DISABLE_COUT)
/**
 * @brief Macro for displaying text on std::cout.
 *
 * \remark This output macro may be disabled by defining
 * the compiler constant RC_DISABLE_COUT.
 * \see RC_DISABLE_COUT
 */
#define RC_COUT(msg) std::cout << msg; (void)0
/**
 * @brief Macro for displaying a line of text on std::cout.
 *
 * \remark This output macro may be disabled by defining
 * the compiler constant RC_DISABLE_COUT.
 * \see RC_DISABLE_COUT
 */
#define RC_COUTL(msg) std::cout << msg << std::endl; (void)0
#else
#define RC_COUT(msg) (void)0
#define RC_COUTL(msg) (void)0
#endif // !defined(RC_DISABLE_COUT)

/** @brief The prefix of ReChannel's report messages.
*/
#define RC_REPORT_MSG_TYPE "/ReChannel"

/** @brief Macro to report info messages.
*/
#define RC_REPORT_INFO(msg_type, msg) \
    if (true) { \
        std::stringstream sstream; \
        sstream << msg; \
        SC_REPORT_INFO(msg_type, sstream.str().c_str()); \
    } else (void)0

/** @brief Macro to report warnings.
*/
#define RC_REPORT_WARNING(msg_type, msg) \
    if (true) { \
        std::stringstream sstream; \
        sstream << msg; \
        SC_REPORT_WARNING(msg_type, sstream.str().c_str()); \
    } else (void)0

/** @brief Macro to report errors.
*/
#define RC_REPORT_ERROR(msg_type, msg) \
    if (true) { \
        std::stringstream sstream; \
        sstream << msg; \
        SC_REPORT_ERROR(msg_type, sstream.str().c_str()); throw 0; \
    } else (void)0

/** @brief Macro to report fatal errors.
*/
#define RC_REPORT_FATAL(msg_type, msg) \
    if (true) { \
        std::stringstream sstream; \
        sstream << msg; \
        sc_report_handler::force(SC_ABORT); \
        SC_REPORT_FATAL(msg_type, sstream.str().c_str()); throw 0; \
    } else (void)0

#ifdef RC_DEBUG
/** @brief ReChannel's assertion macro.
*/
 #define rc_assert(expr) \
    ((void)((expr) \
      ? 0 : (SC_REPORT_FATAL(RC_ID_ASSERTION_FAILED_, #expr), 0)))
#else
 #define rc_assert(expr) \
    (void)0
#endif

#ifndef RC_DEBUG_LEVEL
/**
 * @brief Sets the debug level (in the range of 0-5).
 * \remarks Default debug level is 3.
 */
 #define RC_DEBUG_LEVEL 3
#endif

#ifdef RC_DEBUG
/** @brief Reports a debug message (level >= 0). */
 #define RC_REPORT_DEBUG(msg) RC_REPORT_INFO(RC_ID_DEBUG_MSG_, msg)
#else
 #define RC_REPORT_DEBUG(msg) (void)0
#endif

#if defined(RC_DEBUG) && RC_DEBUG_LEVEL >= 1
/** @brief Reports a debug message (level >= 1). */
 #define RC_REPORT_DEBUG_1(msg) RC_REPORT_INFO(RC_ID_DEBUG_MSG_LVL_1_, msg)
#else
 #define RC_REPORT_DEBUG_1(msg) (void)0
#endif

#if defined(RC_DEBUG) && RC_DEBUG_LEVEL >= 2
/** @brief Reports a debug message (level >= 2). */
 #define RC_REPORT_DEBUG_2(msg) RC_REPORT_INFO(RC_ID_DEBUG_MSG_LVL_2_, msg)
#else
 #define RC_REPORT_DEBUG_2(msg) (void)0
#endif

#if defined(RC_DEBUG) && RC_DEBUG_LEVEL >= 3
/** @brief Reports a debug message (level >= 3). */
 #define RC_REPORT_DEBUG_3(msg) RC_REPORT_INFO(RC_ID_DEBUG_MSG_LVL_3_, msg)
#else
 #define RC_REPORT_DEBUG_3(msg) (void)0
#endif

#if defined(RC_DEBUG) && RC_DEBUG_LEVEL >= 4
/** @brief Reports a debug message (level >= 4). */
 #define RC_REPORT_DEBUG_4(msg) RC_REPORT_INFO(RC_ID_DEBUG_MSG_LVL_4_, msg)
#else
 #define RC_REPORT_DEBUG_4(msg) (void)0
#endif

#if defined(RC_DEBUG) && RC_DEBUG_LEVEL >= 5
/** @brief Reports a debug message (level >= 5). */
 #define RC_REPORT_DEBUG_5(msg) RC_REPORT_INFO(RC_ID_DEBUG_MSG_LVL_5_, msg)
#else
 #define RC_REPORT_DEBUG_5(msg) (void)0
#endif

namespace ReChannel {

// predefined message types
/** \brief Error message if an internal error occurs. */
extern const char RC_ID_INTERNAL_ERROR_[];
/** \brief Error message if an unknown error occurs. */
extern const char RC_ID_UNKNOWN_ERROR_[];
/** \brief Error message if a specific feature is not implemented. */
extern const char RC_ID_NOT_IMPLEMENTED_[];
/** \brief Error message if index is out of bounds. */
extern const char RC_ID_OUT_OF_BOUNDS_[];
/** \brief Error message if a null pointer is given. */
extern const char RC_ID_NULL_POINTER_[];
/** \brief Error message if a class/function is not used as intended. */
extern const char RC_ID_INVALID_USAGE_[];
/** \brief Error message if a entry exists twice. */
extern const char RC_ID_DUPLICATE_ENTRY_[];
/** \brief Error message if a runtime assertion failed. */
extern const char RC_ID_ASSERTION_FAILED_[];

/** \brief Debug message ID (used by RC_REPORT_DEBUG()). */
extern const char RC_ID_DEBUG_MSG_[];
/** \brief Debug message ID (used by RC_REPORT_DEBUG_1()). */
extern const char RC_ID_DEBUG_MSG_LVL_1_[];
/** \brief Debug message ID (used by RC_REPORT_DEBUG_2()). */
extern const char RC_ID_DEBUG_MSG_LVL_2_[];
/** \brief Debug message ID (used by RC_REPORT_DEBUG_3()). */
extern const char RC_ID_DEBUG_MSG_LVL_3_[];
/** \brief Debug message ID (used by RC_REPORT_DEBUG_4()). */
extern const char RC_ID_DEBUG_MSG_LVL_4_[];
/** \brief Debug message ID (used by RC_REPORT_DEBUG_5()). */
extern const char RC_ID_DEBUG_MSG_LVL_5_[];

/** \brief Class rc_static_assertion is used to check static assertions
 * at compile time.
 */
template<bool Assertion>
class rc_static_assertion;

/** \brief Class rc_static_assertion<true> represents a true assertion
 * condition.
 * \see rc_static_assertion<bool>
 */
template<>
class rc_static_assertion<true>
{ public: typedef bool fulfilled; };

/** \brief Class rc_static_assertion<false> represents a false assertion
 * condition.
 * \see rc_static_assertion<bool>
 */
template<>
class rc_static_assertion<false>
{ public: typedef bool failed; };

} // namespace ReChannel

/** \brief Macro to state a static assertion.
 * \see RC_STATIC_ASSERT_T
 * \relates rc_static_assertion
 */
#define RC_STATIC_ASSERT(name_, assertion_) \
    typedef ::ReChannel::rc_static_assertion< \
            (bool)(assertion_) \
        >::fulfilled RC_STATIC_ASSERT_##name_/*;*/

/** \brief Macro to state a static assertion within a template class.
 * \see RC_STATIC_ASSERT
 * \relates rc_static_assertion
 */
#define RC_STATIC_ASSERT_T(name_, assertion_) \
    typedef typename ::ReChannel::template rc_static_assertion< \
            (bool)(assertion_) \
        >::fulfilled RC_STATIC_ASSERT_##name_/*;*/

#endif // RC_REPORT_H_

//
// $Id: rc_report.h,v 1.10 2007/11/23 20:10:50 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_report.h,v $
//
