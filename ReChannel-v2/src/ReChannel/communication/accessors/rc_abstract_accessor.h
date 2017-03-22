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
 * @file    rc_abstract_accessor.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Definition of class rc_abstract_accessor.
 *
 *
 * $Date: 2008/01/01 13:47:20 $
 * $Revision: 1.9 $
 */

#ifndef RC_ABSTRACT_ACCESSOR_H_
#define RC_ABSTRACT_ACCESSOR_H_

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_process.h"
#include "ReChannel/util/rc_hash_map.h"
#include "ReChannel/communication/rc_report_ids.h"
#include "ReChannel/communication/rc_fallback_interface.h"
#include "ReChannel/communication/rc_interface_wrapper.h"

namespace ReChannel {

template<class IF> class rc_abstract_accessor;

/**
 * @brief Base class of an abstract accessor.
 */
class rc_abstract_accessor_b
    : virtual public rc_interface_wrapper_base::accessor_base
{
    template<class IF> friend class rc_abstract_accessor;

private:
    typedef std::vector<sc_port_base*> port_vector;
    typedef std::map<sc_object*, int>  process_index_map;

private:
    rc_abstract_accessor_b();

public:

    virtual void register_port(
        sc_port_base& port_, const char* if_typename_);

    inline virtual rc_event_filter* rc_get_event_filter()
        { return NULL; }

    inline virtual int rc_get_bound_port_count() const
        { return p_bound_port_vector.size(); }

    inline virtual sc_port_base* rc_get_bound_port(int index) const
        { return p_bound_port_vector[index]; }

protected:

    virtual bool rc_on_event(const sc_event& e)
        { return true; }

    virtual void rc_set_event_trigger(
        const sc_event& e, rc_event_trigger& t) { }

    virtual void rc_clear_event_trigger(const sc_event& e) { }

    int rc_get_driver_process_index(sc_object* proc) const;

    int rc_get_nb_driver_process_index(sc_object* proc) const;

    inline int rc_get_driver_process_count() const
        { return p_process_index_map.size(); }

    inline int rc_get_nb_driver_process_count() const
        { return p_nb_process_index_map.size(); }

/* next_trigger(..) redirects */

    inline void next_trigger()
        { rc_next_trigger(); }

    inline void next_trigger(const sc_event& e)
        { rc_next_trigger(e); }

    inline void next_trigger(sc_event_or_list& el)
        { rc_next_trigger(el); }

    inline void next_trigger(sc_event_and_list& el)
        { rc_next_trigger(el); }

    inline void next_trigger(const sc_time& t)
        { rc_next_trigger(t); }

    inline void next_trigger(double v, sc_time_unit tu)
        { rc_next_trigger(v, tu); }

    inline void next_trigger(const sc_time& t, const sc_event& e)
        { rc_next_trigger(t, e); }

    inline void next_trigger(double v, sc_time_unit tu, const sc_event& e)
        { rc_next_trigger(v, tu, e); }

    inline void next_trigger(const sc_time& t, sc_event_or_list& el)
        { rc_next_trigger(t, el); }

    inline void next_trigger(
        double v, sc_time_unit t, sc_event_or_list& el)
        { rc_next_trigger(v, t, el); }

    inline void next_trigger(const sc_time& t, sc_event_and_list& el)
        { rc_next_trigger(t, el); }

    inline void next_trigger(
        double v, sc_time_unit tu, sc_event_and_list& el)
        { rc_next_trigger(v, tu, el); }

/* rc_wait() redirects */

    inline void wait()
        { rc_wait(); }

    inline void wait(int n)
        { rc_wait(n); }

    inline void wait(const sc_event& e)
        { rc_wait(e); }

    inline void wait(sc_event_or_list& el)
        { rc_wait(el); }

    inline void wait(sc_event_and_list& el)
        { rc_wait(el); }

    inline void wait(const sc_time& t)
        { rc_wait(t); }

    inline void wait(double v, sc_time_unit tu)
        { rc_wait(v, tu); }

    inline void wait(const sc_time& t, const sc_event& e)
        { rc_wait(t, e); }

    inline void wait(double v, sc_time_unit tu, const sc_event& e)
        { rc_wait(v, tu, e); }

    inline void wait(const sc_time& t, sc_event_or_list& el)
        { rc_wait(t, el); }

    inline void wait(double v, sc_time_unit t, sc_event_or_list& el)
        { rc_wait(v, t, el); }

    inline void wait(const sc_time& t, sc_event_and_list& el)
        { rc_wait(t, el); }

    inline void wait(double v, sc_time_unit tu, sc_event_and_list& el)
        { rc_wait(v, tu, el); }

private:
    int rc_register_driver_process(sc_object* proc);

    int rc_register_nb_driver_process(sc_object* proc);

private:
    port_vector       p_bound_port_vector;
    process_index_map p_process_index_map;
    process_index_map p_nb_process_index_map;

private:
    // disabled
    rc_abstract_accessor_b(const rc_abstract_accessor_b& accessor);
    rc_abstract_accessor_b& operator=(const rc_abstract_accessor_b& accessor);
};

/* interal helper macros for repetitive code generation */

#define _RECHANNEL_ACCESSOR_NB_FW_(if_method_args, wrapper_call) \
    try { \
        if (p_target_if != NULL) { \
            return ((p_target_if->*method)if_method_args); \
        } else if (p_target_wrapper != NULL) { \
            return (wrapper_call); \
        } \
    } catch(rc_process_cancel_exception* e) { \
        rc_process_handle hproc = rc_get_current_process_handle(); \
        if (hproc.is_canceled()) { \
            ::ReChannel::rc_throw(e); \
        } else { \
            delete e; /* consume cancel exception */ \
        } \
    } \
    return ((rc_get_fallback_if().*method)if_method_args);

#define _RECHANNEL_ACCESSOR_FW_(if_method_args, wrapper_call) \
    rc_process_handle hproc = rc_get_current_process_handle(); \
    while(true) { \
        try { \
            if (p_target_if != NULL) { \
                rc_process_behavior_change temp = \
                    this->_rc_process_behavior_change(hproc); \
                return ((p_target_if->*method)if_method_args); \
            } else if (p_target_wrapper != NULL) { \
                return (wrapper_call); \
            } \
        } catch(rc_process_cancel_exception* e) { \
            if (hproc.is_canceled()) { \
                ::ReChannel::rc_throw(e); \
            } else { \
                delete e; /* consume cancel exception */ \
            } \
        } \
        this->_rc_wait_activation(); \
    }

#define _RECHANNEL_ACCESSOR_NB_FW(if_method_args) \
    _RECHANNEL_ACCESSOR_NB_FW_( \
        if_method_args, \
        ((p_target_wrapper->get_nb_interface_access()->*method) \
            if_method_args))

#define _RECHANNEL_ACCESSOR_FW(if_method_args) \
    _RECHANNEL_ACCESSOR_FW_( \
        if_method_args, \
        ((p_target_wrapper->get_interface_access()->*method) \
            if_method_args))

#define _RECHANNEL_ACCESSOR_NB_FW_DRV(if_method_args, drv_call_args) \
    _RECHANNEL_ACCESSOR_NB_FW_( \
        if_method_args, \
        (p_target_wrapper->get_nb_driver_access( \
            _rc_nb_driver_process_index(sc_get_current_process_handle())) \
                ->call drv_call_args))

#define _RECHANNEL_ACCESSOR_FW_DRV(if_method_args, drv_call_args) \
    _RECHANNEL_ACCESSOR_FW_( \
        if_method_args, \
        (p_target_wrapper->get_driver_access( \
            _rc_driver_process_index(hproc))->call drv_call_args))

/**
 * @brief Abstract base class of an accessor.
 */
template<class IF>
class rc_abstract_accessor
    : public rc_abstract_accessor_b,
      virtual public rc_interface_wrapper<IF>::accessor
{
    RC_STATIC_ASSERT_VALID_INTERFACE(IF);

private:
    typedef typename
        rc_interface_wrapper<IF>::accessor::interface_access
            interface_access;
    typedef typename
        rc_interface_wrapper<IF>::accessor::nb_interface_access
            nb_interface_access;
    typedef typename rc_interface_wrapper<IF>::accessor::driver_access
        driver_access;
    typedef typename rc_interface_wrapper<IF>::accessor::nb_driver_access
        nb_driver_access;

    typedef rc_abstract_accessor<IF> this_type;
    typedef rc_abstract_accessor_b   base_type;

public:
    typedef IF if_type;

protected:
    explicit rc_abstract_accessor(
        rc_interface_wrapper<IF>* target_wrapper=NULL);

public:
    virtual
        rc_interface_wrapper_base* rc_get_target_wrapper_base() const
        { return p_target_wrapper; }

    virtual rc_interface_wrapper<IF>* rc_get_target_wrapper() const
        { return p_target_wrapper; }

    virtual bool rc_set_target(rc_interface_wrapper_base& target);

    virtual bool rc_set_target(rc_interface_wrapper<IF>& target);

    virtual sc_interface* rc_get_target_interface() const
        { return p_target_if; }

    virtual bool rc_set_target(sc_interface& target);

    virtual bool rc_set_target(
        sc_interface& target, const rc_process_control& pctrl);

    virtual bool rc_set_target(IF& target);

    virtual bool rc_set_target(
        IF& target, const rc_process_control& pctrl);

    virtual void rc_clear_target();

    virtual void rc_on_target_changed() { }

    inline bool rc_has_target_wrapper() const
        { return (p_target_wrapper != NULL); }

    inline bool rc_has_target_interface() const
        { return (p_target_if != NULL); }

protected:
    virtual IF& rc_get_fallback_if() const;

/* non-blocking forwarder */

    template<class R, class IF_>
    inline R rc_nb_forward(R (IF_::*method)()) const
        { _RECHANNEL_ACCESSOR_NB_FW(()); }

    template<class R, class IF_>
    inline R rc_nb_forward(R (IF_::*method)() const) const
        { _RECHANNEL_ACCESSOR_NB_FW(()); }

    template<class R, class A1, class A1_, class IF_>
    inline R rc_nb_forward(R (IF_::*method)(A1_), A1 a1) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1)); }

    template<class R, class A1, class A1_, class IF_>
    inline R rc_nb_forward(R (IF_::*method)(A1_) const, A1 a1) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1)); }

    template<class R, class A1, class A2, class A1_, class A2_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_), A1 a1, A2 a2) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2)); }

    template<class R, class A1, class A2, class A1_, class A2_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_) const, A1 a1, A2 a2) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2)); }

    template<class R, class A1, class A2, class A3,
        class A1_, class A2_, class A3_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_), A1 a1, A2 a2, A3 a3) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3)); }

    template<class R, class A1, class A2, class A3,
        class A1_, class A2_, class A3_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_) const,
        A1 a1, A2 a2, A3 a3) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3)); }

    template<class R, class A1, class A2, class A3, class A4,
        class A1_, class A2_, class A3_, class A4_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_),
        A1 a1, A2 a2, A3 a3, A4 a4) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3, a4)); }

    template<class R, class A1, class A2, class A3, class A4,
        class A1_, class A2_, class A3_, class A4_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_) const,
        A1 a1, A2 a2, A3 a3, A4 a4) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3, a4)); }

    template<class R, class A1, class A2, class A3, class A4, class A5,
        class A1_, class A2_, class A3_, class A4_, class A5_,
        class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3, a4, a5)); }

    template<class R, class A1, class A2, class A3, class A4, class A5,
        class A1_, class A2_, class A3_, class A4_, class A5_,
        class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3, a4, a5)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3, a4, a5, a6)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3, a4, a5, a6)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3, a4, a5, a6, a7)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3, a4, a5, a6, a7)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3, a4, a5, a6, a7, a8)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const
        { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3, a4, a5, a6, a7, a8)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9)const
    { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3, a4, a5, a6, a7, a8, a9)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5,
        A6 a6, A7 a7, A8 a8, A9 a9) const
    { _RECHANNEL_ACCESSOR_NB_FW((a1, a2, a3, a4, a5, a6, a7, a8, a9)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9, class A10,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class A10_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_, A10_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9, A10 a10) const
    {
        _RECHANNEL_ACCESSOR_NB_FW(
            (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10));
    }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9, class A10,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class A10_, class IF_>
    inline R rc_nb_forward(
        R (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_, A10_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9, A10 a10) const
    {
        _RECHANNEL_ACCESSOR_NB_FW(
            (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10));
    }

/* blocking forwarder */

    template<class R, class IF_>
    inline R rc_forward(R (IF_::*method)()) const
        { _RECHANNEL_ACCESSOR_FW(()); }

    template<class R, class IF_>
    inline R rc_forward(R (IF_::*method)() const) const
        { _RECHANNEL_ACCESSOR_FW(()); }

    template<class R, class A1, class A1_, class IF_>
    inline R rc_forward(R (IF_::*method)(A1_), A1 a1) const
        { _RECHANNEL_ACCESSOR_FW((a1)); }

    template<class R, class A1, class A1_, class IF_>
    inline R rc_forward(R (IF_::*method)(A1_) const, A1 a1) const
        { _RECHANNEL_ACCESSOR_FW((a1)); }

    template<class R, class A1, class A2, class A1_, class A2_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_), A1 a1, A2 a2) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2)); }

    template<class R, class A1, class A2, class A1_, class A2_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_) const, A1 a1, A2 a2) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2)); }

    template<class R, class A1, class A2, class A3,
        class A1_, class A2_, class A3_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_), A1 a1, A2 a2, A3 a3) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3)); }

    template<class R, class A1, class A2, class A3,
        class A1_, class A2_, class A3_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_) const,
        A1 a1, A2 a2, A3 a3) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3)); }

    template<class R, class A1, class A2, class A3, class A4,
        class A1_, class A2_, class A3_, class A4_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_),
        A1 a1, A2 a2, A3 a3, A4 a4) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4)); }

    template<class R, class A1, class A2, class A3, class A4,
        class A1_, class A2_, class A3_, class A4_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_) const,
        A1 a1, A2 a2, A3 a3, A4 a4) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4)); }

    template<class R, class A1, class A2, class A3, class A4, class A5,
        class A1_, class A2_, class A3_, class A4_, class A5_,
        class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4, a5)); }

    template<class R, class A1, class A2, class A3, class A4, class A5,
        class A1_, class A2_, class A3_, class A4_, class A5_,
        class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4, a5)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4, a5, a6)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4, a5, a6)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4, a5, a6, a7)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4, a5, a6, a7)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4, a5, a6, a7, a8)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4, a5, a6, a7, a8)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4, a5, a6, a7, a8, a9)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9) const
        { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4, a5, a6, a7, a8, a9)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9, class A10,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class A10_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_, A10_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9, A10 a10) const
    { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9, class A10,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class A10_, class IF_>
    inline R rc_forward(
        R (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_, A10_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9, A10 a10) const
    { _RECHANNEL_ACCESSOR_FW((a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)); }

/* non-blocking driver forwarder (without return value) */

    template<class IF_>
    inline void rc_nb_forward_driver(void (IF_::*method)())
        { _RECHANNEL_ACCESSOR_NB_FW_DRV((), (method)); }

    template<class IF_>
    inline void rc_nb_forward_driver(void (IF_::*method)() const)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV((), (method)); }

    template<class A1, class A1_, class IF_>
    inline void rc_nb_forward_driver(void (IF_::*method)(A1_), A1 a1)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV((a1), (method, a1)); }

    template<class A1, class A1_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_) const, A1 a1)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV((a1), (method, a1)); }

    template<class A1, class A2, class A1_, class A2_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_), A1 a1, A2 a2)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV((a1, a2), (method, a1, a2)); }

    template<class A1, class A2, class A1_, class A2_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_) const, A1 a1, A2 a2)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV((a1, a2), (method, a1, a2)); }

    template<class A1, class A2, class A3,
        class A1_, class A2_, class A3_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_), A1 a1, A2 a2, A3 a3)
    { _RECHANNEL_ACCESSOR_NB_FW_DRV((a1, a2, a3), (method, a1, a2, a3)); }

    template<class A1, class A2, class A3,
        class A1_, class A2_, class A3_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_) const, A1 a1, A2 a2, A3 a3)
    { _RECHANNEL_ACCESSOR_NB_FW_DRV((a1, a2, a3), (method, a1, a2, a3)); }

    template<class A1, class A2, class A3, class A4,
        class A1_, class A2_, class A3_, class A4_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_, A4_),
        A1 a1, A2 a2, A3 a3, A4 a4)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4), (method, a1, a2, a3, a4)); }

    template<class A1, class A2, class A3, class A4,
        class A1_, class A2_, class A3_, class A4_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_, A4_) const,
        A1 a1, A2 a2, A3 a3, A4 a4)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4), (method, a1, a2, a3, a4)); }

    template<class A1, class A2, class A3, class A4, class A5,
        class A1_, class A2_, class A3_, class A4_, class A5_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4, a5), (method, a1, a2, a3, a4, a5)); }

    template<class A1, class A2, class A3, class A4, class A5,
        class A1_, class A2_, class A3_, class A4_, class A5_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4, a5), (method, a1, a2, a3, a4, a5)); }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4, a5, a6), (method, a1, a2, a3, a4, a5, a6)); }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4, a5, a6), (method, a1, a2, a3, a4, a5, a6)); }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7),
            (method, a1, a2, a3, a4, a5, a6, a7)); }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7),
            (method, a1, a2, a3, a4, a5, a6, a7)); }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7, a8),
            (method, a1, a2, a3, a4, a5, a6, a7, a8)); }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7, a8),
            (method, a1, a2, a3, a4, a5, a6, a7, a8)); }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7, a8, a9),
            (method, a1, a2, a3, a4, a5, a6, a7, a8, a9)); }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7, a8, a9),
            (method, a1, a2, a3, a4, a5, a6, a7, a8, a9)); }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9, class A10,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class A10_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_, A10_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9, A10 a10)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10),
            (method, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)); }

    template<class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9, class A10,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class A10_, class IF_>
    inline void rc_nb_forward_driver(
        void (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_, A10_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9, A10 a10)
        { _RECHANNEL_ACCESSOR_NB_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10),
            (method, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)); }

