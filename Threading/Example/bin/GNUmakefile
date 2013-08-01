# Include your header file location
CXXFLAGS += -I$(MAKE_TOP_DIR)/ThreadBase
CXXFLAGS += -I$(MAKE_TOP_DIR)/Example
CXXFLAGS += -I. $(shell root-config --cflags) -g

# Include your shared object lib location
LD := $(subst build/,$(ROOTSYS)/build/,$(LD))
LDFLAGS  += -L$(ROOTSYS)/lib  $(shell root-config --libs) -g

LIBDIR  = $(MAKE_TOP_DIR)/lib
LDFLAGS += -L$(LIBDIR) -lThreadBase -lExample

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = test

all:		$(PROGRAMS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)
