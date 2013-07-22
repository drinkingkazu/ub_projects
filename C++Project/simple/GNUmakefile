#
# This is an example GNUmakefile for my packages
#

# Specific names for this package 
DICT  = fooCint
SHLIB = libfoo.so
SOURCES = $(wildcard *.cc)
HEADERS = $(wildcard *.hh)
OBJECTS = $(SOURCES:.cc=.o)

# Include options for this package 
# Add other package path if this depends on them.
INCFLAGS = -I.  #Include itself
# Example: INCFLAGS += -I$(MAKE_TOP_DIR)/OTHER_PACKAGE

# Platform-specific options 
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# set compiler options for ROOT
CXXFLAGS +=  `root-config --cflags`

# call the common GNUmakefile
include $(MAKE_TOP_DIR)/Makefile/GNUmakefile.CORE


