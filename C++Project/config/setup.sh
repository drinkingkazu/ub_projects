#!/bin/bash

if [[ -z $MAKE_TOP_DIR ]]; then
    echo \$MAKE_TOP_DIR not set! 
    echo You have to set this first.
else
    if [[ -z $ROOTSYS ]]; then
	case `uname -n` in
	    (*nevis.columbia.edu)
	    if [[ -z ${ROOTSYS} ]]; then
		source /usr/nevis/adm/nevis-init.sh
		setup root
	    fi
	    ;;
	    (*cmsaf*)
	    if [[ -z ${ROOTSYS} ]]; then
		export ROOTSYS=/app/d-Chooz/Software/root/root_v5.28.00
	    fi
	    ;;
	    (*)
	    echo system not recognized!!! 
	    ;;
	esac
    fi
    if [[ -z $ROOTSYS ]]; then
	echo Failed to set up ROOTSYS. Contact kazuhiro@nevis.columbia.edu
	echo
    else
	export LD_LIBRARY_PATH=$MAKE_TOP_DIR/lib:$LD_LIBRARY_PATH
	export DYLD_LIBRARY_PATH=$MAKE_TOP_DIR/lib:$DYLD_LIBRARY_PATH
	export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH
    fi
fi
