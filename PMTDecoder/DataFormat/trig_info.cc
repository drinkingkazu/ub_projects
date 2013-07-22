#ifndef TRIG_INFO_CC
#define TRIG_INFO_CC

#include "trig_info.hh"

void trig_info::clear_data(){
  
  init_vars();

}

trig_info::trig_info(const trig_info &original) : 
  data_base(original),
  _trig_timeslice(original._trig_timeslice),
  _trig_frame_id(original._trig_frame_id),
  _trig_id(original._trig_id),
  _pmt_data(original._pmt_data),
  _trig_pc(original._trig_pc),
  _trig_ext(original._trig_ext),
  _active(original._active),
  _gate1_in(original._gate1_in),
  _gate2_in(original._gate2_in),
  _veto_in(original._veto_in),
  _calib(original._calib),
  _reminder_64MHz(original._reminder_64MHz),
  _reminder_16MHz(original._reminder_16MHz)
{};

void trig_info::init_vars(){

  _trig_timeslice = 0xffff;
  _trig_frame_id  = PMT::INVALID_WORD;
  _trig_id        = PMT::INVALID_WORD;
  _pmt_data       = 0xffff;
  _trig_pc        = false;
  _trig_ext       = false;
  _active         = false;
  _gate1_in       = false;
  _gate2_in       = false;
  _veto_in        = false;
  _calib          = false;
  _reminder_16MHz = 0xffff;
  _reminder_64MHz = 0xffff;
}

#endif
