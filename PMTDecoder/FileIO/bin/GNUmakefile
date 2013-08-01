
LD := $(subst build/,$(ROOTSYS)/build/,$(LD))
LIBDIR  = $(MAKE_TOP_DIR)/lib
LDFLAGS  += -L$(ROOTSYS)/lib  $(shell root-config --libs) -g

# Include your header file location
CXXFLAGS += -I$(MAKE_TOP_DIR)/Base 
CXXFLAGS += -I$(MAKE_TOP_DIR)/FileIO
CXXFLAGS += -I. $(shell root-config --cflags) -g

# Include your shared object lib location
LDFLAGS += -L$(LIBDIR) -lBase -lFileIO

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = simple_io

all:		$(PROGRAMS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)
