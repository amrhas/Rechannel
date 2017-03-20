#
# Copyright (C) 2005, Technical Computer Science Group,
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
# Generic Makefile for ReChannel test applications
#
# Usable from within a specialized Makefile
# inside one of the test directories. See ./skeleton/
# for an example.
#
# Author(s): Ralph Thesen <thesen@cs.uni-bonn.de>
#            Philipp A. Hartmann <hartman@cs.uni-bonn.de>
#            Armin Felke <felke@cs.uni-bonn.de>
# Version  : $Revision: 1.2 $
# Date     : $Date: 2007/10/07 23:09:58 $

#
# Each of the following variables can be overwritten inside
# the base Makefile. In most cases, the default values should
# be fine.
#

MODULE=reconf_channel

# Needed programs
CXX     ?= g++
DEL     ?= rm -f

# general compiler configuration
OPT    ?= -O3
DEBUG  ?= -g -DRC_DEBUG
OTHER  = -DSC_INCLUDE_DYNAMIC_PROCESSES \
         -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -Wall

# Build debugging version by default.
CFLAGS  ?= $(OPT) $(OTHER)
#CFLAGS ?= $(DEBUG) $(OTHER)

# Default target architecture is linux
# This is only needed to link against SystemC properly, though.
TARGET_ARCH ?= linux

# SystemC installation
# If the variable that points to SystemC installation path is not
# set, try a sane default
SYSTEMC ?= /opt/systemc/default

# ReChannel source tree
RECHANNEL2 ?= ../..
RECHANNEL2_MAKE_DIR = $(RECHANNEL2)/src
RECHANNEL2_INC_DIR = $(RECHANNEL2)/src
RECHANNEL2_LIB_DIR = $(RECHANNEL2)/lib

# Include directories
INCLUDES = -I. -I$(SYSTEMC)/include -I$(RECHANNEL2_INC_DIR) \
           $(EXTRA_INCLUDES)

# Library paths
LIBDIRS = -L. -L.. -L$(SYSTEMC)/lib-$(TARGET_ARCH) \
          -L$(RECHANNEL2_LIB_DIR) $(EXTRA_LIBDIRS)
LIBS    = -lrechannel -lsystemc -lm $(EXTRA_LIBS)

#
# Look for source files
#
ifndef SRCS
# source files are not specified directly
# check source directories - if not set, use current directory
SRCDIRS ?= .
# look for source files in all source directories
SRCS    := $(wildcard $(SRCDIRS:%=%/*.cpp))
endif

#
# The variable MODULE has to be set, since this determines
# the name of the executable
#
ifndef MODULE
$(error "Error: MODULE not set. Bailing out." )
else
# Name of the executable
EXE := $(MODULE).x
endif

# Object files
OBJS := $(SRCS:.cpp=.o)
# Dependency files
DEPS := $(SRCS:.cpp=.d)

# default target: build the executable
# depends on object files and static version of
# ReChannel library
$(EXE): $(OBJS) $(RECHANNEL2_LIB_DIR)/librechannel.a
	@echo "* Linking example application '$(MODULE)' ..."
	$(CXX) $(CFLAGS) \
	    $(INCLUDES) \
	    $(LIBDIRS) \
	    -o $@ \
	    $(OBJS) \
	    $(LIBS) \
	2>&1 | c++filt

# Shortcut: build by module name
# Additionally, this target runs the test application after
# a succesful build process. Parameters to this run can be given
# in the variable ARGS.
$(MODULE): $(EXE)
	@echo "*"
	@echo "* Starting test application '$(MODULE)'..."
	@echo "*"
	./$(EXE) $(ARGS)

# ReChannel library depends on phony target to build the lib
$(RECHANNEL2_LIB_DIR)/librechannel.a: ReChannel

# (phony) rule to remake ReChannel library, if necessary
ReChannel:
	@echo "* Rebuild ReChannel library, if necessary..."
	cd $(RECHANNEL2_MAKE_DIR) && $(MAKE) -f Makefile ReChannel

# rule to compile a single source file
.cpp.o:
	$(CXX) $(CFLAGS) \
	    $(INCLUDES) \
	    -o $@ \
	    -c $<

objclean:
	@echo "* Cleaning object files..."
	$(DEL) $(OBJS)

clean: objclean $(EXTRA_CLEAN)
	@echo "* Cleaning executable ..."
	$(DEL) $(EXE)
	@echo "* Cleaning dependency files ..."
	$(DEL) $(DEPS)


# Dependency handling of source files
%.d: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -MM \
	    -MT $(<:.cpp=.o) \
	    -o  $@ \
	    -c  $<

# actually process dependencies
include $(DEPS)

# $Id: Makefile,v 1.2 2007/10/07 23:09:58 felke Exp $