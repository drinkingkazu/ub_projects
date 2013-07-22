#ifndef TRIGPULSER_CC
#define TRIGPULSER_CC

#include "TrigPulser.hh"

TrigPulser::TrigPulser(){

  InitD();

}

void TrigPulser::InitD(){

  _p1_delay=0;
  _p2_delay=0;

  _p1_width=0;
  _p2_width=0;
  
  _trig_pulse_delay=0;
  _npulse=1;

  _sleep_after_prep=10;
  _sleep_after_pulse=10000;

  _dev_ptr=0;
  _module=0;

  for(short iter=0; iter<PCIE::BUFF_SEND_LENGTH; iter++)
    _buf_send[iter]=0;

  _ready=false;

}

bool TrigPulser::Prepare(){

  if(_ready) return _ready;
  if(!_dev_ptr) return false;
  if(!_module) return false;

  bool status=false;

  // set up p1 delay;
  _buf_send[0]=((_module->trigger_id)<<11) + (mb_trig_p1_delay) + ((_p1_delay)<<16); 
  status = pcie_send(*_dev_ptr, 1, 1, (uint32_t*)(&_buf_send));

  // set up p1 width;  
  _buf_send[0]=((_module->trigger_id)<<11) + (mb_trig_p1_width) + ((_p1_width)<<16);
  status = pcie_send(*_dev_ptr, 1, 1, (uint32_t*)(&_buf_send));

  // set up p1 delay;  
  _buf_send[0]=((_module->trigger_id)<<11) + (mb_trig_p2_delay) + ((_p2_delay)<<16);
  status = pcie_send(*_dev_ptr, 1, 1, (uint32_t*)(&_buf_send));
  
  // set up p1 width;
  _buf_send[0]=((_module->trigger_id)<<11) + (mb_trig_p2_width) + ((_p2_width)<<16);
  status = pcie_send(*_dev_ptr, 1, 1, (uint32_t*)(&_buf_send));
  
  // set up triger delay;
  _buf_send[0]=((_module->trigger_id)<<11) + (mb_trig_pulse_delay) + ((_trig_pulse_delay)<<16);
  status = pcie_send(*_dev_ptr, 1, 1, (uint32_t*)(&_buf_send));

  usleep(_sleep_after_prep);

  if(status)
    _ready=true;

  return _ready;
}

bool TrigPulser::Fire(){

  bool status=true;
  if(!_ready) 
    if(!Prepare()) return false;


  for(size_t iter=0; iter<_npulse; iter++){

    if(!Prepare()) 
      {
	status = false;
	break;
      }
    
    _buf_send[0]= ((_module->trigger_id)<<11) + (mb_trig_pulse1) + ((0x1)<<16); // fire pulse 1;
    status = pcie_send(*_dev_ptr, 1, 1, (uint32_t*)(&_buf_send));

    usleep(_sleep_after_pulse);

  }
  
  _ready=false;
  return status;
}

#endif
