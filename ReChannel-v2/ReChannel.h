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
 * @file    ReChannel.h
 * @author  Armin Felke <felke@cs.uni-bonn.de>
 * @author  Andreas Raabe <raabe@cs.uni-bonn.de>
 *
 * @brief   Top level include file of ReChannel.
 *
 * All public classes and functions are included in this file.
 * The namespace ReChannel is imported into the current namespace.
 *
 * $Date: 2007/12/20 20:43:59 $
 * $Revision: 1.17 $
 */

namespace ReChannel {
/**
 * \mainpage ReChannel - A Reconfiguration Simulation Library for SystemC
 *
 * <SPAN style="line-height: 1.2em">
 *
 * \section sec_toc Table of Contents
 * - \ref sec_introduction
 * - \ref sec_requirements
 * - \ref sec_sim_sem
 * - \ref sec_rechannel_concept
 * - \ref sec_using_rechannel
 *  - \ref sec_derive_reconf_module
 *  - \ref sec_reconf_module
 *  - \ref sec_conn_switch
 *  - \ref sec_reconf_control
 *  - \ref sec_moving_reconf_module
 *  - \ref sec_custom_channels
 * - \ref sec_reconf_behaviour
 *  - \ref sec_reconf_context
 *  - \ref sec_reset_comp
 *  - \ref sec_custom_reset_comp
 *  - \ref sec_reset_proc
 * - \ref sec_further_topics
 *
 * <hr> \section sec_introduction Introduction
 *
 * %ReChannel is a C++ class library that has been developed for the purpose of
 * extending SystemC with the aspects of Dynamically Reconfigurable Hardware
 * (DRHW). This class library provides a toolkit for system designers to aid
 * them in the task of describing and simulating Dynamic Reconfiguration (DR)
 * in SystemC on all levels of abstraction. On the one hand, it is possible to
 * turn existing static modules (e.g., third party IP) into reconfigurable ones,
 * completely without any modifications to their code. On the other hand,
 * %ReChannel provides a set of language extensions that allow designers to
 * explicitly describe dynamically reconfigurable behaviour and structure in an
 * intuitive way. In addition to this, both approaches may also be combined
 * for augmenting a static module with additional dynamic behaviour.
 *
 * %ReChannel does not alter the simulation kernel of SystemC in any way, as it
 * was designed under the premise to be compatible with any SystemC 
 * implementation that is compliant to the current language standard
 * (IEEE 1666-2005). The class library has been tested with the standard
 * compliant OSCI SystemC implementation (version 2.2) and the following C++
 * compilers: Microsoft Visual C++ 2003 .NET (SP1), Microsoft Visual C++ 2005,
 * GNU GCC 3.4, GNU GCC 4.1 and Intel C++ Compiler 8.0.
 *
 * <hr> \section sec_requirements Requirements
 *
 * %ReChannel requires the existence/installation of the following external
 * tools and libraries: 
 * <ul>
 * <li>A modern C++ compiler<br>
 *     (with the capability of partial template specialisation)</li>
 * <li>IEEE standard compliant SystemC Library<br>
 *     (e.g., OSCI SystemC v2.2; downloadable from
 *      http://www.systemc.org)</li>
 * <li>Boost C++ Libraries<br>
 *   (a recent version, e.g., 1.33 or 1.34; downloadable from
 *    http://www.boost.org)<br>
 *   Required packages: bind, function, type_traits, multi_index,
 *    and utility.</li>
 * </ul>
 *
 * <hr> \section sec_sim_sem Simulation Semantics
 *
 * A dynamically reconfigurable design consists of one or more reconfigurable
 * design parts and a non-reconfigurable design part. For simplicity, in this
 * documentation a single reconfigurable design part is referred to as the
 * <em>dynamic side</em> and the respective surrounding non-reconfigurable
 * design part is referred to as the <em>static side</em>. The separating
 * border between these two sides is referred to as the <em>boundary</em> of
 * the dynamic side. If two distinct reconfigurable design parts are directly
 * neighbouring each other, the borderline between them is considered to be a
 * part of the static side.
 * Hence, by this definition, two dynamic sides are never directly adjacent to
 * each other (though, in reality the respective hardware areas may be directly
 * connected, of course).
 *
 * The dynamic side is represented by a group of modules, the so-called 
 * <em>reconfigurable modules</em>. Among them only one single module can be
 * <em>loaded</em> at a time, all others have to be <em>unloaded</em>. In
 * simulation semantics a loaded module is regarded as being existent, whereas
 * an unloaded module is regarded as being nonexistent. A loaded module can be
 * in one of two states: <em>active</em> or <em>inactive</em>. Only if a module
 * is currently active, it is allowed to communicate with channels and modules
 * on the static side. The inactive module is regarded as existent but not yet
 * ready for regular operation and is hence still blocked from the outside
 * world. <em>Reconfiguration</em> is defined as the operation of replacing two
 * reconfigurable modules. In simulation semantics this means unloading the
 * currently loaded module and subsequently loading one of the unloaded modules
 * of the group. The dynamic side is said to be <em>undefined</em> if no
 * module is currently loaded. A <em>reconfiguration control</em> is a
 * component that initiates and executes the reconfiguration for a set of
 * associated modules.
 *
 * <hr> \section sec_rechannel_concept The Concept behind ReChannel
 *
 * %ReChannel is a toolkit for the simulation of reconfiguration in SystemC that
 * implements the simulation semantics as described in the preceding chapter. 
 * Hence, it must be capable of emulating the temporary nonexistence of design
 * components. For this reason, %ReChannel provides the concept of the
 * <em>switch</em>.
 *
 * A switch is a helper component that serves as a connector of the dynamic
 * and the static side. The basic principle of a switch is similar to that of a
 * multiplexer (or demultiplexer, respectively). The switch's task is to
 * control communication between the static side and an arbitrary number of
 * reconfigurable modules on the dynamic side. Thereby, it ensures that only
 * the currently active module is allowed to communicate. Unloaded modules are
 * blocked from any external events and access attempts. Thus, a switch allows
 * isolation of unloaded and inactive modules from the rest of the design.
 * As a consequence of the way switches are employed in %ReChannel, a dedicated
 * group of switches technically forms the boundary of a particular
 * reconfigurable design part.
 *
 * %ReChannel distinguishes between two classes of switches: <em>portal</em> and
 * <em>exportal</em>. A portal is used to connect a channel (or port) of the
 * static side with a port of the dynamic side. In contrast, an exportal is
 * used to access an exported channel
 * (\link sc_core::sc_export sc_export<IF>\endlink) on the dynamic side
 * (note: an exportal may optionally be bound to a static port).
 *
 * To overcome SystemC's design rule checks, every reconfigurable module's
 * port has to be bound to a so-called <em>accessor</em> object. The binding of
 * accessors to ports is automatically performed by a switch and is fully
 * transparent to the designer. An accessor is an auxiliary object. It
 * implements a channel interface and aids the portal in controlling the
 * communication which passes through the port.
 *
 * If custom channels are to be used with %ReChannel, a designer has to define a
 * specialised portal (and/or exportal), as well as an accessor implementation.
 * (Further details on this can be found in section \ref sec_custom_channels.)
 *
 * \see rc_switch, rc_portal, rc_exportal, rc_accessor
 *
 * <hr> \section sec_using_rechannel Using ReChannel
 *
 * The %ReChannel library can be used in a design by including the main header
 * file "ReChannel.h".
 * \code #include <ReChannel.h> \endcode
 *
 * \see ReChannel.h
 *
 * <hr>
 * \subsection sec_derive_reconf_module Rendering a Given Module Reconfigurable
 *
 * One way to create a reconfigurable module in %ReChannel is to turn an
 * existing non-reconfigurable module reconfigurable. This approach is
 * especially well-suited for IP reuse.
 *
 * Theoretically, any object can be used with %ReChannel's reconfiguration
 * algorithm. The only technical precondition is that it has to be derived from
 * class rc_reconfigurable. This class contains all the behaviour and state
 * information that is necessary for the simulation of reconfiguration in
 * %ReChannel to work.
 *
 * In contrast to that, for modules it is recommended to automate the 
 * derivation of rc_reconfigurable by simply using the class template 
 * rc_reconfigurable_module. Using rc_reconfigurable_module for the derivation
 * is more convenient and, furthermore, also is a requirement for some more
 * advanced %ReChannel features to work.
 *
 * <b>First variant:</b>
 *
 * For example, let M be a static module. All that has to be done is to
 * state M as a template parameter for class rc_reconfigurable_module. An object
 * of type \link rc_reconfigurable_module rc_reconfigurable_module<M>\endlink
 * actually represents a reconfigurable and
 * ready-to-use version of M. Moreover, by using the "typedef" keyword, this
 * class composition can be simply given a new name, e.g.:
 * \code
 * typedef rc_reconfigurable_module<M> M_rc;
 * \endcode
 *
 * \remark \link rc_reconfigurable_module rc_reconfigurable_module<M>\endlink
 *       automatically inherits all constructors of M with up to ten arguments.
 *
 * <b>Second variant:</b>
 *
 * In cases where additional code and/or properties have to be added to the 
 * reconfigurable version of a module, the module may be directly derived from
 * rc_reconfigurable_module, as the following example illustrates:
 * \code
 * class M_rc
 *     : public rc_reconfigurable_module<M>
 * {
 *     [...] // additional members
 *     M_rc(const sc_module_name& name_)
 *         : rc_reconfigurable_module<M>(name_) {
 *         [...] // additional initialisations
 *     }
 *     [...] // additional code and members
 * };
 * \endcode
 *
 * <b>Third variant:</b>
 *
 * The macros RC_RECONFIGURABLE_MODULE_DERIVED() and
 * RC_RECONFIGURABLE_CTOR_DERIVED() are provided for convenience to simplify
 * the declaration for the designer. The following notation is equivalent to
 * the declaration shown above:
 * \code
 * RC_RECONFIGURABLE_MODULE_DERIVED(M_rc, M) // declaration of reconf. module
 * {
 *     [...] // additional members
 *     RC_RECONFIGURABLE_CTOR_DERIVED(M_rc, M) { // standard module constructor
 *         [...] // additional initialisations
 *     }
 *     [...] // additional code and members
 * };
 * \endcode
 *
 * \remark M_rc may also be a templated type.
 * For this purpose simply let the declaration macro precede with a C++ template
 * paramter declaration.
 * Type M may be a template, too. But there is a problem with macros and
 * multiple template parameters due to the requirement for commas. Therefore one
 * may use the macros RC_PARAMS2(), RC_PARAMS3(), ... RC_PARAMS10() to enclose
 * potential template parameters of M.
 * <i>[Variadic macros are not provided due to compatibility considerations
 * regarding MSVC .NET 2003]</i>
 *
 * \see rc_reconfigurable, rc_reconfigurable_module,
 *      RC_RECONFIGURABLE_MODULE_DERIVED(),
 *      RC_RECONFIGURABLE_CTOR_DERIVED()
 *
 * <hr>
 * \note How to set up the delays for reconfiguration operations (like "load"
 * or "activate") see section "The Reconfiguration Control".
 *
 * <hr> \subsection sec_reconf_module Creating a Reconfigurable Module from Scratch
 *
 * For the purpose of creating a reconfigurable module from scratch, %ReChannel
 * allows the class rc_reconfigurable_module to be used with empty template
 * brackets, e.g.:
 * \code
 * class M_rc
 *     : public rc_reconfigurable_module<>
 * { [...] };
 * \endcode
 *
 * Additionally, the macros
 * RC_RECONFIGURABLE_MODULE() and RC_RECONFIGURABLE_CTOR()
 * are available to ease the definition of pure reconfigurable modules, which
 * can be used as follows:
 * \code
 * RC_RECONFIGURABLE_MODULE(M_rc) // declares the reconfigurable module M_rc
 * {
 *     [...]
 *     RC_RECONFIGURABLE_CTOR(M_rc) { // constructor
 *         [...]
 *     }
 *     [...]
 * };
 * \endcode
 *
 * \see rc_reconfigurable_module,
 *      RC_RECONFIGURABLE_MODULE(), RC_RECONFIGURABLE_CTOR()
 *
 * <hr> \subsection sec_conn_switch Connecting the Switches
 * 
 * For simulation purposes, every reconfigurable module's external port (and
 * export respectively) has to be connected to a switch instance of a
 * compatible type at construction time.
 *
 * \par Portals
 *
 * %ReChannel already comes with a set of predefined portals, one for each named
 * SystemC port. For the declaration of a portal type, class
 * \link rc_portal rc_portal<PORT>\endlink is parametrised with the type of a
 * connectable port, e.g.:
 * \code
 * rc_portal<sc_in<int> >  in_portal;
 * rc_portal<sc_out<int> > out_portal;
 * \endcode
 *
 * \note Named versions for the predefined portal classes are available, too,
 *       e.g.: rc_in_portal<T>, rc_out_portal<T>, rc_fifo_in_portal<T>, etc.
 *       (In future C++ versions these portal types might be declared by
 *       templated typedefs.)
 *
 * Portals can be connected to a static channel/port by the method
 * bind_static(). Alternatively, the portal's child port static_port can be
 * used for the direct binding with a static port of type PORT.<br>
 * With method bind_dynamic() a portal is connected to the
 * port of a reconfigurable module.
 * dynamic_port is available, too, as an alias for method bind_dynamic().
 * dynamic_port is in reality no port but the name of a method.
 *
 * E.g., let m1 and m2 be modules, with two
 * ports of type sc_in<int> and sc_out<int> each. On the static side there are two
 * signal channels, in_signal and out_signal, to which the modules are to be
 * connected to. The binding will look as follows:
 * \code
 * // connecting the static side
 * in_portal.bind_static(in_signal); 
 * out_portal.bind_static(out_signal);
 *
 * // connecting the dynamic side
 * // binding of reconfigurable module m1 
 * in_portal.bind_dynamic(m1.in_port());
 * out_portal.bind_dynamic(m1.out_port();
 * // binding of reconfigurable module m2
 * in_portal.bind_dynamic(m2.in_port()); 
 * out_portal.bind_dynamic(m2.out_port(););
 * \endcode
 *
 * \see rc_switch, rc_portal, rc_abstract_portal
 *
 * \par Exportals
 *
 * %ReChannel also provides predefined exportals for each standard SystemC
 * interface. For the declaration of an exportal, class rc_exportal<IF> is 
 * parametrised with the interface of the specific channel the export (see 
 * \link sc_core::sc_export sc_export<IF>\endlink) has been bound to, e.g.:
 * \code
 * rc_exportal<sc_fifo_in_if<bool> >         fifo_in_exportal; 
 * rc_exportal<sc_signal_inout_if<sc_lv<8> > signal_exportal;
 * \endcode
 * 
 * The binding of exportals is analogous to the binding of portals, with
 * the difference that instead of ports exports are used.
 *
 * \see rc_switch, rc_exportal, rc_abstract_exportal
 *
 *<hr>
 * \note Some switch types possess special properties or methods to adjust
 * their default behaviour. See the documentation of a particular switch for
 * more information.
 *
 * <hr> \subsection sec_reconf_control The Reconfiguration Control
 *
 * After all external ports and exports of a reconfigurable module have been 
 * connected, the module has to be registered with a reconfiguration control, 
 * which is represented by class rc_control. rc_control is the user interface
 * for the reconfiguration algorithm (see also rc_control_if). It is not
 * possible to directly command a reconfigurable module to reconfigure itself.
 * Multiple instances of rc_control can be used in a design, but a
 * reconfigurable can only be controlled by one control instance at a time.
 *
 * The following example adds the reconfigurable modules m1, m2, m3 to the
 * control. The '+' operator can be used to construct a rc_module_set, by which
 * several modules can be passed to a controller's method at once.
 * \code
 * rc_control ctrl; // declaration of ctrl (e.g., in the top module)
 * [...]
 * ctrl.add(m1 + m2 + m3); // adding multiple modules at once
 * ctrl.add(m4);           // adding module m4
 * ctrl.activate(m3);      // initially, module m3 will be active 
 * \endcode
 *
 * The delay of a reconfiguration operation can be set independently for each
 * module by method rc_set_delay(), either inside the constructor of the module
 * or alternatively after construction from outside. rc_set_delay()'s first
 * parameter is a constant identifying a reconfiguration operation (RC_LOAD,
 * RC_ACTIVATE, RC_DEACTIVATE or RC_UNLOAD) and the second parameter is the
 * delay. The delays default value is always SC_ZERO_TIME.
 *
 * In the following example the default loading delay of modules m1 and m2 is
 * overwritten by a user specified time:
 * \code
 * m1.rc_set_delay(RC_LOAD, sc_time(1,   SC_MS)); // loading m1 will take 1ms
 * m2.rc_set_delay(RC_LOAD, sc_time(0.5, SC_MS)); // loading m2 will take 0.5ms
 * \endcode
 *
 * The class rc_control provides the blocking methods load(), activate(),
 * deactivate() and unload() to control the reconfiguration of a module or a
 * set of modules.
 *
 * An example of a possible control sequence at simulation time:
 * \code
 * // Start of simulation
 * // (m3 is initially active)
 * ctrl.lock(m1 + m2 + m3);   // acquire exclusive access to m1, m2 and m3
 * wait(2, SC_MS);            // wait for a while
 * ctrl.unload(m3);           // unload module m3 
 * ctrl.load(m1);             // load module m1
 * ctrl.activate(m1);         // activate module m1
 * wait(2, SC_MS);            //
 * ctrl.unload(m1);           // unload module m2
 * ctrl.activate(m2)          // load and activate module m2
 * wait(2, SC_MS);            //
 * ctrl.deactivate(m2);       // deactivate module m2
 * ctrl.unlock(m1 + m2 + m3); // release exclusive access
 * \endcode
 *
 * The use of lock() and unlock() is required if a control sequence
 * has to be executed and multiple processes have concurrent access to a
 * particular control instance. However, a single load(), activate(), etc.
 * operation is atomic and cannot be interrupted by another process.
 *
 * The control commands may result in a SystemC run-time error in cases where a
 * reconfigurable module <br>
 * <b>(a)</b> has not been previously added to the control,<br>
 * <b>(b)</b> is currently in use by another thread (i.e. another
 *            control operation is in progress), or <br>
 * <b>(c)</b> conflicts with other modules (i.e. its connected switches are
 *            currently in use by (an)other loaded module(s)).
 *
 * Thus, the loading of two modules that share one or more switches at the same
 * time is not allowed and will result in an error.
 *
 *
 * \see rc_control, rc_control_if, RC_LOAD, RC_UNLOAD, RC_ACTIVATE,
 *      RC_DEACTIVATE, rc_reconfigurable, rc_reconfigurable_set
 *
 * <hr> \subsection sec_moving_reconf_module Moving Reconfigurable Modules
 *
 * Since portals and accessors are loosely coupled, %ReChannel allows to
 * simulate the movement of reconfigurable modules within a design. See the
 * documentation of \link rc_control::move()\endlink and
 * \link rc_control::remove()\endlink for further information on this.
 *
 * <hr> \subsection sec_custom_channels Custom Channels
 *
 * To use a custom channel with %ReChannel, at least one specialised switch
 * (portal and/or exportal) as well as an accessor implementation has to be
 * defined. This can be done with the help of macros or by using the explicit
 * notation (i.e. derivation from a base class). Since a portal is
 * parametrised with a port type (PORT), one portal specialisation has to be
 * defined for every port a channel may be bound to. In contrast, an exportal
 * is parametrised with the interface type (IF) of the channel (which is
 * identical to the interface that is specified for the export:
 * \link sc_core::sc_export sc_export<IF>\endlink).
 *
 * The following subsections give a short introduction to the proceedings of
 * implementing custom portals, exportals and accessors.
 *
 * \note For further information on this topic, please refer to
 * the documentation and source code of %ReChannel's predefined
 * switches and the provided examples.
 *
 * \par How to Implement Custom Portals:
 *
 * With macros a specialised portal class is defined as follows:
 * \code
 * RC_PORTAL(myport_type)            //defines the class rc_portal<myport_type>
 * {                                 //
 *     RC_PORTAL_CTOR(myport_type) { // constructor
 *         [...]                     // custom initialisations
 *     }                             //
 *     [...]                         // custom members
 * };
 * \endcode
 *
 * If myport_type is a class template (e.g., myport_type<T>) the macros
 * RC_PORTAL_TEMPLATE() and RC_PORTAL_TEMPLATE_CTOR() have to be used instead. The
 * template parameter declaration simply precedes the invocation of the
 * declaration macro:
 * \code
 * template<class T>                             // defines the class
 * RC_PORTAL_TEMPLATE(myport_type<T>)            //  rc_portal<myport_type<T> >
 * {                                             //
 *     RC_PORTAL_TEMPLATE_CTOR(myport_type<T>) { // constructor
 *         [...]                                 // custom initialisations
 *     }                                         //
 *     [...]                                     // custom members
 * };
 * \endcode
 *
 * A declaration without the help of macros may look like this:
 * \code
 * template<class T>
 * class rc_portal<myport_type<T> >
 *     : public rc_abstract_portal<myport_type<T> >
 * {
 *     // typedef for convenience
 *     typedef rc_abstract_portal<myport_type<T> > base_type;
 * public:
 *     // typedefs for port, interface and accessor type
 *     typedef typename base_type::port_type     port_type;
 *     typedef typename base_type::if_type       if_type;
 *     typedef typename base_type::accessor_type accessor_type;
 *     // note:
 *     // - if_type is required by some other ReChannel macros
 *     // - port_type and accessor_type are declared to provide more
 *     //    generality in the use of types
 *
 *     rc_portal(const sc_module_name& name_) // constructor
 *         : base_type(name_) // calling constructor of the base class
 *     {
 *         [...] //(1) initialisations (event forwarders, etc.)
 *     }
 *     [...]     //(2) custom members (methods, ports, data, etc.)
 * };
 * \endcode
 *
 * \attention A custom implementation shall define a portal according to the
 * examples shown above. The public typedefs port_type, if_type and
 * accessor_type have to be explicitly stated in definitions that do not use
 * the macros RC_PORTAL_CTOR() or RC_PORTAL_TEMPLATE_CTOR().
 *
 * \remark Class \link rc_port_traits\endlink is used to determine the
 * interface type from a port type.
 * The SystemC language lacks a way to do this.
 * If no appropriate rc_port_traits is available for a custom port it has to be
 * defined by the designer. However, for all build-in SystemC ports as well as
 * for all custom ports that provide the typedef if_type, no additional
 * rc_port_traits is needed.
 *
 * In the constructor - at code position (1) - all events shall be
 * stated that have to be forwarded from the static to the dynamic
 * side. Without the declaration of the so-called <em>event forwarders</em>
 * the triggering of the static channel's events would not be noticed on the
 * dynamic side.
 *
 * For this purpose the macro RC_PORTAL_FORWARD_EVENT() can be invoked within
 * the portal's constructor. The event name, which is passed as argument, has
 * to correspond to the name of the event getter method of the channel's
 * interface. 
 *
 * Consider the following declaration of two event forwarders:
 * \code
 * RC_PORTAL_FORWARD_EVENT(event1); // creates an event forwarder for event1
 * RC_PORTAL_FORWARD_EVENT(event2); // creates an event forwarder for event2
 * \endcode
 *
 * The macro RC_PORTAL_FORWARD_EVENT() expands to a call to method
 * add_event_forwarder() (which represents an alternative notation). The first
 * parameter is a function pointer (or function object) to the channel
 * interface's event getter method. The second is the name of the event
 * (for later referencing).
 *
 * The same event forwarders as above, declared without macros:
 * \code
 * add_event_forwarder(&if_type::event1, "event1"); 
 * add_event_forwarder(&if_type::event2, "event2");
 * \endcode
 *
 * At code position (2) the portal's "switch behaviour" can be defined. For
 * every switch's state change a callback can be specified. The
 * following methods (and macros) are available:
 * \code
 * virtual void rc_on_open();           // macro: RC_ON_OPEN() {...}
 * virtual void rc_on_close();          // macro: RC_ON_CLOSE() {...}
 * virtual void rc_on_undef();          // macro: RC_ON_UNDEF() {...}
 * virtual void rc_on_refresh_notify(); // macro: RC_ON_REFRESH_NOTIFY() {...}
 * \endcode
 * \note See the documentation of the particular method/macro for more details.
 *
 * \see RC_ON_OPEN(), RC_ON_CLOSE(), RC_ON_UNDEF(), RC_ON_REFRESH_NOTIFY(),
 *      rc_abstract_portal<PORT>::add_event_forwarder()
 *
 * Example of a portal with events and behaviour:
 * \code
 * RC_PORTAL(myport_type)
 * {
 *     RC_PORTAL_CTOR(myport_type) {
 *         // declaration of the event forwarders
 *         RC_PORTAL_FORWARD_EVENT(event1);
 *         RC_PORTAL_FORWARD_EVENT(event2);
 *     }
 *     RC_ON_OPEN() {              // what has to be done on opening up
 *         [...] // do something
 *         this->refresh_notify(); // call for the refresh of all events
 *     }
 *     RC_ON_REFRESH_NOTIFY() {    // what has to be done on a refresh
 *         if ([...]) {
 *             this->notify_event("event1"); // notify event1
 *         }
 *         if ([...]) {
 *             this->notify_event("event2"); // notify event2
 *         }
 *     }
 * };
 * \endcode
 *
 * \see rc_switch, rc_portal, rc_abstract_portal
 *
 * \par How to Implement Custom Exportals:
 *
 * Exportals are defined analogously to portals. In
 * contrast to portals, exportals are parametrised with an interface type IF,
 * since they have to be connected to SystemC exports of type
 * \link sc_core::sc_export sc_export<IF>\endlink.
 * The following special macros are available to aid in the implementation
 * of an exportal:<br>
 * RC_EXPORTAL(), RC_EXPORTAL_CTOR(), RC_EXPORTAL_TEMPLATE(),
 * RC_EXPORTAL_TEMPLATE_CTOR(), RC_EXPORTAL_FORWARD_EVENT().
 *
 * The callback declaration macros RC_ON_OPEN(), RC_ON_CLOSE(), etc. may be
 * used with exportals, too.
 *
 * The structure of the definition of an exportal without the use of macros
 * would look like this:
 * \code
 * class rc_exportal<myinterface_type>
 *     : public rc_abstract_exportal<myinterface_type>
 * {
 *     // typedef for convenience
 *     typedef rc_abstract_exportal<myinterface_type> base_type;
 * public:
 *     // the required typedefs for the interface and accessor type
 *     typedef base_type::if_type       if_type;
 *     typedef base_type::accessor_type accessor_type;
 *
 *     rc_exportal(const sc_module_name& name_) // constructor
 *         : base_type(name_) // invoking base class's constructor
 *     {
 *         [...] // initialisations (event forwarders, etc.)
 *     }
 *     [...] // custom members (methods, ports, data, etc.)
 * };
 * \endcode
 *
 * \attention The public typedefs if_type and accessor_type have to be
 * explicitly stated in exportal definitions that do not use the macros
 * RC_EXPORTAL_CTOR() or RC_EXPORTAL_TEMPLATE_CTOR().
 *
 * \see rc_switch, rc_exportal, rc_abstract_exportal
 *
 * \par How to Implement Custom Accessors:
 *
 * Conceptually, an accessor is a forwarder for communication that passes the
 * boundary between the static and dynamic side, and vice versa.
 * Simply speaking, an accessor is the medium connecting a dynamic side's
 * communication object, like port or export, to a portal or exportal switch.
 * %ReChannel makes use of accessor objects to obtain full control over the
 * communication that flows through portals and exportals. A connected
 * reconfigurable module can be the communication endpoint as well as the
 * initiator of the call.
 *
 * The behaviour of switch and accessor is directly linked with the state of the
 * reconfigurable module that is either active, inactive or unloaded. An
 * Interface Method Call (IMC) on a connected port's or export's interface
 * therefore will be forwarded or blocked, respectively.
 *
 * The accessor is represented by class rc_accessor<IF>. rc_accessor is
 * parameterised with the custom communication interface type (IF).
 * Ready-to-use accessor implementations are already available for all native
 * SystemC channels. The creation of a custom accessor implementation is
 * required in cases where static and dynamic side are connected by user
 * defined channels. For this purpose a specialisation of class rc_accessor<IF>
 * has to be defined that is derived from rc_abstract_accessor<IF>.
 *
 * Definition macros are provided for convenience. The following code
 * demonstrates their use:
 * \code
 * RC_ACCESSOR(myChannel_if)              // declares the accessor
 * {
 *     RC_ACCESSOR_CTOR(myChannel_if) { } // constructor
 *
 *     [...]                              // event declarations
 *                                        // and interface methods
 * };
 * \endcode
 *
 * If the interface is a template, the macros RC_ACCESSOR_TEMPLATE() and
 * RC_ACCESSOR_TEMPLATE_CTOR() have to be used:
 * \code
 * template<class T>
 * RC_ACCESSOR_TEMPLATE(myChannel_if<T>)  // declares the accessor
 * {
 *     RC_ACCESSOR_TEMPLATE_CTOR(myChannel_if<T>) { } // constructor
 *
 *     [...]                              // event declarations
 *                                        // and interface methods
 * };
 * \endcode
 *
 * \note
 * For convenience, the constructor macros RC_ACCESSOR_CTOR() and
 * RC_ACCESSOR_TEMPLATE_CTOR() declare the public typedef if_type at class
 * scope. if_type is the interface type the accessor was created for.
 *
 * The accessor's base class rc_abstract_accessor<IF> is an abstract class,
 * as it is derived from the pure virtual interface IF.
 * Hence, the user has to implement <b>all</b> interface methods of
 * rc_accessor<IF>. Fortunately, there is no need for a fully-fledged
 * reimplementation, since he accessor shall simply forward the call according
 * to its access type. The four supported access types are "blocking",
 * "non-blocking", "blocking driver" and "non-blocking driver". The
 * respective forwarder methods are rc_forward(), rc_nb_forward(),
 * rc_forward_driver() and rc_nb_forward_driver(). The first parameter is a
 * pointer to the interface method that is called, followed by up to ten
 * optional parameters representing the method's arguments.
 * 
 * The following example illustrates how a non-blocking IMC is forwarded:
 * \code
 * virtual const T& read() const {
 *     return this->rc_nb_forward(&if_type::read);
 * }
 * \endcode
 *
 * The use of rc_forward_driver() and rc_nb_forward_driver() enables the
 * forwarding of driver accesses. These two so-called driver forwarder methods
 * are provided to solve a problem that directly arises from ReChannel's
 * simulation mechanism. Multiple reconfigurable modules accessing one
 * static channel are always a potential source for driver conflicts. In
 * general, driver forwarding is applied if driver identity is required, i.e.
 * the channel's functioning relies on the distinction of process identities.
 * As an example, consider the non-blocking write method of a signal channel's
 * interface:
 * \code
 * virtual void write(const T& value) const {
 *     this->rc_nb_forward_driver(&if_type::write, rc_cref(value));
 * }
 * \endcode
 *
 * \note rc_ref() and rc_cref() shall be used whenever arguments have to be
 * passed by (const) reference and not by value.
 *
 * \remarks
 * Due to technical restriction (or, more precisely, due to the way SystemC's
 * method processes work), the forwarding of non-blocking driver calls is
 * restricted to methods that do not have a return value.
 *
 * In the following, some examples of possible calls are listed that may be
 * used for IMC forwarding within accessor implementations.
 *
 * - A blocking access with two "by value" parameters and one reference
 *   parameter:
 * \code
 * this->rc_forward(&if_type::calculate, op1, op2, rc_ref(ret));
 * \endcode
 * - A blocking access with return value and parameters (e.g., a transaction):
 * \code
 * return this->rc_forward(&if_type::call, a, b, c);
 * \endcode
 * - A non-blocking access with return value and a constant reference parameter:
 * \code this->rc_nb_forward(&if_type::call, rc_cref(arg)); \endcode
 * - A simple blocking driver access:
 * \code this->rc_forward_driver(&if_type::bus_write, value); \endcode
 *
 * \remark Mixing of blocking and non-blocking driver forwarder methods in an
 * accessor implementation may result in unexpected behaviour and should be
 * avoided. A process calling methods of both types will result in the creation
 * of two distinct so-called driver objects and may cause driver conflicts or
 * other interferences.
 *
 * In contrast to the above forwarding technique, the event getter methods of
 * an interface have to be handled differently, as they supply events which
 * have to be forwarded by other means. Since processes of the dynamic side can
 * be sensitive to events of the static side, event
 * notifications have to be forwarded or suppressed as well. To gain control
 * over events that are listended beyond reconfiguraton boundaries, event
 * getter methods may not return the actual channel's event but rather return a
 * clone of the event.
 * The event's clone as well as the event getter method are declared by
 * invoking convenience macro RC_EVENT() within the accessor's class scope. If
 * the identical event is also returned by one or more other event getter
 * methods, RC_EVENT_ALIAS() shall be used for each additional declaration to
 * avoid unnecessary duplicates.
 *
 * \code
 * // the interface myChannel_if<T>
 * template<class T>
 * class myChannel_if<T>
 *     : virtual public sc_interface
 * {
 * public:
 *     virtual const sc_event& value_changed_event() const = 0;
 *     virtual const T& value read() const = 0;
 *     virtual void write(const T& value) const = 0;
 * };
 * \endcode
 * \code
 * // the accessor for interface myChannel_if<T>
 * template<class T>
 * RC_ACCESSOR_TEMPLATE(myChannel_if<T>)
 * {
 *    // constructor
 *    RC_ACCESSOR_TEMPLATE_CTOR(myChannel_if<T>) { }
 *
 *    // event declarations
 *    RC_EVENT(value_changed);
 *    RC_EVENT_ALIAS(value_changed_event, default_event);
 *
 *    // interface methods
 *    virtual const T& value read() const
 *        { return this->rc_nb_forward(&if_type::read); }
 *    virtual void write(const T& value) const
 *        { this->rc_nb_forward_driver(&if_type::write, rc_cref(value)); }
 * };
 * \endcode
 *
 * \remark To solve the problem with macros and multiple template parameters
 * (reason: requirement for commas) one may use the macros RC_PARAMS2(),
 * RC_PARAMS3(), ... RC_PARAMS10() to enclose potential template parameters
 * of M.
 * <i>[Variadic macros are not provided due to compatibility considerations
 * regarding MSVC .NET 2003]</i>
 *
 * \see rc_accessor, rc_abstract_accessor,
 *      rc_abstract_accessor<IF>::rc_forward(),
 *      rc_abstract_accessor<IF>::rc_nb_forward(),
 *      rc_abstract_accessor<IF>::rc_forward_driver(),
 *      rc_abstract_accessor<IF>::rc_nb_forward_driver(),
 *      rc_driver_object, rc_nb_driver_object
 *
 * <hr>
 * \section sec_reconf_behaviour Explicit Description of Reconfigurable Behaviour
 *
 * In a static module the states of internal subcomponents, variables and
 * thread processes are inherently persistent, and cannot be magically reset by
 * an external command. Instead, a SystemC module's reset functionality has to
 * be explicitly provided by the designer. The reset will be carried out by a
 * process which is part of the simulated design and may require some time (e.g., a number
 * of clock cycles). In pure static models such a behaviour is desirable, but
 * inappropriate for the emulation of reconfiguration (one may, for example,
 * think of a module's bitstream which is newly written to reconfigure a FPGA).
 * This is a limitation imposed by rendering static modules reconfigurable
 * by derivation.
 *
 * Therefore, %ReChannel allows explicit description of dynamically
 * reconfigurable behaviour in SystemC. This is preferentially applied if a
 * reconfigurable module is build from scratch, or if it is augmented with
 * additional dynamic behaviour. For this purpose, %ReChannel comes with a set
 * of language extensions intended for explicit description of reconfiguration.
 * In constrast to native SystemC, %ReChannel language constructs possess an
 * implicit reset mechanism being triggered on reconfiguration.
 *
 * %ReChannel's modelling constructs are primarily comprised of classes,
 * functions and macros corresponding to a particular functionality already
 * known from SystemC. They are named like their match in SystemC but can be
 * recognised by a different prefix ("rc_") taking the place of the native one
 * ("sc_"). In addition to a collection of basic components - like rc_module,
 * rc_event, rc_signal, etc. - %ReChannel provides several functions and macros,
 * e.g., rc_spawn() or RC_THREAD(), each of which corresponds to a particular
 * functionality in SystemC with the same name.
 *
 * With the availability of resettable components and processes, both structure
 * and behaviour of reconfigurable modules can be modelled in an intuitive way
 * without the need to care about additional logic that deals with
 * reconfigurable behaviour itself.
 * And due to the strong syntactical similarity, modelling DRHW with %ReChannel
 * may appear quite familiar to those who are already used to SystemC.
 *
 * <hr> \subsection sec_reconf_context The Reconfigurable Context
 *
 * Performing a reconfiguration action on a module will always affect this
 * module on the whole (that is all of its internal structure). Hence there
 * must be a way by which a child component can determine its reconfigurable
 * module up the SystemC object hierarchy tree, i.e. the first object among
 * the component's parent list which is derived from class rc_reconfigurable.
 * Such a parent is denoted as the <em>reconfigurable context</em> of the
 * component. Whereas if no such parent exists, a component is said to be used
 * in a <em>non-reconfigurable context</em>.
 *
 * The current reconfigurable context can be obtained by a call to the global
 * function rc_get_reconfigurable_context() which returns a pointer to a
 * rc_reconfigurable instance if found, or NULL otherwise.
 * If this function is called during the construction of a reconfigurable
 * module, that object is returned as the <em>current reconfigurable context
 * </em>. If rc_get_reconfigurable_context() is called on other occasions -
 * e.g., during SystemC's simulation phase - the current position in the
 * hierarchy tree is unknown, since a function call is not associated with a
 * particular location in the object model. To handle this second case, an
 * argument of type sc_object can be passed to specify a starting point for the
 * necessary hierarchy tree traversal.
 *
 * \see rc_reconfigurable, rc_get_reconfigurable_context(),
 *      rc_non_reconfigurable
 *
 * <hr> \subsection sec_reset_comp Resettable Components
 *
 * A component is regarded as <em>resettable</em> if it implements the abstract
 * base interface rc_resettable. Resettable components are automatically
 * reset in case of activation or deactivation of their associated
 * reconfigurable context.
 * \code
 * class rc_resettable
 * {
 *     friend class rc_reconfigurable;
 * protected:
 *     virtual void rc_on_reset() = 0;
 *     virtual void rc_on_init_resettable() = 0;
 * };
 * \endcode
 *
 * For the reset mechanism to work, resettable components have to be registered
 * at a rc_reconfigurable instance. Usually a resettable component
 * automatically registers itself to the current reconfigurable context by
 * invoking function rc_register_resettable() at construction.
 *
 * The particular state such a component is reset to can be assigned beforehand
 * during the construction phase. At start of simulation the callback method
 * rc_on_init_resettable() is invoked once on all resettables to give them
 * opportunity to store their initial state after construction has finished.
 * The request for an immediate reset is propagated by a call to rc_on_reset().
 *
 * \see rc_resettable, rc_reconfigurable, rc_register_resettable(),
 *      rc_prim_channel,
 *      RC_HAS_VAR(), rc_var(), rc_resettable_var(), rc_preservable_var(),
 *      rc_module, rc_event, rc_signal, rc_fifo, rc_mutex, rc_semaphore,
 *      rc_signal_rv, rc_signal_resolved, rc_buffer, RC_FORK(), RC_JOIN()
 *
 * <hr> \subsection sec_custom_reset_comp Custom Resettable Components
 *
 * The creation of a custom reconfigurable component can be easily achieved by
 * implementing the abstract base interface rc_resettable.
 *
 * <b>Example of a simple custom resettable component:</b>
 * \code
 * class myComponent
 *     : public sc_object,
 *       public rc_resettable //implement abstract base interface rc_resettable
 * {
 *     myComponent() // constructor
 *         : p_curr_value(0), p_reset_value(0)
 *     {
 *         // register the resettable component with the reconfigurable context
 *         rc_register_resettable(*this, this->get_parent_object());
 *     }
 *
 *     [...] // implementation of myComponent
 *
 * protected:
 *     // preservation of initial state
 *     virtual void rc_on_init_resettable() {
 *         p_reset_value = p_curr_value;
 *     }
 *     // definition of reset functionality
 *     virtual void rc_on_reset() {
 *         p_curr_value = p_reset_value;
 *     }
 * private:
 *     int p_curr_value;
 *     int p_reset_value;
 * };
 * \endcode
 *
 * If the custom component is a channel, deriving from base class
 * rc_prim_channel is more convenient.
 *
 * Here is an example that speaks for itself:
 * \code
 * class myComponent
 *     : public rc_prim_channel // derive from rc_prim_channel
 * {
 *     myComponent() // constructor
 *         : p_curr_value(0), p_reset_value(0)
 *     { } // the registration is done by base class rc_prim_channel
 *
 *     [...] // implementation of myComponent
 *
 * protected:
 *     // preservation of initial state
 *     virtual void rc_on_init_resettable() {
 *         p_reset_value = p_curr_value;
 *     }
 *     // definition of reset functionality
 *     virtual void rc_on_reset() {
 *         p_curr_value = p_reset_value;
 *     }
 * private:
 *     int p_curr_value;
 *     int p_reset_value;
 * };
 * \endcode
 *
 * <hr> \subsection sec_reset_proc Resettable Processes
 *
 * \see rc_resettable, RC_HAS_PROCESS(), RC_THREAD(), RC_METHOD(), RC_CTHREAD()
 *      rc_process_control, rc_process_reset
 *
 * \note
 * (See documentation of package "ReChannel/components")
 *
 * <hr> \section sec_further_topics Further topics
 *
 * \subsection sec_switch_connectors Switch Connectors
 *
 * A switch connector is a utility object that simplifies the binding of
 * switches. The static side as well as the dynamic side can be bound by
 * using the methods bind_static() or bind_dynamic(), respectively.
 * A switch connector can be bound to port maps and rc_reconfigurable
 * instances that are compatible.
 *
 * Example:
 * \code
 * // create the switch connector
 * rc_switch_connector<myPortMap> connector(
 *     "connector", clk_portal, reset_portal, A_portal, B_portal, C_portal);
 *
 * // bind static ports and channels
 * connector.bind_static(
 *     myPortMap_static(clk_port, reset_signal, A_fifo, B_fifo, C_fifo));
 * // bind reconfigurable modules
 * connector.bind_dynamic(m1);
 * connector.bind_dynamic(m2);
 * \endcode
 *
 * \see rc_switch_connector.h rc_switch_connector
 *
 * </SPAN>
 */
} // namespace ReChannel

