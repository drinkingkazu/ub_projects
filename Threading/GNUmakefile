#####################################################################################
#
# A top Makefile for building my project.
# One needs to define $MAKE_TOP_DIR environment variable and set it to where this
# makefile exists. 
# One can type "make" and this builds packages that are added in $SUBDIR defined below.
# 
# The original is taken from Glenn A. Smith's example for Double Chooz experiment.
#
#####################################################################################
#
# IMPOSE CONDITION BETWEEN MAKE_TOP_DIR & PWD =>
#   do not compile if PWD !=$MAKE_TOP_DIR is set elsewhere
#
ifndef MAKE_TOP_DIR
 MAKE_TOP_DIR := $(shell cd . && pwd -P)
endif
NORMALIZED_MAKE_TOP_DIR := $(shell cd ${MAKE_TOP_DIR} && pwd -P)
ifneq ($(NORMALIZED_MAKE_TOP_DIR), $(shell cd . && pwd -P))
 ifneq ($(wildcard ./DCDB/*),)
ERROR_MESSAGE := $(error Your source code detected in current dir, but MAKE_TOP_DIR is not current dir.  \
   To avoid recompiling the wrong installation,\
   MAKE_TOP_DIR must be set to the current directory when making.  \
   Currently it is set to ${MAKE_TOP_DIR} [$(NORMALIZED_MAKE_TOP_DIR)].  \
   Current directory is $(shell pwd).)
 endif
endif
export MAKE_TOP_DIR
#
#####################################################################################
#
# Define directories to be compile upon a global "make"...
#
SUBDIRS := ThreadBase Example

#####################################################################################
#
# COMPILATION...
#
#.phony: all configure default-config clean
.phony: all clean

all:
	@for i in $(SUBDIRS); do ( echo "" && echo "Compiling $$i..." && cd $$i && $(MAKE) ) || exit $$?; done
#####################################################################################
#
# CLEANs...
#
clean:
	@for i in $(SUBDIRS); do ( echo "" && echo "Cleaning $$i..." && cd $$i && $(MAKE) clean ) || exit $$?; done
	@rm -f $(MAKE_TOP_DIR)/lib/lib*.so

#####################################################################################
#
# DOCUMENTATION...
#
doxygen:
	@echo 'dOxygenising your code...'
	@mkdir -p $(MAKE_TOP_DIR)/doc/dOxygenMyProject
	@doxygen $(MAKE_TOP_DIR)/doc/doxygenMyProject.script

doxygen+:
	@echo 'dOxygenising MyProject + local-ROOT...'
	@mkdir -p $(MAKE_TOP_DIR)/doc/dOxygenMyProject+
	@doxygen $(MAKE_TOP_DIR)/doc/doxygenMyProject+.script
#
#####################################################################################
#EOF
