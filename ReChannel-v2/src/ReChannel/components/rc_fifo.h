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
 * @file    rc_fifo.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 * @brief   
 *
 *
 * $Date: 2007/12/20 20:33:51 $
 * $Revision: 1.10 $
 */

#ifndef RC_FIFO_H_
#define RC_FIFO_H_

#include "ReChannel/components/rc_prim_channel.h"
#include "ReChannel/components/rc_report_ids.h"
#include <deque>

namespace ReChannel {

/**
 * @brief Resettable representation of sc_fifo.
 */
template<class T>
class rc_fifo
    : public sc_fifo_in_if<T>,
      public sc_fifo_out_if<T>,
      public rc_prim_channel
{
public:

    explicit rc_fifo(int size_=16)
        : rc_prim_channel(sc_gen_unique_name("fifo")),
          m_max_size(size_), m_num_read(0), m_num_written(0),
          m_reader_port(NULL), m_writer_port(NULL)
        { }

    explicit rc_fifo(const char* name_, int size_=16)
        : rc_prim_channel(name_),
          m_max_size(size_), m_num_read(0), m_num_written(0),
          m_reader_port(NULL), m_writer_port(NULL)
        { }

    virtual const char* kind() const
        { return "rc_fifo"; }

    virtual void register_port(sc_port_base& port_, const char* if_name_);

    inline virtual void read(T& value);
    inline virtual T read();
    inline virtual bool nb_read(T& value);
    inline virtual int num_available() const;

    inline virtual void write(const T& value);
    inline virtual bool nb_write(const T& value);
    inline virtual int num_free() const;

    virtual const sc_event& data_written_event() const
        { return m_data_written_event; }
    virtual const sc_event& data_read_event() const
        { return m_data_read_event; }

    operator T ()
        { return this->read(); }

    rc_fifo<T>& operator=(const T& value)
        { this->write(value); return *this; }

    virtual void print(std::ostream& os=std::cout) const;
    virtual void dump(std::ostream& os=std::cout) const;

protected:

    virtual void update();

    RC_ON_INIT_RESETTABLE()
    {
        // store fifo's initial state
        if (!m_deque.empty()) {
            m_reset_values.insert(
                m_reset_values.end(), m_deque.begin(), m_deque.end());
        }
    }

    RC_ON_RESET()
    {
        // clear the FIFO
        m_deque.clear();
        m_num_read = 0;
        m_num_written = 0;
        m_data_read_event.cancel();
        m_data_written_event.cancel();

        // restore fifo's initial state
        if (!m_reset_values.empty()) {
            m_deque.insert(
                m_deque.end(), m_reset_values.begin(),
                m_reset_values.end());
        }
    }

protected:

    std::deque<T>       m_deque;
    const unsigned int  m_max_size;
    unsigned int        m_num_read;
    unsigned int        m_num_written;

    sc_event            m_data_read_event;
    sc_event            m_data_written_event;

    const sc_port_base* m_reader_port;
    const sc_port_base* m_writer_port;

    std::vector<T>      m_reset_values;

private:
    // disabled
    rc_fifo(const rc_fifo<T>& other);
    rc_fifo<T>& operator=(const rc_fifo<T>& other);
};

/* inline code */

template<class T>
inline int rc_fifo<T>::num_available() const
{
    if (!this->rc_is_active()) {
        return 0;
    }
    return (int)(m_deque.size() - m_num_written);
}

template<class T>
inline int rc_fifo<T>::num_free() const
{
    if (!this->rc_is_active()) {
        return 0;
    }
    return m_max_size - m_deque.size();
}

template<class T>
inline void rc_fifo<T>::read(T& value)
{
    while (this->num_available() == 0) {
        this->wait(m_data_written_event);
    }
    value = m_deque.front();
    m_deque.pop_front();
    m_num_read++;
    this->request_update();
}

template<class T>
inline T rc_fifo<T>::read()
{
    T tmp;
    this->read(tmp);
    return tmp;
}

template<class T>
inline bool rc_fifo<T>::nb_read(T& value)
{
    if (this->num_available() == 0) {
        return false;
    } else {
        value = m_deque.front();
        m_deque.pop_front();
        m_num_read++;
        this->request_update();
        return true;
    }
}

template<class T>
inline void rc_fifo<T>::write(const T& value)
{
    while(this->num_free() == 0) {
        this->wait(m_data_read_event);
    }
    m_num_written++;
    m_deque.push_back(value);
    this->request_update();
}

template<class T>
inline bool rc_fifo<T>::nb_write(const T& value)
{
    if (this->num_free() == 0) {
        return false;
    } else {
        m_num_written++;
        m_deque.push_back(value);
        this->request_update();
        return true;
    }
}

/* template code */

template<class T>
void rc_fifo<T>::register_port(
    sc_port_base& port_, const char* if_name_)
{
    const std::string if_name(if_name_);
    if (if_name == typeid(sc_fifo_in_if<T>).name()) {
        if (m_reader_port != NULL) {
            RC_REPORT_ERROR(RC_ID_FIFO_BINDING_ERROR_,
                "more than one FIFO reader"
                " (in rc_fifo '" << this->name() << "')");
        }
        m_reader_port = &port_;
    } else { // if_name == typeid(sc_fifo_out_if<T>).name()
        if (m_writer_port != NULL) {
            RC_REPORT_ERROR(RC_ID_FIFO_BINDING_ERROR_,
                "more than one FIFO writer"
                " (in rc_fifo '" << this->name() << "')");
        }
        m_writer_port = &port_;
    }
}

template<class T>
void rc_fifo<T>::print(std::ostream& os) const
{
    for(typename std::deque<T>::const_reverse_iterator it =
        m_deque.rbegin();
        it != m_deque.rend();
        ++it)
    {
        os << (*it) << std::endl;
    }
}

template<class T>
void rc_fifo<T>::dump(std::ostream& os) const
{
    os << "name = " << this->name() << ::std::endl;
    unsigned int i = 0;
    for(typename std::deque<T>::const_iterator it = m_deque.begin();
        it != m_deque.end();
        ++it)
    {
        os << "value[" << i << "] = " << (*it) << ::std::endl;
        ++i;
    }
}

template<class T>
void rc_fifo<T>::update()
{
    if (this->rc_is_active()) {
        if (m_num_read > 0) {
            m_data_read_event.notify(SC_ZERO_TIME);
        }
        if (m_num_written > 0) {
            m_data_written_event.notify(SC_ZERO_TIME);
        }
    }
    m_num_read = 0;
    m_num_written = 0;
}

} // namespace ReChannel

#endif //RC_FIFO_H_

//
// $Id: rc_fifo.h,v 1.10 2007/12/20 20:33:51 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel/components/rc_fifo.h,v $
//