/**
 * \namespace ReChannel
 * @brief The namespace of ReChannel.
 */
/**
 * \namespace ReChannel::internals
 * @brief The internal namespace of ReChannel.
 *
 * This namespace contains all class/function/type definitions that are used
 * internally and are shared among ReChannel classes.
 *
 * The namespace internals is subgrouped to other namespaces
 * which contain internal definitions that are exclusive to a specific class or
 * file.
 */

// RC_MAKE_DOXYGEN_DOCS shall be defined if the
// doxygen documentation is created.

#if defined(RC_MAKE_DOXYGEN_DOCS)
/* The available compiler switches. */

/**
 * @brief Macro to disable output messages sent by RC_COUT() and RC_COUTL().
 * \see RC_COUT(), RC_COUTL()
 */
#define RC_DISABLE_COUT

/**
 * @brief (compatibility option)
 *
 * Define this constant if a Non-OSCI SystemC implementation is used.
 */
#define RC_USE_NON_OSCI_KERNEL

/**
 * @brief (optimisation option)
 *
 * If defined, disables the mechanism which allows to move modules with
 * CTHREADs.
 */
#define RC_DISABLE_CTHREAD_MOBILITY

/**
 * @brief (optimisation option)
 *
 * If defined, ReChannel uses one shared method driver for all non-blocking
 * driver accesses on switches.
 * \see rc_driver_object_base, rc_driver_object, rc_nb_driver_object,
 *          rc_accessor
 */