/* blocking driver forwarder */

    template<class R, class IF_>
    inline R rc_forward_driver(R (IF_::*method)())
        { _RECHANNEL_ACCESSOR_FW_DRV((), (method)); }

    template<class R, class IF_>
    inline R rc_forward_driver(R (IF_::*method)() const)
        { _RECHANNEL_ACCESSOR_FW_DRV((), (method)); }

    template<class R, class A1, class A1_, class IF_>
    inline R rc_forward_driver(R (IF_::*method)(A1_), A1 a1)
        { _RECHANNEL_ACCESSOR_FW_DRV((a1), (method, a1)); }

    template<class R, class A1, class A1_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_) const, A1 a1)
        { _RECHANNEL_ACCESSOR_FW_DRV((a1), (method, a1)); }

    template<class R, class A1, class A2, class A1_, class A2_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_), A1 a1, A2 a2)
        { _RECHANNEL_ACCESSOR_FW_DRV((a1, a2), (method, a1, a2)); }

    template<class R, class A1, class A2, class A1_, class A2_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_) const, A1 a1, A2 a2)
        { _RECHANNEL_ACCESSOR_FW_DRV((a1, a2), (method, a1, a2)); }

    template<class R, class A1, class A2, class A3,
        class A1_, class A2_, class A3_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_), A1 a1, A2 a2, A3 a3)
        { _RECHANNEL_ACCESSOR_FW_DRV((a1, a2, a3), (method, a1, a2, a3)); }

    template<class R, class A1, class A2, class A3,
        class A1_, class A2_, class A3_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_) const, A1 a1, A2 a2, A3 a3)
        { _RECHANNEL_ACCESSOR_FW_DRV((a1, a2, a3), (method, a1, a2, a3)); }

    template<class R, class A1, class A2, class A3, class A4,
        class A1_, class A2_, class A3_, class A4_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_, A4_),
        A1 a1, A2 a2, A3 a3, A4 a4)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4), (method, a1, a2, a3, a4)); }

    template<class R, class A1, class A2, class A3, class A4,
        class A1_, class A2_, class A3_, class A4_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_, A4_) const,
        A1 a1, A2 a2, A3 a3, A4 a4)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4), (method, a1, a2, a3, a4)); }

    template<class R, class A1, class A2, class A3, class A4, class A5,
        class A1_, class A2_, class A3_, class A4_, class A5_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4, a5), (method, a1, a2, a3, a4, a5)); }

    template<class R, class A1, class A2, class A3, class A4, class A5,
        class A1_, class A2_, class A3_, class A4_, class A5_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4, a5), (method, a1, a2, a3, a4, a5)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4, a5, a6), (method, a1, a2, a3, a4, a5, a6)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4, a5, a6), (method, a1, a2, a3, a4, a5, a6)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7),
            (method, a1, a2, a3, a4, a5, a6, a7)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7),
            (method, a1, a2, a3, a4, a5, a6, a7)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7, a8),
            (method, a1, a2, a3, a4, a5, a6, a7, a8)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7, a8),
            (method, a1, a2, a3, a4, a5, a6, a7, a8)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7, a8, a9),
            (method, a1, a2, a3, a4, a5, a6, a7, a8, a9)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7, a8, a9),
            (method, a1, a2, a3, a4, a5, a6, a7, a8, a9)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9, class A10,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class A10_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_, A10_),
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9, A10 a10)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10),
            (method, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)); }

    template<class R,
        class A1, class A2, class A3, class A4, class A5, class A6,
        class A7, class A8, class A9, class A10,
        class A1_, class A2_, class A3_, class A4_, class A5_, class A6_,
        class A7_, class A8_, class A9_, class A10_, class IF_>
    inline R rc_forward_driver(
        R (IF_::*method)(
            A1_, A2_, A3_, A4_, A5_, A6_, A7_, A8_, A9_, A10_) const,
        A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6,
        A7 a7, A8 a8, A9 a9, A10 a10)
        { _RECHANNEL_ACCESSOR_FW_DRV(
            (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10),
            (method, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)); }

