#ifndef EVENT_WAVEFORM_CC
#define EVENT_WAVEFORM_CC

#include "event_waveform.hh"

ClassImp(event_waveform)

void event_waveform::clear_data(){
  clear();
  init_vars();
}

void event_waveform::init_vars(){
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

