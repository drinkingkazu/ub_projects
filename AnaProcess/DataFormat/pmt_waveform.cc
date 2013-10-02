#ifndef PMT_WAVEFORM_CC
#define PMT_WAVEFORM_CC

#include "pmt_waveform.hh"

void pmt_waveform::clear_data(){
  data_base::clear_data();
  init_vars();
}

void pmt_waveform::init_vars(){
  clear();
  _channel_number=PMT::INVALID_CH;
  _channel_frame_id=PMT::INVALID_WORD;
  _disc_id=PMT::DISC_MAX;
  _timeslice=PMT::INVALID_WORD;
}

void pmt_wf_collection::clear_data(){
  data_base::clear_data();
  init_vars();
}

void pmt_wf_collection::init_vars(){
  clear();
  _event_id=PMT::INVALID_WORD;
  _event_frame_id=PMT::INVALID_WORD;
  _module_address=PMT::INVALID_WORD;
  _module_id=PMT::INVALID_WORD;
  _channel_header_count=PMT::INVALID_WORD;
  _checksum=PMT::INVALID_WORD;
  _nwords=PMT::INVALID_WORD;
  _trigger_frame_id=PMT::INVALID_WORD;
  _trigger_timeslice=PMT::INVALID_WORD;
}

#endif
