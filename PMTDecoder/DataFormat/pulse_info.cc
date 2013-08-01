#ifndef PULSE_INFO_CC
#define PULSE_INFO_CC

#include "pulse_info.hh"

void pulse_info::clear_data() {

  _channel_number=PMT::INVALID_CH;
  _ped_mean=-1;
  _ped_rms=-1;
  _charge=-1;
  _start_time=-1;
  _end_time=-1;
  _max_time=-1;

}

//////////////////////////////////////////////////////

void pulse_collection::clear_data(){

  clear();
  _sum_charge=-1;
  _sum_peak=-1;
  _npulse=0;

}

void pulse_collection::calculate_sum(){

  std::vector<pulse_info>::const_iterator iter(this->begin());
  _sum_charge=0;
  _sum_peak=0;
  _npulse=0;
  while(iter!=this->end()){

    _sum_charge+=(*iter).charge();
    _sum_peak+=(*iter).pulse_peak();
    _npulse++;
  }

}

#endif 
