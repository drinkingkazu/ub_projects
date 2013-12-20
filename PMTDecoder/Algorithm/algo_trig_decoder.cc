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

  // If in back_trace mode, add this word in record
  if(_bt_mode){

    // Check if buffer is filled
    if(!_bt_nwords_filled)
      _bt_nwords_filled = (_bt_nwords == _bt_words.size());

    // If filled, remove the oldest element
    if(_bt_nwords_filled)
      _bt_words.pop_front();

    // Add new word
    _bt_words.push_back(word);

  }

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
    if(_trigger_words[(TRIGGER_WORD_COUNT - 1)] != TRIGGER_LAST_WORD) {
      
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form(" Invalid end-of-trigger-data word: %x (event %d)", 
			 _trigger_words[_trigger_word_count-1],_last_event_id));

      status = false;

      if(_debug_mode) {

	std::string msg("Currently in memory: ");
	
	for(size_t i=0; i<TRIGGER_WORD_COUNT; ++i) {

	  msg += Form("%08x ",_trigger_words[i]);

	  if( (i+1) < TRIGGER_WORD_COUNT)

	    _trigger_words[i]=_trigger_words[i+1];

	}
	
	Message::send(MSG::WARNING,__FUNCTION__,
		      Form("%s ...  Continue by shifting a data word...",msg.c_str()));
	
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
    std::ostringstream msg;
    msg << std::endl << "Decoded words: ";
    for(size_t i=0; i<TRIGGER_WORD_COUNT; ++i)
      msg << Form("0x%08x ",trigger_words[i]) ;
    msg 
      << std::endl
      << Form("Trigger id : %d",_data->trig_id()) << std::endl
      << Form("Frame  id  : %d",_data->trig_frame_id()) << std::endl
      << Form("Sample id  : %d",_data->trig_timeslice()) << std::endl
      << Form("PC     : %d",_data->trig_pc()) << std::endl
      << Form("EXT    : %d",_data->trig_ext()) << std::endl
      << Form("Active : %d",_data->active()) << std::endl
      << Form("Gate 1 : %d",_data->gate1()) << std::endl
      << Form("Gate 2 : %d",_data->gate2()) << std::endl
      << Form("Veto   : %d",_data->veto_in()) << std::endl
      << Form("Calib  : %d",_data->calib()) << std::endl
      << std::endl;
    Message::send(MSG::INFO,__FUNCTION__,msg.str());
  }

  if(_data->trig_id() && _data->trig_id() != (_last_event_id+1))

    Message::send(MSG::WARNING,__FUNCTION__,
		  Form("Processed event %d while the last event ID was %d ... missing %d!",
		       _data->trig_id(),
		       _last_event_id,
		       (_last_event_id+1)));

  _last_event_id = _data->trig_id();

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