private:

/* for internal use only */

    rc_process_behavior_change _rc_process_behavior_change(
        rc_process_handle& hproc) const;

    void _rc_wait_activation() const;

    inline int _rc_driver_process_index(sc_process_handle& hproc);

    inline int _rc_nb_driver_process_index(sc_process_handle hproc);

private:
    rc_interface_wrapper<IF>* p_target_wrapper;
    IF*                       p_target_if;
    sc_event                  p_target_set_event;
    const rc_process_control* p_pctrl;

private:
    // disabled
    rc_abstract_accessor(const rc_abstract_accessor& accessor);
    rc_abstract_accessor& operator=(const rc_abstract_accessor& accessor);
};

#undef _RECHANNEL_ACCESSOR_NB_FW_
#undef _RECHANNEL_ACCESSOR_FW_
#undef _RECHANNEL_ACCESSOR_NB_FW
#undef _RECHANNEL_ACCESSOR_FW
#undef _RECHANNEL_ACCESSOR_NB_FW_DRV
#undef _RECHANNEL_ACCESSOR_FW_DRV

/* template code */

template<class IF>
rc_abstract_accessor<IF>::rc_abstract_accessor(
    rc_interface_wrapper<IF>* target_wrapper)
    : p_target_wrapper(target_wrapper), p_target_if(NULL), p_pctrl(NULL)
{ }