#define RC_USE_SHARED_METHOD_DRIVER

/**
 * @brief (optimisation option)
 *
 * If defined, informs ReChannel that drivers in signal switches are not
 * needed and thus driver accesses can be omitted.
 * \see rc_driver_object_base, rc_driver_object, rc_nb_driver_object,
 *          rc_accessor
 */
#define RC_SIGNAL_WRITE_CHECK_DISABLED

/**
 * @brief (intended for synthesis tools)
 *
 * If defined, ONLY the synthesis header is included.
 */
#define RC_SYNTHESIS_MODE
#endif // defined(RC_MAKE_DOXYGEN_DOCS)

#ifndef RECHANNEL_INCLUDED
#define RECHANNEL_INCLUDED

#if !defined(RC_SYNTHESIS_MODE)

#include "ReChannel/core/rc_common_header.h"
#include "ReChannel/core/rc_process.h"
#include "ReChannel/core/rc_interface_filter.h"
#include "ReChannel/core/rc_switch.h"
#include "ReChannel/core/rc_reconfigurable.h"
#include "ReChannel/core/rc_reconfigurable_module.h"
#include "ReChannel/core/rc_transaction_counter.h"
#include "ReChannel/core/rc_control.h"
#include "ReChannel/core/rc_portmap.h"
#include "ReChannel/core/rc_switch_connector.h"

