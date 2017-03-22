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
 * @file    rc_resettable.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   ABI rc_resettable.
 *
 * \see ReChannel::rc_resettable
 *
 * $Date: 2007/10/14 05:29:39 $
 * $Revision: 1.10 $
 */

#ifndef RC_RESETTABLE_H_
#define RC_RESETTABLE_H_

#include "ReChannel/core/rc_common_header.h"

namespace ReChannel {

class rc_reconfigurable;

/**
 * @brief The pure virtual interface rc_resettable.
 * 
 * A component is regarded as <em>resettable</em> if it implements the abstract
 * base interface rc_resettable. Resettable components are automatically
 * reset in case of activation or deactivation of their associated
 * reconfigurable context.
 * For the reset mechanism to work, resettable components have to be registered
 * at a rc_reconfigurable instance. Usually a resettable component
 * automatically registers itself to the current reconfigurable context by
 * invoking function rc_register_resettable() at construction.
 *
 * The execution of rc_on_reset() and rc_on_init_resettable() shall not
 * take any time, i.e. it may not invoke any wait statements.
 *
 * \remark rc_on_init_resettable() will be called once by rc_reconfigurable at
 * the start of simulation. rc_on_reset() is called every time a
 * rc_reconfigurable is activated or deactivated during simulation phase, i.e.
 * normally it will be called twice. An implementation is free to only reset
 * itself either at activation or deactivation if this is possible (and safe).
 * 
 * \see \ref sec_reset_comp, rc_reconfigurable, rc_register_resettable(),
 *      rc_get_reconfigurable_context()
 */
class rc_resettable
{
    /**
     * @brief This interface is intended to be used by rc_reconfigurable.
     */
    friend class rc_reconfigurable;

protected:
	/**
	 * @brief Called to request the object for an immediate reset.
	 * 
	 * rc_on_reset() is called every time a
     * rc_reconfigurable is activated or deactivated during simulation phase,
     * i.e. normally it will be called twice. An implementation is free to only
     * reset itself either at activation or deactivation.
     *
	 * \attention An implementation of this method shall not invoke any wait
     *         statements.
     * \note Method should be declared to be "protected".
	 */
	virtual void rc_on_reset() = 0;

    /**
	 * @brief Called when the object shall store its state.
	 * 
     * This callback method shall be invoked to give the object opportunity to
     * store its initial state. A good point of time may be directly after
     * SystemC's construction phase has finished.
     *
	 * \remark An implementation of this method shall not invoke any wait
     *         statements.
     * \note Method should be declared to be "protected".
	 */
    virtual void rc_on_init_resettable() = 0;

    /**
	 * @brief Virtual destructor.
	 */
	virtual ~rc_resettable() {}
};

} // namespace ReChannel

/**
 * @brief Declares the rc_on_reset() callback method.
 * 
 * This convenience macro can be invoked within classes that implement
 * interface rc_resettable. It simplifies the notation of the virtual
 * rc_on_reset() callback method within class declarations.
 *
 * Example:
 * \code
 * RC_ON_RESET() {
 *     [...] // implementation of the reset
 * }
 * \endcode
 *
 * \see rc_resettable
 */
#define RC_ON_RESET() \
    virtual void rc_on_reset()

/**
 * @brief Declares the rc_on_init_resettable() callback method.
 * 
 * This convenience macro can be invoked within classes that implement
 * interface rc_resettable. It simplifies the notation of the virtual
 * rc_on_init_resettable() callback method within class declarations.
 *
 * Example:
 * \code
 * RC_ON_INIT_RESETTABLE() {
 *     [...] // implementation of the initialisation
 * }
 * \endcode
 *
 * \see rc_resettable
 */
#define RC_ON_INIT_RESETTABLE() \
    virtual void rc_on_init_resettable()

#endif // RC_RESETTABLE_H_

//
// $Id: rc_resettable.h,v 1.10 2007/10/14 05:29:39 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/core/rc_resettable.h,v $
//
