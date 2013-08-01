#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = EncoderCint
SHLIB = libEncoder.so
SOURCES = $(wildcard *.cc)
HEADERS = $(wildcard *.hh)
OBJECTS = $(SOURCES:.cc=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += -I$(MAKE_TOP_DIR)/Base
INCFLAGS += -I$(MAKE_TOP_DIR)/DataFormat
INCFLAGS += -I$(MAKE_TOP_DIR)/FileIO
INCFLAGS += -I$(MAKE_TOP_DIR)/Analysis

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# set compiler options for ROOT
CXXFLAGS +=  `root-config --cflags`

# call the common GNUmakefile
include $(MAKE_TOP_DIR)/Makefile/GNUmakefile.CORE



