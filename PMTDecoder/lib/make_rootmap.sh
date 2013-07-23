#!/bin/bash

temp=make_rootmap.$$.temp
rm -f $temp

rootlibmap() {
 ROOTMAP=$1
 SOFILE=$2
 LINKDEF=$3
 shift 3
 DEPS=$*
 if [[ -e $SOFILE && -e $LINKDEF ]]; then
     rlibmap -f -o $ROOTMAP -l $SOFILE -d $DEPS -c $LINKDEF 2>> $temp
 fi
}

######################################################
# "simple" example package
rootlibmap libsimple.rootmap libsimple.so $MAKE_TOP_DIR/simple/LinkDef.h 

######################################################
# Base
rootlibmap libBase.rootmap libBase.so $MAKE_TOP_DIR/Base/LinkDef.h 

######################################################
# DataFormat
rootlibmap libDataFormat.rootmap libDataFormat.so $MAKE_TOP_DIR/DataFormat/LinkDef.h \
    libTree.so libBase.so

######################################################
# FileIO
rootlibmap libFileIO.rootmap libFileIO.so $MAKE_TOP_DIR/FileIO/LinkDef.h \
    libBase.so 

######################################################
# Analysis
rootlibmap libAnalysis.rootmap libAnalysis.so $MAKE_TOP_DIR/Analysis/LinkDef.h \
    libGeom.so libGui.so libHist.so  libBase.so libDataFormat.so 

######################################################
# Algorithm
rootlibmap libAlgorithm.rootmap libAlgorithm.so $MAKE_TOP_DIR/Algorithm/LinkDef.h \
    libBase.so libDataFormat.so 

######################################################
# Decoder
rootlibmap libDecoder.rootmap libDecoder.so $MAKE_TOP_DIR/Decoder/LinkDef.h \
    libBase.so libDataFormat.so libAlgorithm.so libAnalysis.so libFileIO.so

######################################################
# Encoder
rootlibmap libEncoder.rootmap libEncoder.so $MAKE_TOP_DIR/Encoder/LinkDef.h \
    libBase.so libDataFormat.so libAlgorithm.so libAnalysis.so libFileIO.so