template<class IF>
bool rc_abstract_accessor<IF>::rc_set_target(
    rc_interface_wrapper<IF>& target)
{
    this->rc_clear_target();
    p_target_wrapper = &target;
    rc_notify(p_target_set_event);
    return true;
}

template<class IF>
bool rc_abstract_accessor<IF>::rc_set_target(
    rc_interface_wrapper_base& target)
{
    rc_interface_wrapper<IF>* target_ =
        dynamic_cast<rc_interface_wrapper<IF>*>(&target);
    if (target_ != NULL) {
        return this->rc_set_target(*target_);
    } else {
        return false;
    }
}

template<class IF>
bool rc_abstract_accessor<IF>::rc_set_target(IF& target)
{
    this->rc_clear_target();
    p_target_if = &target;
    rc_notify(p_target_set_event);
    return true;
}

template<class IF>
bool rc_abstract_accessor<IF>::rc_set_target(
    IF& target, const rc_process_control& pctrl)
{
    this->rc_clear_target();
    p_target_if = &target;
    p_pctrl = &pctrl;
    rc_notify(p_target_set_event);
    return true;
}

template<class IF>
bool rc_abstract_accessor<IF>::rc_set_target(sc_interface& target)
{
    IF* target_ = dynamic_cast<IF*>(&target);
    if (target_ != NULL) {
        return this->rc_set_target(*target_);
    } else {
        return false;
    }
}