#include "ReChannel/util/rc_throwable.h"
#include "ReChannel/util/rc_report.h"
#include "ReChannel/util/rc_driver_object.h"
#include "ReChannel/util/rc_port_traits.h"

#include "ReChannel/communication/accessors/rc_accessor.h"
#include "ReChannel/communication/accessors/rc_signal_accessors.h"
#include "ReChannel/communication/accessors/rc_fifo_accessors.h"
#include "ReChannel/communication/accessors/rc_mutex_accessor.h"
#include "ReChannel/communication/accessors/rc_semaphore_accessor.h"
#include "ReChannel/communication/accessors/rc_event_queue_accessor.h"

#include "ReChannel/communication/portals/rc_portal.h"
#include "ReChannel/communication/portals/rc_signal_portals.h"
#include "ReChannel/communication/portals/rc_resolved_portals.h"
#include "ReChannel/communication/portals/rc_rv_portals.h"
#include "ReChannel/communication/portals/rc_fifo_portals.h"
#include "ReChannel/communication/portals/rc_mutex_portal.h"
#include "ReChannel/communication/portals/rc_semaphore_portal.h"
#include "ReChannel/communication/portals/rc_event_queue_portal.h"

#include "ReChannel/communication/exportals/rc_exportal.h"
#include "ReChannel/communication/exportals/rc_signal_exportals.h"
#include "ReChannel/communication/exportals/rc_fifo_exportals.h"
#include "ReChannel/communication/exportals/rc_mutex_exportal.h"
#include "ReChannel/communication/exportals/rc_semaphore_exportal.h"
#include "ReChannel/communication/exportals/rc_event_queue_exportal.h"

#include "ReChannel/communication/filters/rc_abstract_filter.h"
#include "ReChannel/communication/filters/rc_abstract_prim_filter.h"
#include "ReChannel/communication/filters/rc_fifo_filters.h"
#include "ReChannel/communication/filters/rc_mutex_filter.h"
#include "ReChannel/communication/filters/rc_semaphore_filter.h"

#include "ReChannel/components/rc_components.h"

using namespace ReChannel;

#else

#include "ReChannel/synthesis_header.h"

#endif //!defined(RC_SYNTHESIS_MODE)

#endif //RECHANNEL_INCLUDED

//
// $Id: ReChannel.h,v 1.17 2007/12/20 20:43:59 felke Exp $
// $Source: /var/cvs/projekte/ReChannel-v2/src/ReChannel.h,v $
//
