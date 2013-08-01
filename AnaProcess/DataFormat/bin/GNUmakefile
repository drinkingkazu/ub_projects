
LD := $(subst build/,$(ROOTSYS)/build/,$(LD))
LIBDIR  = $(MAKE_TOP_DIR)/lib
LDFLAGS  += -L$(ROOTSYS)/lib  $(shell root-config --libs) -g

# Include your header file location
CXXFLAGS += -I$(MAKE_TOP_DIR)/Base 
CXXFLAGS += -I$(MAKE_TOP_DIR)/DataFormat
CXXFLAGS += -I. $(shell root-config --cflags) -g

# Include your shared object lib location
LDFLAGS += -L$(LIBDIR) -lBase -lDataFormat

# Add your program below with a space after the previous one.
# This makefile compiles all binaries specified below.
PROGRAMS = simple_io simple_loop

all:		$(PROGRAMS)
	@rm -rf *.dSYM
clean:	
	rm -f $(PROGRAMS)
