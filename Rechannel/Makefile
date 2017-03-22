#
# Copyright (C) 2007, Technical Computer Science Group,
#                     University of Bonn
#
# This file is part of the ReChannel library.
#
# The ReChannel library is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This ReChannel library is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library; see the file COPYING. If not, write to the
# Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
# Boston, MA 02110-1301, USA.
#
#
# Static Makefile for the ReChannel library
#
# Author(s): Armin Felke <felke@cs.uni-bonn.de>
#            Philipp A. Hartmann <hartman@cs.uni-bonn.de>
#            
# $Revision: 1.8 $
# $Date: 2007/10/07 23:09:58 $
#

# Needed programs
CXX      ?= g++
DOXYGEN  ?= doxygen
DEL      ?= rm -f
MOVE     ?= mv -f
AR       ?= ar
RANLIB   ?= ranlib

# compiler configuration
OPT    = -O3 -DRC_DEBUG 
DEBUG  = -g  -DRC_DEBUG -DRC_DEBUG_LEVEL=5
OTHER  = -DSC_INCLUDE_DYNAMIC_PROCESSES \
         -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -Wall -std=c++11 \
         -LC:\systemc-2.2.0\lib-cygwin -lsystemc 

#CFLAGS = $(OPT) $(OTHER)
CFLAGS = $(DEBUG) $(OTHER)

# SystemC installation
# if the variable that points to SystemC installation path is not
# set, try a sane default
SYSTEMC ?= C:\systemc-2.2.0

# Doxygen documentation of SystemC
SYSTEMC_DOC ?= ../systemc/html
SYSTEMC_DOXYGEN_TAG := systemc.tag

# include directories
INCLUDES = -I. -I$(SYSTEMC)\include $(EXTRA_INCLUDES) -IE:\simulators\boost_1_34_0

# Project name
PROJECT := ReChannel
# The library name
LIBNAME := librechannel.a

# Source-Directories
SRCDIRS := \
	./ReChannel/util \
    ./ReChannel/core \
    ./ReChannel/communication \
    ./ReChannel/communication/accessors \
    ./ReChannel/communication/portals \
    ./ReChannel/communication/exportals \
    ./ReChannel/components

# Source files
SRCS := $(wildcard $(SRCDIRS:%=%/*.cpp))
# Object files
OBJS := $(SRCS:.cpp=.o)
# Dependency files
DEPS := $(SRCS:.cpp=.d)

# build the library by default
$(PROJECT): $(LIBNAME)

# build library AND documentation
all: ReChannel

# create shared library
$(LIBNAME): $(OBJS)
	@echo "* Creating static library..."
	$(AR) rc $@ $^
	$(RANLIB) $@
	#$(MOVE) $(LIBNAME) ../lib/$(LIBNAME)

# rule to compile a single source file
.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) \
	    -o $@ \
	    -c $<

# create project documentation
doc:
	@echo "* Generating Doxygen documentation..."
	((cat Doxyfile ; \
	  echo "GENERATE_TAGFILE=../doc/doxygen/$(PROJECT).tag"; \
	  echo "TAGFILES=../doc/doxygen/$(SYSTEMC_DOXYGEN_TAG)") \
	  | $(DOXYGEN) -) && ( \
		find ../doc/doxygen/html -name "*.html" | \
		xargs ../doc/doxygen/html/installdox \
		    -l $(SYSTEMC_DOXYGEN_TAG)@$(SYSTEMC_DOC) \
	)

# create SystemC documentation
doc-systemc:
	@echo "* Generating Doxygen documentation for SystemC..."
	(cat ./Doxyfile-systemc ; \
	 echo "INPUT=$(SYSTEMC)/include" ; \
	 echo "GENERATE_TAGFILE=../doc/doxygen/$(SYSTEMC_DOXYGEN_TAG)"; \
	 echo "OUTPUT_DIRECTORY=../doc/doxygen/systemc") \
	| $(DOXYGEN) -

# create all documentation
doc-all: doc-systemc doc
	# doc-systemc doc

# clean
objclean:
	@echo "* Cleaning object files..."
	$(DEL) $(OBJS)

# clean project documentation
docclean:
	@echo "* Cleaning documentation ..."
	$(DEL) -r ../doc/doxygen/html
	$(DEL)    ../doc/doxygen/$(PROJECT).tag
	$(DEL)    ../doc/doxygen/warnings.log

# clean SystemC documentation
docclean-systemc:
	@echo "* Cleaning SystemC documentation ..."
	$(DEL) -r ../doc/doxygen/systemc
	$(DEL)    ../doc/doxygen/$(SYSTEMC_DOXYGEN_TAG)

# clean all documentation
docclean-all: docclean-systemc docclean
	# docclean-systemc docclean

clean: objclean
	@echo "* Cleaning dependency files ..."
	$(DEL) $(DEPS)
	@echo "* Cleaning library file ..."
	$(DEL) $(LIBNAME)
	$(DEL) ../lib/$(LIBNAME)

distclean: docclean clean
	# distclean

# Dependency handling of source files
%.d: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -MM \
	    -MT $(<:.cpp=.o) \
	    -o  $@ \
	    -c  $<

# actually process dependencies
include $(DEPS)

# $Id: Makefile,v 1.8 2007/10/07 23:09:58 felke Exp $
