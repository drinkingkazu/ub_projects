#ifndef ALGO_TRIG_DECODER_CC
#define ALGO_TRIG_DECODER_CC

#include "algo_trig_decoder.hh"

algo_trig_decoder::algo_trig_decoder() : algo_base() {
  _name="algo_trig_decoder"; 
  _data=0;
  reset();
}


bool algo_trig_decoder::process_word(PMT::word_t word) {

  bool status=true;
  /*
  if(!_nwords){
    // Check if the storage_manager is assigned to store trigger info
    _storage->set_data_to_write(PMT::TRIG_INFO,true);
  }
  */

  if(!_data){
    _data=(trig_info*)(_storage->get_data(DATA_STRUCT::TRIG_INFO));
    if(!_data){
      Message::send(MSG::ERROR,__FUNCTION__,
		    "Could not retrieve writeable trig_info pointer!");
      return false;
    }
  }

  _trigger_words[_trigger_word_count]=word;
    
  _trigger_word_count++;

  if(_trigger_word_count==TRIGGER_WORD_COUNT) {

    // Check that the last word is end-of-packet.
    // If not, spit out warning and shift data word to continue.
    if(_trigger_words[_trigger_word_count-1] != TRIGGER_LAST_WORD) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form(" Invalid end-of-trigger-data word: %x ", _trigger_words[_trigger_word_count-1]));

      status = false;

      if(_debug_mode) {

	Message::send(MSG::WARNING,__FUNCTION__," Continue by shifting a data word...");
	
	for(size_t i=0; i<(TRIGGER_WORD_COUNT-1); ++i)
	  
	  _trigger_words[i]=_trigger_words[i+1];
	
	_trigger_word_count-=1;

      }

    }else{

      decode_trigger_words(_trigger_words);
      
      _storage->next_event();
      
      clear_event_info();
    }
  }

  _nwords+=1;
  _checksum+=word;

  return status;
}

bool algo_trig_decoder::decode_trigger_words(PMT::word_t *trigger_words){

  if(_verbosity[MSG::DEBUG]) {
    sprintf(_buf,"Processing %x : %x : %x ",trigger_words[0],trigger_words[1],trigger_words[2]);
    Message::send(MSG::DEBUG,__FUNCTION__,_buf);
  }
    
  _data->set_trig_timeslice( (trigger_words[0]>>4) & 0xfff );
  _data->set_trig_frame_id( ((trigger_words[1] & 0xff)<<16) + (trigger_words[0]>>16) );
  _data->set_trig_id( (trigger_words[1]>>8) );
  _data->set_pmt_data( trigger_words[2] & 0xff );

  _data->set_trigger_bits( (trigger_words[2]>>8  & 0x1),   // pc
			  (trigger_words[2]>>9  & 0x1),   // ext
			  (trigger_words[2]>>10 & 0x1),   // active
			  (trigger_words[2]>>12 & 0x1),   // gate1
			  (trigger_words[2]>>11 & 0x1),   // gate2
			  (trigger_words[2]>>13 & 0x1),   // veto
			  (trigger_words[2]>>14 & 0x1) ); // calib

  _data->set_reminder_64MHz((trigger_words[2]>>15 & 0x3));
  _data->set_reminder_16MHz((trigger_words[0]>>1  & 0x7));

  if(_verbosity[MSG::INFO]){
    sprintf(_buf,"Decoded trigger id : %d",_data->trig_id());
    Message::send(MSG::INFO,__FUNCTION__,_buf);
    sprintf(_buf,"Frame  id : %d",_data->trig_frame_id());
    Message::send(MSG::INFO,__FUNCTION__,_buf);
    sprintf(_buf,"Sample id : %d",_data->trig_timeslice());    
    Message::send(MSG::INFO,__FUNCTION__,_buf);
  }

  return true;
}

void algo_trig_decoder::reset(){

  init_checker_info();
  clear_event_info();
  algo_base::reset();

}

void algo_trig_decoder::clear_event_info(){

  _trigger_word_count=0;

  for(size_t i=0; i<TRIGGER_WORD_COUNT; ++i)
    _trigger_words[i]=0x0;

}


bool algo_trig_decoder::check_event_quality() {

  return (_trigger_word_count == TRIGGER_WORD_COUNT);
}

#endif
