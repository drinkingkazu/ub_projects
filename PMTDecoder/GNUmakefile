#####################################################################################
#
# A top Makefile for building my project.
# One needs to define $PMT_DECODER_DIR environment variable and set it to where this
# makefile exists. 
# One can type "make" and this builds packages that are added in $SUBDIR defined below.
# 
# The original is taken from Glenn A. Smith's example for Double Chooz experiment.
#
#####################################################################################
#
# IMPOSE CONDITION BETWEEN PMT_DECODER_DIR & PWD =>
#   do not compile if PWD !=$PMT_DECODER_DIR is set elsewhere
#
ifndef PMT_DECODER_DIR
 PMT_DECODER_DIR := $(shell cd . && pwd -P)
endif
NORMALIZED_PMT_DECODER_DIR := $(shell cd ${PMT_DECODER_DIR} && pwd -P)
ifneq ($(NORMALIZED_PMT_DECODER_DIR), $(shell cd . && pwd -P))
 ifneq ($(wildcard ./Decoder/*),)
ERROR_MESSAGE := $(error Your source code detected in current dir, but PMT_DECODER_DIR is not current dir.  \
   To avoid recompiling the wrong installation,\
   PMT_DECODER_DIR must be set to the current directory when making.  \
   Currently it is set to ${PMT_DECODER_DIR} [$(NORMALIZED_PMT_DECODER_DIR)].  \
   Current directory is $(shell pwd).)
 endif
endif
export PMT_DECODER_DIR
#
#####################################################################################
#
# Define directories to be compile upon a global "make"...
#
#SUBDIRS := Base DataFormat FileIO Algorithm Analysis Decoder Encoder
SUBDIRS := FileIO Algorithm Decoder Encoder

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
	@rm -f $(PMT_DECODER_DIR)/lib/lib*.so
	@rm -f $(PMT_DECODER_DIR)/lib/lib*.rootmap

#####################################################################################
#
# DOCUMENTATION...
#
doxygen:
	@echo 'dOxygenising your code...'
	@mkdir -p $(PMT_DECODER_DIR)/doc/dOxygenMyProject
	@doxygen $(PMT_DECODER_DIR)/doc/doxygenMyProject.script

doxygen+:
	@echo 'dOxygenising MyProject + local-ROOT...'
	@mkdir -p $(PMT_DECODER_DIR)/doc/dOxygenMyProject+
	@doxygen $(PMT_DECODER_DIR)/doc/doxygenMyProject+.script
#
#####################################################################################
#EOF
