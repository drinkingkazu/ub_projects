#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = ThreadBaseCint
SHLIB = libThreadBase.so
SOURCES = $(wildcard *.cc)
HEADERS = $(wildcard *.hh)
#OBJECTS = $(SOURCES:.cc=.o)

EXECUTABLE = test.o

OBJECTS = $(filter-out $(EXECUTABLE), $(SOURCES:.cc=.o));

# include options for this package
INCFLAGS  = -I.                       #Include itself
#INCFLAGS += -I./usr/include
INCFLAGS += $(shell root-config --cflags)


LDFLAGS += -L$(ROOTSYS)/lib $(shell root-config --libs) -g -lpthread

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

# set compiler options for ROOT
#CXXFLAGS +=  -I. $(shell root-config --cflags) -g

include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(MAKE_TOP_DIR)/Makefile/GNUmakefile.CORE

LIBS += $(shell root-config --libs) -g -lpthread
LIBS += -L$(MAKE_TOP_DIR)/lib -lThreadBase

all: $(EXECUTABLES)

test: lib/$(SHLIB) test.o
	@echo "<< compiling executable test >>"
	@$(CXX) -g $(CXXFLAGS) -o test $^ $(LIBS)


