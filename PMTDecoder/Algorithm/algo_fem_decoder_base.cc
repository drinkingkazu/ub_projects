#ifndef ALGO_FEM_DECODER_BASE
#define ALGO_FEM_DECODER_BASE

#include "algo_fem_decoder_base.hh"

//#################################################
algo_fem_decoder_base::algo_fem_decoder_base() 
  : algo_base() 
//#################################################
{
  _name = "algo_fem_decoder_base";
  reset();
}


//#################################################
void algo_fem_decoder_base::reset() {
//#################################################

  algo_base::reset();
  
  _process               = READ_HEADER;

  _last_word             = PMT::INVALID_WORD;

  _event_header_count    = 0;

  _search_for_next_event = false;

  for(size_t i=0; i< EVENT_HEADER_COUNT; i++)

    _event_header_words[i] = 0;

}


//#################################################
bool algo_fem_decoder_base::process_word(PMT::word_t word)
//#################################################
{

  // 
  // A simple function to call other members depending on the status.
  // Let other members to deal with expected/unexpected case
  //

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

  bool status=true;
  PMT::word_t word_class=get_word_class(word);

  // Check if _search_for_next_event flag is on or not.
  // If it is on, it is OK to simply skip this word. 
  if(_search_for_next_event && word_class!=PMT::EVENT_FIRST_HEADER){

    if(_verbosity[MSG::WARNING]){
      
      Message::send(MSG::WARNING,__FUNCTION__,
		    Form("Skipping till next event (debug mode): %x",word));
      
    }

    return status;
  }

  switch(_process){

  case READ_HEADER:
    //
    // Exceptional case handling: if word type is NOT event header
    //
    if(word_class!=PMT::EVENT_HEADER && 
       word_class!=PMT::EVENT_FIRST_HEADER) {
      
      // Call an exception handling method. 
      // This method should be implemented in children class, and handle whatever needs to be done.
      // The return boolean decides what to do for further processing (false = terminate)
      status = handle_unexpected_ch_word(word, _last_word);

    }else{

      //
      // hand over 32-bit word as it is for processing event header
      //
      status=process_event_header(word, _last_word);
      
      // If status return of processing header is false, handle the case
      if(!status) 
	
	status = handle_failure_header(word,_last_word);
    }

    break;

  case READ_CHANNEL:
    //
    // Exceptional case handling: if word type is event header,
    //
    if(word_class == PMT::EVENT_HEADER ||
       word_class == PMT::EVENT_FIRST_HEADER) {

      // Warn a user & process it as a header
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Found an event header (%x) while looping over channel data (last word: %x)!",
			 word,_last_word));

      // Call an exception handling method. 
      // This method should be implemented in children class, and handle whatever needs to be done.
      // The return boolean decides what to do for further processing (false = terminate)
      status = handle_unexpected_header(word, _last_word);
      
    }
    else if(word_class == PMT::FEM_LAST_WORD) 

      // Special operation @ end of FEM data stream (32-bit word operation)
      status = process_fem_last_word(word, _last_word);

    else if(word_class == PMT::EVENT_LAST_WORD)
      
      // Special operation @ end of event data stream (32-bit word operation)
      status = process_event_last_word(word, _last_word);

    else {
      //
      // Everything else is 16-bit word operation that is to be done
      // by process_ch_word() method (children implement what-to-do).
      //

      // Split two 16 bit words
      PMT::word_t first_word  = word & 0xffff;
      PMT::word_t second_word = word >> 16;

      //
      // Process the 1st word
      //
      status = process_ch_word(first_word, _last_word);

      // If return is false, handle the failure
      if(!status)
	
	status = handle_failure_ch_word(first_word, _last_word);

      //
      // If status return is alright and _process is still READ_CHANNEL, process the 2nd word
      //
      if(status && _process == READ_CHANNEL) {

	status = process_ch_word(second_word, _last_word);

      // If return is false, handle the failure
      if(!status)
	
	status = handle_failure_ch_word(first_word, _last_word);

      }

      break;
    }

    break;
  }

  return status;

}


