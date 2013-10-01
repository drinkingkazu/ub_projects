#ifndef PULSE_SELECTOR_BASE_CC
#define PULSE_SELECTOR_BASE_CC

#include "pulse_selector_base.hh"

//################################################
pulse_selector_base::pulse_selector_base(){
//################################################
  
  _name="pulse_selector_base"; 
  
  _fout=0; 

  _pulse_type=DATA_STRUCT::PULSE_COLLECTION;

}

//################################################
void pulse_selector_base::set_pulse_type(DATA_STRUCT::DATA_TYPE type) {
//################################################

  switch(type){

  case DATA_STRUCT::PMT_WF_COLLECTION:
  case DATA_STRUCT::TPC_WF_COLLECTION:
  case DATA_STRUCT::TRIG_INFO:
  case DATA_STRUCT::USER_COLLECTION:
  case DATA_STRUCT::DATA_TYPE_MAX:
    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("The specified data type (=%ud) does not match any pulse data type!",type)
		  );
    break;
  case DATA_STRUCT::PULSE_COLLECTION:
  case DATA_STRUCT::FIXED_WIN_PULSE_COLLECTION:
  case DATA_STRUCT::THRES_WIN_PULSE_COLLECTION:
  case DATA_STRUCT::SLIDE_WIN_PULSE_COLLECTION:
    _pulse_type = type;
    break;
  }

}

#endif
