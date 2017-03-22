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
 * @file    rc_common_header.cpp
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/10/09 00:22:26 $
 * $Revision: 1.3 $
 */

#include "rc_common_header.h"

namespace ReChannel {

sc_object* rc_find_object(
    const char* obj_name, const sc_object* rel_obj)
{
    if (obj_name == NULL) {
        return NULL;
    }
    if (obj_name[0] == '.' || obj_name[0] == '#') {
        if (rel_obj == NULL) {
            return NULL;
        }
        std::string buf;
        if (obj_name[0] == '.') {
            buf.append(rel_obj->name());
            buf.append(obj_name);
        } else { // '#'
            // resolve the relative name
            const char* pos_rel_name = obj_name + 1;
            const sc_object* base_obj = rel_obj;
            while(*pos_rel_name == '.') {
                if (base_obj == NULL) {
                    break;
                }
                base_obj = base_obj->get_parent_object();
                pos_rel_name++;
            }
            if (pos_rel_name == obj_name + 1 || *pos_rel_name == '\0') {
                RC_REPORT_ERROR(RC_ID_RELNAME_SYNTAX_ERROR_,
                    "Relative name '" << obj_name << "'"
                    " could not be resolved due to"
                    " syntax error");
            }
            if (*pos_rel_name == '.') {
                return NULL;
            } else if (base_obj == NULL) {
                buf.append(pos_rel_name);
            } else {
                buf.append(rel_obj->name());
                buf.append(pos_rel_name - 1);
            }
        }
        return sc_find_object(buf.c_str());
    } else {
        return sc_find_object(obj_name);
    }
}

sc_module* rc_find_parent_module(sc_object* obj)
{
    sc_module* parent = NULL;
    while(obj != NULL
        && (parent = dynamic_cast<sc_module*>(
                obj = obj->get_parent_object())) == NULL);
    return parent;
}

} // namespace ReChannel

//
// $Id: rc_common_header.cpp,v 1.3 2007/10/09 00:22:26 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_common_header.cpp,v $
//
