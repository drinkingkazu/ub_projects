#ifndef TPC_WAVEFORM_CC
#define TPC_WAVEFORM_CC

#include "tpc_waveform.hh"
ClassImp(tpc_waveform)

void tpc_waveform::clear_data(){
  data_base::clear_data();
  init_vars();
}

void tpc_waveform::init_vars(){
  _channel_number=PMT::INVALID_CH;
}

#endif