template<class IF>
bool rc_abstract_accessor<IF>::rc_set_target(
    sc_interface& target, const rc_process_control& pctrl)
{
    IF* target_ = dynamic_cast<IF*>(&target);
    if (target_ != NULL) {
        return this->rc_set_target(*target_, pctrl);
    } else {
        return false;
    }
}

template<class IF>
void rc_abstract_accessor<IF>::rc_clear_target()
{
    p_target_if = NULL;
    p_target_wrapper = NULL;
    p_pctrl = NULL;
    this->rc_on_target_changed();
}

template<class IF>
IF& rc_abstract_accessor<IF>::rc_get_fallback_if() const
{
    static IF* s_fallback_if =
        rc_fallback_interface_factory<IF>::create();
    if (s_fallback_if != NULL) {
        return *s_fallback_if;
    } else {
        RC_REPORT_ERROR(RC_ID_FALLBACK_INTERFACE_MISSING_,
            "a default fallback interface doesn't exist for this accessor");
    }
}

template<class IF>
rc_process_behavior_change
rc_abstract_accessor<IF>::_rc_process_behavior_change(
    rc_process_handle& hproc) const
{
    if (p_pctrl != NULL) {
        return hproc.behavior_change(*p_pctrl);
    } else {
        return rc_process_behavior_change();
    }
}

template<class IF>
void rc_abstract_accessor<IF>::_rc_wait_activation() const
{
    if (p_pctrl == NULL) {
        rc_wait(p_target_set_event);
    } else {
        rc_wait(p_pctrl->get_activation_event());
    }
}

template<class IF>
inline
int rc_abstract_accessor<IF>::_rc_driver_process_index(
    sc_process_handle& hproc)
{
    sc_object* proc = hproc.get_process_object();
    int index = rc_get_driver_process_index(proc);
    return (index == -1 ? rc_register_driver_process(proc) : index);
}

template<class IF>
inline
int rc_abstract_accessor<IF>::_rc_nb_driver_process_index(
    sc_process_handle hproc)
{
    sc_object* proc = hproc.get_process_object();
    int index = rc_get_nb_driver_process_index(proc);
    return (index == -1 ? rc_register_nb_driver_process(proc) : index);
}

} // namespace ReChannel

#endif // RC_ABSTRACT_ACCESSOR_H_

//
// $Id: rc_abstract_accessor.h,v 1.9 2008/01/01 13:47:20 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/communication/accessors/rc_abstract_accessor.h,v $
//