//#################################################
bool algo_fem_decoder_base::process_event_header(const PMT::word_t word, PMT::word_t &last_word) {
//#################################################
  
  bool status=true;
  //
  // Check if this is an event header word or not
  //
  PMT::PMT_WORD word_class=get_word_class(word);
  if(_verbosity[MSG::DEBUG]){
    sprintf(_buf,"Processing Header: %x",word);
    Message::send(MSG::DEBUG,__FUNCTION__,_buf);
  }
  switch(word_class){

    // Undefined word
  case PMT::UNDEFINED_WORD:

    Message::send(MSG::WARNING,__FUNCTION__,
		  Form("Found Undefined word while searching an event header: %x",word));

    break;

    // Non-event-header word
  case PMT::FIRST_WORD:
  case PMT::CHANNEL_HEADER:
  case PMT::CHANNEL_WORD:
  case PMT::CHANNEL_LAST_WORD:
  case PMT::FEM_LAST_WORD:
  case PMT::EVENT_LAST_WORD:

    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Encountered unexpected word while an event header search: %x (word type=%d)",
		       word,word_class) );
    
    status = false;

    break;

    // Header word -> store & process
  case PMT::EVENT_FIRST_HEADER:

    // This is just a marker. Nothing really to be done.
    // Unflagg _search_for_next_event in case it is true (we no longer skip anything as a new header just found)
    _search_for_next_event = false;

    break;

  case PMT::EVENT_HEADER:
    // Event header should come as a 32-bit word which is a pair of two 16-bit header words.
    // The first 16-bit is already checked by this point. Check the second part.
    if(get_word_class(word>>16)!=PMT::EVENT_HEADER) {

      Message::send(MSG::ERROR,__FUNCTION__,Form("Found an odd event header word: %x",word));

      status = false;

      break;

    }

    // Process the subject word as an event header
    if (_event_header_count<EVENT_HEADER_COUNT) {

      // Store header words
      _event_header_words[_event_header_count]=word;
      _event_header_count++;

      // If stored number of header words reached to the expected number, decode.
      if(_event_header_count==EVENT_HEADER_COUNT) {
	
	// Decode header words
	status = decode_event_header(_event_header_words);

	// Check the return status. If success, set the header count to 0 & change process status
	if(status){

	  _event_header_count=0;

	  _process=READ_CHANNEL;

	}
      }
    }
    else {
      // Raise error if a header word count > set constant (should not happen)
      Message::send(MSG::ERROR,__FUNCTION__,
		    "Logic error: event header word counter not working!");
      status=false;
    }
    break;
  }

  // Update the last word if status is true
  if(status) last_word = word;    

  return status;  

}



//#################################################
bool algo_fem_decoder_base::handle_failure_header(const PMT::word_t word, 
						  PMT::word_t &last_word)
{
//#################################################
  //
  // In this base class, we simply set the algorithm to skip any word till it finds a new
  // event header ONLY IF _debug_mode is set.
  //

  bool status = false;

  if(_debug_mode){ 

    status = true;
    
    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Failed processing the word %x (last word %x) as an event header!",word,last_word));

    _process = READ_HEADER;

    _search_for_next_event = true;

  }

  return status;  

}

//#################################################
bool algo_fem_decoder_base::handle_unexpected_ch_word(const PMT::word_t word,
						      PMT::word_t &last_word)
{
//#################################################

  //
  // Ignore this word, continue to the next event if in _debug_mode
  //
  bool status = false;

  if(_debug_mode){

    status = true;

    _process = READ_HEADER;

    _search_for_next_event = true;

  }

  return status;

}


