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
 * @file    rc_object_handle.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   Generalised handling of SystemC objects.
 *
 * Class rc_object_handle is helpful when no template arguments are known,
 * e.g., in base classes, and SystemC objects of different type have to be
 * managed. If the object is a communication object
 * (i.e. port/export or channel) generalised access to the respective interface
 * is provided.
 *
 * $Date: 2007/10/09 00:22:26 $
 * $Revision: 1.3 $
 */

#ifndef RC_OBJECT_HANDLE_H_
#define RC_OBJECT_HANDLE_H_

#include "ReChannel/util/rc_port_handle.h"
#include "ReChannel/util/rc_export_handle.h"

namespace ReChannel {

 /** @brief Class for a generalised handling of SystemC objects.
 *
 * rc_object_handle is helpful when no template arguments are known,
 * e.g., in base classes, and SystemC objects of different type have to be
 * managed. If the object is a communication object
 * (i.e. port/export or channel) generalised access to the respective interface
 * is provided.
 *
 * \note rc_object_handle objects are copyable.
 */
class rc_object_handle // (copyable)
{
public:
    /** @brief default constructor (creates an invalid handle) */
    rc_object_handle();

    /** @brief constructor (creates handle for a pure object) */
    rc_object_handle(sc_object& obj_);

    /** @brief constructor (creates handle from a port handle) */
    rc_object_handle(const rc_port_handle& port_);

    /** @brief constructor (creates handle for a port) */
    template<class IF, int N, sc_port_policy POL>
    rc_object_handle(sc_port<IF, N, POL>& port_);

    /** @brief constructor (creates handle from an export handle) */
    rc_object_handle(const rc_export_handle& export_);

    /** @brief constructor (creates handle for an export) */
    template<class IF>
    rc_object_handle(sc_export<IF>& export_);

public:
    rc_object_handle& operator=(sc_object& obj_);

    template<class IF, int N, sc_port_policy POL>
    rc_object_handle& operator=(sc_port<IF, N, POL>& port_);

    rc_object_handle& operator=(const rc_port_handle& port_);

    template<class IF>
    rc_object_handle& operator=(sc_export<IF>& export_);

    rc_object_handle& operator=(const rc_export_handle& export_);

    /** @brief is this handle valid? */
    inline bool valid() const
        { return (p_object != NULL); }

    /** @brief does this handle reference a port? */
    inline bool is_port() const
        { return p_port.valid(); }

    /** @brief does this handle reference an export? */
    inline bool is_export() const
        { return p_export.valid(); }

    /** @brief does this handle reference a channel? */
    inline bool is_channel() const
        { return p_channel_if != NULL; }

    /** @brief does this handle reference a communcation object? */
    inline bool is_comm_object() const
        { return (is_channel() || is_port() || is_export()); }

    /** @brief returns a port handle */
    inline rc_port_handle get_port() const
        { return p_port; }

    /** @brief returns an export handle */
    inline rc_export_handle get_export() const
        { return p_export; }

    /** @brief get the interface of a communication object */
    sc_interface* get_interface() const;

    /** @brief get the object of the referenced object */
    inline sc_object* get_object() const
        { return p_object; }

    /** @brief direct access to the referenced object */
    sc_object* operator->() const
        { return p_object; }

    /** @brief direct access to the referenced object */
    sc_object* operator*() const
        { return p_object; }

    inline operator sc_object*() const
        { return p_object; }

    /** @brief compare with other object handle
     * @return true if equal, false otherwise
     */
    bool operator==(const rc_object_handle& other) const
        { return p_object == other.p_object; }

    /** @brief specifies the object handle's natural sort order
     * 
     * \note object pointer values are compared.
     */
    bool operator<(const rc_object_handle& other) const
        { return p_object < other.p_object; }

private:
    /** @brief the referenced object */
    sc_object*       p_object;
    /** @brief the referenced channel */
    sc_interface*    p_channel_if;
    /** @brief the referenced port */
    rc_port_handle   p_port;
    /** @brief the referenced export */
    rc_export_handle p_export;
};

/* template code */

template<class IF, int N, sc_port_policy POL>
rc_object_handle::rc_object_handle(sc_port<IF, N, POL>& port_)
    : p_object(static_cast<sc_object*>(&port_)), p_channel_if(NULL),
      p_port(port_)
{ }

template<class IF>
rc_object_handle::rc_object_handle(sc_export<IF>& export_)
    : p_object(static_cast<sc_object*>(&export_)), p_channel_if(NULL),
      p_export(export_)
{ }

template<class IF, int N, sc_port_policy POL>
rc_object_handle& rc_object_handle::operator=(sc_port<IF, N, POL>& port_)
{
    p_object = static_cast<sc_object*>(&port_);
    p_channel_if = NULL;
    p_port = port_;
    p_export.reset();
    return *this;
}

template<class IF>
rc_object_handle& rc_object_handle::operator=(sc_export<IF>& export_)
{
    p_object = static_cast<sc_object*>(&export_);
    p_channel_if = NULL;
    p_port.reset();
    p_export = export_;
    return *this;
}

} // namespace ReChannel

#endif // RC_PORT_HANDLE_H_

//
// $Id: rc_object_handle.h,v 1.3 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/util/rc_object_handle.h,v $
//
