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
 * @file    rc_sc_fifo.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/11/23 13:25:45 $
 * $Revision: 1.7 $
 */

#ifndef RC_SC_FIFO_H_
#define RC_SC_FIFO_H_

#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/core/rc_resettable.h"
#include "ReChannel/communication/exportals/rc_fifo_exportals.h"

namespace ReChannel {

/**
 * @brief Resettable FIFO (wrapping class sc_fifo).
 */
template<class T>
class rc_sc_fifo
    : public sc_channel,
      virtual public sc_fifo_in_if<T>,
      virtual public sc_fifo_out_if<T>,
      virtual public rc_resettable
{
public:

/* constructors */

    explicit rc_sc_fifo(int n=16);

    rc_sc_fifo(const sc_module_name& name_, int n=16);

/* interface methods */

    virtual void register_port(sc_port_base& port_, const char* if_name_)
        { return p_fifo.register_port(port_, if_name_); }

    virtual bool nb_read(T& value)
        { return get_in_if().nb_read(value); }

    virtual void read(T& value)
        { get_in_if().read(value); }

    virtual T read()
        { return get_in_if().read(); }

    virtual int num_available() const
        { return get_in_if().num_available(); }

    virtual const sc_event& data_written_event() const
        { return get_in_exportal_if().data_written_event(); }

    virtual bool nb_write(const T& value)
        { return get_out_if().nb_write(value); }

    virtual void write(const T& value)
        { get_out_if().write(value); }

    virtual int num_free() const
        { return get_out_if().num_free(); }

    virtual const sc_event& data_read_event() const
        { return get_out_exportal_if().data_read_event(); }

/* other channel methods */

    operator T()
        { return this->read(); }

    rc_sc_fifo<T>& operator=(const T& value)
        { this->write(value); return *this; }

    virtual void print(std::ostream& sout=std::cout) const
        { p_fifo.print(sout); }

    virtual void dump(std::ostream& sout=std::cout) const
        { p_fifo.dump(sout); }

    virtual const char* kind() const
        { return p_fifo.kind(); }

protected:

    virtual void start_of_simulation()
    {
        p_constr_done = true;
    }

    RC_ON_RESET()
    {
        // clear the FIFO
        T dummy;
        while(p_fifo.nb_read(dummy))
        { }
    }

    RC_ON_INIT_RESETTABLE() { }

private:

    inline sc_fifo_in_if<T>& get_in_if();

    inline const sc_fifo_in_if<T>& get_in_if() const;

    inline sc_fifo_in_if<T>& get_in_exportal_if() const
        { return *p_in_if; }

    inline sc_fifo_out_if<T>& get_out_if();

    inline const sc_fifo_out_if<T>& get_out_if() const;

    inline sc_fifo_out_if<T>& get_out_exportal_if() const
        { return *p_out_if; }

private:
    sc_fifo<T>                      p_fifo;
    rc_exportal<sc_fifo_in_if<T> >  p_exp_in;
    rc_exportal<sc_fifo_out_if<T> > p_exp_out;
    const rc_reconfigurable*        p_reconf;
    sc_fifo_in_if<T>*               p_in_if;
    sc_fifo_out_if<T>*              p_out_if;
    bool                            p_constr_done;
};

/* inline code */

template<class T>
inline
sc_fifo_in_if<T>& rc_sc_fifo<T>::get_in_if()
{
    if (p_reconf != NULL && p_constr_done) {
        return *p_in_if;
    } else {
        return p_fifo;
    }
}

template<class T>
inline
const sc_fifo_in_if<T>& rc_sc_fifo<T>::get_in_if() const
{
    if (p_reconf != NULL && p_constr_done) {
        return *p_in_if;
    } else {
        return p_fifo;
    }
}

template<class T>
inline
sc_fifo_out_if<T>& rc_sc_fifo<T>::get_out_if()
{
    if (p_reconf != NULL && p_constr_done) {
        return *p_out_if;
    } else {
        return p_fifo;
    }
}

template<class T>
inline
const sc_fifo_out_if<T>& rc_sc_fifo<T>::get_out_if() const
{
    if (p_reconf != NULL && p_constr_done) {
        return *p_out_if;
    } else {
        return p_fifo;
    }
}

/* template code */

template<class T>
rc_sc_fifo<T>::rc_sc_fifo(int n)
    : sc_channel(sc_module_name(sc_gen_unique_name("fifo"))),
      p_fifo(n),
      p_exp_in("_rc_exportal_in"), p_exp_out("_rc_exportal_out"),
      p_in_if(NULL), p_out_if(NULL), p_constr_done(false)
{
    p_exp_in.bind_exclusively(p_fifo);
    p_exp_out.bind_exclusively(p_fifo);
    p_reconf = rc_register_resettable(*this, this->get_parent_object());
    p_in_if = &p_exp_in.static_export();
    p_out_if = &p_exp_out.static_export();
}

template<class T>
rc_sc_fifo<T>::rc_sc_fifo(const sc_module_name& name_, int n)
    : sc_channel(name_),
      p_fifo(name_, n),
      p_exp_in("_rc_exportal_in"), p_exp_out("_rc_exportal_out"),
      p_in_if(NULL), p_out_if(NULL), p_constr_done(false)
{
    p_exp_in.bind_exclusively(p_fifo);
    p_exp_out.bind_exclusively(p_fifo);
    p_reconf = rc_register_resettable(*this, this->get_parent_object());
    p_in_if = &p_exp_in.static_export();
    p_out_if = &p_exp_out.static_export();
}

} // namespace ReChannel

#endif //RC_SC_FIFO_H_

//
// $Id: rc_sc_fifo.h,v 1.7 2007/11/23 13:25:45 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_sc_fifo.h,v $
//
