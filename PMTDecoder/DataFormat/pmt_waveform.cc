#ifndef PMT_WAVEFORM_CC
#define PMT_WAVEFORM_CC

#include "pmt_waveform.hh"
ClassImp(pmt_waveform)

void pmt_waveform::clear_data(){
  clear();
  init_vars();
}

void pmt_waveform::init_vars(){
  _channel_number=PMT::INVALID_CH;
  _channel_frame_id=PMT::INVALID_WORD;
  _disc_id=PMT::DISC_MAX;
  _timeslice=PMT::INVALID_WORD;
}


#endif
