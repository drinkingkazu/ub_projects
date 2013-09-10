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
# FileIO
rootlibmap libFileIO.rootmap libFileIO.so $PMT_DECODER_DIR/FileIO/LinkDef.h \
    libBase.so 

######################################################
# Algorithm
rootlibmap libAlgorithm.rootmap libAlgorithm.so $PMT_DECODER_DIR/Algorithm/LinkDef.h \
    libBase.so libDataFormat.so 

######################################################
# Decoder
rootlibmap libDecoder.rootmap libDecoder.so $PMT_DECODER_DIR/Decoder/LinkDef.h \
    libBase.so libDataFormat.so libAlgorithm.so libAnalysis.so libFileIO.so

######################################################
# Encoder
rootlibmap libEncoder.rootmap libEncoder.so $PMT_DECODER_DIR/Encoder/LinkDef.h \
    libBase.so libDataFormat.so libAlgorithm.so libAnalysis.so libFileIO.so

######################################################
# Pulse Reco
rootlibmap libPulseReco.rootmap libPulseReco.so $PMT_DECODER_DIR/PulseReco/LinkDef.h \
    libBase.so libDataFormat.so libAnalysis.so