//#################################################
bool algo_fem_decoder_base::decode_event_header(const PMT::word_t *event_header){
//#################################################

  bool status=true;
  //
  // Get event information
  //

  // (1) check if the very first 16-bit word is as expected
  if(!( event_header[0] & 0xffff))
    Message::send(MSG::ERROR,"Unexpected first word in event headers!");

  if(!status) return status;

  // Initialize data holder
  _header_info.reset();

  // (2) get module address ... lowest 5 of 12 bits
  _header_info.module_address = ( (event_header[0]>>16 & 0xfff)    & 0x1f);

  // (3) get module ID number ... upper 7 bit of 12 bits
  _header_info.module_id      = ( (event_header[0]>>16 & 0xfff)>>5 & 0x7f);

  // (4) get number of 16-bit words to be read in this event.
  // Lower 12 bits of two 16-bit words.
  // The very last "last word for channel info" is not included in this.
  // For later checking purpose, increment by 1.
  _header_info.nwords         = ( (((event_header[1]>>16) & 0xfff) + ((event_header[1] & 0xfff)<<12)));

  // (5) get event ID
  // Lower 12 bits of two 16-bit words.
  _header_info.event_id       = ( (((event_header[2]>>16) & 0xfff) + ((event_header[2] & 0xfff)<<12)));

  // (6) get frame ID
  // Lower 12 bits of two 16-bit words.
  _header_info.event_frame_id = ( (((event_header[3]>>16) & 0xfff) + ((event_header[3] & 0xfff)<<12)));

  // (7) get checksum
  _header_info.checksum       = ( (((event_header[4]>>16) & 0xfff) + ((event_header[4] & 0xfff)<<12)));


#ifdef INCLUDE_EXTRA_HEADER
  _header_info.trigger_frame_id  = ((((event_header[5]>>16) & 0xfff)>>4 & 0xf) +
				    (((_header_info.event_frame_id)>>4)<<4)); 

  // Correct for a roll over
  _header_info.trigger_frame_id  = round_diff(_header_info.event_frame_id, _header_info.trigger_frame_id, 0xf);

  _header_info.trigger_timeslice = ((((event_header[5]>>16) & 0xf)<<8) + (event_header[5] & 0xff));

#endif

  // Report if verbosity is set.
  if(_verbosity[MSG::INFO])
    {
      Message::send(MSG::INFO,Form("Module %d (ID=%d)", _header_info.module_address, _header_info.module_id));
      Message::send(MSG::INFO,Form("Event ID %d",_header_info.event_id));
      Message::send(MSG::INFO,Form("Frame ID %d",_header_info.event_frame_id));
      Message::send(MSG::INFO,Form("Number of Words = %d",_header_info.nwords));
      Message::send(MSG::INFO,Form("Checksum = %x", _header_info.checksum));
      Message::send(MSG::INFO,Form("Trigger Frame %d",_header_info.trigger_frame_id));
      Message::send(MSG::INFO,Form("Trigger Sample %d",_header_info.trigger_timeslice));
    }

  _checksum=_header_info.checksum;

  _nwords=_header_info.nwords;
		      
  return status;
}


//#################################################
PMT::word_t algo_fem_decoder_base::round_diff(PMT::word_t ref_id, 
					      PMT::word_t subject_id, 
					      PMT::word_t diff) const
//#################################################
{
  // Used to recover pmt/trigger frame id from roll over effect.
  // One can test this by simply calling this function.
  // For instance, to test the behavior for a roll-over of 0x7 ...
  //
  // > root
  // root[0] gSystem->Load("libDecoder")
  // root[1] algo_slow_readout_decoder k
  // root [6] k.get_pmt_frame(583,584,0x7)
  // (const unsigned int)584
  // root [7] k.get_pmt_frame(584,583,0x7)
  // (const unsigned int)583
  //
  // I think this implementation works. ... Aug. 12 2013
  if( (subject_id > ref_id) && ((subject_id-ref_id) >= diff) )
    return subject_id - (diff+1);
  else if( (ref_id > subject_id) && ((ref_id-subject_id) >= diff) )	   
    return subject_id + (diff+1);
  else
    return subject_id;

  // Following part is provided by David & Georgia for a specific implementation for PMT... Aug. 12 2013
  // But I don't think this works right. 
  // -Kazu Aug. 12 2013
  /*
  PMT::word_t correct_frame= ( ( event_frame_id & (~0x7) ) | ( channel_frame_id & 0x7 ) );
  if( (correct_frame-event_frame_id)<-1 )
    return correct_frame+8;
  else if( (correct_frame-event_frame_id)>2 )
    return correct_frame-8;
  else 
    return correct_frame;
  */

}


#endif
