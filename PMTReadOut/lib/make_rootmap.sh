#!/bin/bash
root_libmap() {
 ROOTMAP=$1
 SOFILE=$2
 LINKDEF=$3
 shift 3
 DEPS=$*
 if [[ -e $SOFILE && -e $LINKDEF ]]; then
    rlibmap -f \
    -o \
    $ROOTMAP \
    -l \
    $SOFILE \
    -d \
    $DEPS \
    -c \
    $LINKDEF \
     2>> $temp
 fi
}

######################################################
# "simple" example package
root_libmap libsimple.rootmap libsimple.so $MAKE_TOP_DIR/simple/LinkDef.h 

