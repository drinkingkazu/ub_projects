#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = PMTReadOutCint
SHLIB = libPMTReadOut.so
SOURCES = $(wildcard *.cc)
HEADERS = $(wildcard *.hh)
OBJECTS = $(SOURCES:.cc=.o)

WD_BASE_DIR=/home/ub/WinDriver

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += -I$(MAKE_TOP_DIR)/../PMTDecoder/Base/
#INCFLAGS = -I$(MAKE_TOP_DIR)/Base
INCFLAGS += -I$(WD_BASE_DIR)
INCFLAGS += -I$(WD_BASE_DIR)/include

CFLAGS += -I$(WD_BASE_DIR)
CFLAGS += -I$(WD_BASE_DIR)/include

CXXFLAGS += -I$(WD_BASE_DIR)
CXXFLAGS += -I$(WD_BASE_DIR)/include

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

#################################################
#
#
#
#################################################

# Comment/uncomment to enable/disable debugging code
DEBUG = 1

ifeq ($(DEBUG),1)
	DEBFLAGS = -g -O -DDEBUG 
else
	DEBFLAGS = -O2
endif

ifndef TARGET_CPU
	TARGET_CPU=$(shell uname -m | sed 's/i.86/i386/' | sed 's/ppc/PPC/' | sed 's/ia64/IA64/')
endif

CXXFLAGS += -c

ifeq ("$(TARGET_CPU)", "PPC")
#	CFLAGS += -DPOWERPC
	CXXFLAGS += -DPOWERPC
endif
ifeq ("$(TARGET_CPU)", "IA64")
#	CFLAGS += -DKERNEL_64BIT
	CXXFLAGS += -DKERNEL_64BIT
endif
ifeq ("$(TARGET_CPU)", "PPC64")
#	CFLAGS += -DKERNEL_64BIT
	CXXFLAGS += -DKERNEL_64BIT
	ifndef USER_BITS
		USER_BITS = 64
	endif
#	CFLAGS += -m$(USER_BITS)
	CXXFLAGS += -m$(USER_BITS)
	LFLAGS += -m$(USER_BITS)
endif
ifeq ("$(TARGET_CPU)", "x86_64")
#	CFLAGS += -DKERNEL_64BIT
	CXXFLAGS += -DKERNEL_64BIT
	ifndef USER_BITS
		USER_BITS = 64
	endif
#	CFLAGS += -m$(USER_BITS)
	CXXFLAGS += -m$(USER_BITS)
	LFLAGS += -m$(USER_BITS)
endif

#################################################

include $(MAKE_TOP_DIR)/Makefile/Makefile.${OSNAME}

# set compiler options for ROOT
CXXFLAGS +=  `root-config --cflags`

# call the common GNUmakefile
include $(MAKE_TOP_DIR)/Makefile/GNUmakefile.CORE



