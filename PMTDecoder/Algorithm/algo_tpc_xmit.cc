#ifndef ALGO_TPC_XMIT
#define ALGO_TPC_XMIT

#include "algo_tpc_xmit.hh"

//#########################################
algo_tpc_xmit::algo_tpc_xmit() 
  : algo_fem_decoder_base() 
{
//#########################################

  reset();

}

//#########################################
void algo_tpc_xmit::reset() {
//#########################################

  _ch_data.clear_data();

  algo_fem_decoder_base::reset();

}



//#########################################
bool algo_tpc_xmit::process_word(PMT::word_t word) {
//#########################################

  
  // If data pointer is not set, set
  if(!_event_data) {
    _event_data=(tpc_wf_collection*)(_storage->get_data(DATA_STRUCT::TPC_WF_COLLECTION));
    if(!_event_data) {
      Message::send(MSG::ERROR,__FUNCTION__,"Could not retrieve tpc_wf_collection poitner!");
      return false;
    }
  }

  return algo_fem_decoder_base::process_word(word);

}

//#########################################
bool algo_tpc_xmit::process_ch_word(const PMT::word_t word, 
				    PMT::word_t &last_word) 
{
//#########################################

  bool status=true;

  if(_verbosity[MSG::DEBUG]){
    sprintf(_buf,"Processing ch-word: %x",word);
    Message::send(MSG::DEBUG,__FUNCTION__,_buf);
  }

  PMT::PMT_WORD word_class=get_word_class(word);
  PMT::PMT_WORD last_word_class=get_word_class(last_word);
  
  switch(word_class){

  case PMT::CHANNEL_HEADER:
    
    //
    // Check if the last word was event header or channel last word
    //
    if(last_word_class == PMT::FEM_HEADER ||
       last_word_class == PMT::CHANNEL_LAST_WORD ) {

      // New data starts here. 
      // Clear data content & assign channel number.
      _ch_data.clear_data();

      _ch_data.set_channel_number( (word & 0xfff) );
      
      if(_verbosity[MSG::DEBUG])

	Message::send(MSG::DEBUG,__FUNCTION__,
		      Form("New channel header: %d",_ch_data.channel_number()));
      
    }else{

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Unexpected channel header (%x)! Last word = %x",word,last_word));

      status = false;

    }

    break;

  case PMT::CHANNEL_LAST_WORD:

    //
    // Check if the last word was channel word
    //
    if(last_word_class == PMT::CHANNEL_WORD) {

      // Check if the channel number in this ch-last-word agrees with that in the header
      PMT::ch_number_t ch( (word & 0xfff) );

      if(ch == _ch_data.channel_number()) {

	if(_verbosity[MSG::DEBUG])

	  Message::send(MSG::DEBUG,__FUNCTION__,
			Form("Finished reading %zu samples for Ch %d",
			     _ch_data.size(),_ch_data.channel_number()));
	
	// Store
	_event_data->push_back(_ch_data);
	
	_ch_data.clear_data();

      }else{

	Message::send(MSG::ERROR,__FUNCTION__,
		      Form("Ch. number disagreement! Header: %u ... Last: %u!",
			   _ch_data.channel_number(),ch) );
	
	status = false;

      }

    }
    else{

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Unexpected channel ending (%x)! Last word = %x",word,last_word));
      
      status = false;

    }

    break;

  case PMT::CHANNEL_WORD:

    //
    // Check if the last word was channel header or channel adc word
    //
    if(last_word_class == PMT::CHANNEL_HEADER ||
       last_word_class == PMT::CHANNEL_WORD){

      status = decode_ch_word(word,last_word);

    }else{

      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Unexpected channel word (%x)! Last word = %x",word,last_word));

      status = false;

    }

    break;

  case PMT::EVENT_LAST_WORD:

    store_event();

    _process = READ_HEADER;

    break;

  case PMT::FEM_LAST_WORD:
  case PMT::UNDEFINED_WORD:
  case PMT::EVENT_HEADER:
  case PMT::FEM_HEADER:
  case PMT::FEM_FIRST_WORD:

    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Unexpected word found while channel word processing: %x",word));

    status = false;

    break;
  }

  // If processing of this word is successful, add it to the checksum
  if(status) {
    _nwords--;
    _checksum -= word;
    last_word = word;
  }

  return status;
}

//#########################################################
bool algo_tpc_xmit::check_event_quality(){
//#########################################################

  bool status = true;

  // In case of TPC, nwords & checksum include event header
  for(size_t i=0; i<FEM_HEADER_COUNT; i++){

    _nwords++;
    _checksum += _event_header_words[i];

  }
  
  // Check if _checksum and _nwords agrees with that of event header.
  if(_nwords != _header_info.nwords) {

    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Disagreement on nwords: counted=%u, expected=%u",_nwords,_header_info.nwords));

    status = false;

  }

  if(_checksum != _header_info.checksum) {

    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Disagreement on checksum: summed=%u, expected=%u",_checksum,_header_info.checksum));

    status = false;

  }

  return status;

}

//#########################################################
bool algo_tpc_xmit::process_fem_last_word(const PMT::word_t word,
						  PMT::word_t &last_word)
{
//#########################################################

  if(_verbosity[MSG::INFO]){
    
    Message::send(MSG::INFO,__FUNCTION__,
		  Form("End of FEM word: %x...",word));
  }
  last_word = word;
  return true;
}

//#########################################################
bool algo_tpc_xmit::process_event_last_word(const PMT::word_t word,
						    PMT::word_t &last_word)
{  
//#########################################################

  if(_verbosity[MSG::INFO]){
      
    Message::send(MSG::INFO,__FUNCTION__,
		  Form("End of event word: %x...",word));
    
  }
  last_word = word;
  _process  = READ_HEADER;
  return store_event();
}


//#########################################################
void algo_tpc_xmit::clear_event(){
//#########################################################

  init_checker_info();

  _event_data->clear_data();

  _ch_data.clear();

}

//#########################################################
bool algo_tpc_xmit::store_event(){
//#########################################################
 
  bool status = check_event_quality();

  // Store if condition is satisfied
  if(status) {

    if(_verbosity[MSG::INFO]){

      Message::send(MSG::INFO,__FUNCTION__,
		    Form("Storing event %u with %zu channel entries...",
			 _event_data->event_id(), _event_data->size()));

    }

    status = _storage->next_event();

  }else{
    
    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Skipping to store event %d...",_header_info.event_id));

  }

  clear_event();

  return status;
}

//#########################################################
bool algo_tpc_xmit::handle_unexpected_header (const PMT::word_t word, 
					      PMT::word_t &last_word){
//#########################################################

  if(!_debug_mode) return false;

  //
  // if in debug mode, we continue to process this as the next event header
  // Attempt to store event data
  //

  bool status = store_event();

  _process = READ_HEADER;
  
  status = process_event_header(word,last_word);

  if(!status) status = handle_failure_header(word, last_word);

  return status;

}

//#########################################################
bool algo_tpc_xmit::handle_failure_ch_word (const PMT::word_t word, 
					    PMT::word_t &last_word){
//#########################################################
  
  // Raise warning
  Message::send(MSG::ERROR,__FUNCTION__,
		Form("Failed processing ch-word: %x (previous %x)!",word,_last_word));
  
  //
  // if in debug_mode, let it continue
  //
  return _debug_mode;

}

//#########################################################
bool algo_tpc_xmit::decode_ch_word(const PMT::word_t word, 
				   PMT::word_t &last_word)
{
//#########################################################

  bool status = true;
  // Simply append if it is not compressed
  if( !(is_compressed(word)) ) _ch_data.push_back( (word & 0xfff) );

  else if(!(_ch_data.size())){

    // This is a problem: if huffman coded, then we must have a previous ADC sample
    // as a reference. Raise an error.

    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Huffman coded word %x found while the previous was non-ADC word (%x)!",
		       word,last_word));
    
    status = false;

  }
  else{

    

    // Compresed data is in last 15 bit of this word.
    PMT::word_t data = (word & 0x7fff);

    // The compression is based on an extremely simple Huffman encoding.
    // The Huffman tree used here assigns the following values:
    //
    // Value   Code
    //  +3     0000001
    //  +2     00001
    //  +1     001
    //  +0     1
    //  -1     01
    //  -2     0001
    //  -3     000001

    size_t zero_count = 0;
    for(size_t index=0; index<15 && status; ++index){

      if( !((data >> index) & 0x1) ) zero_count++;
      else{

	switch(zero_count){
	  
	case 0:
	  _ch_data.push_back( (*(_ch_data.rbegin())) ); break;	  
	  
	case 1:
	  _ch_data.push_back( (*(_ch_data.rbegin())) -1 ); break;

	case 2:
	  _ch_data.push_back( (*(_ch_data.rbegin())) +1 ); break;

	case 3:
	  _ch_data.push_back( (*(_ch_data.rbegin())) -2 ); break;

	case 4:
	  _ch_data.push_back( (*(_ch_data.rbegin())) +2 ); break;

	case 5:
	  _ch_data.push_back( (*(_ch_data.rbegin())) -3 ); break;

	case 6:
	  _ch_data.push_back( (*(_ch_data.rbegin())) +3 ); break;

	default:
	  Message::send(MSG::ERROR,__FUNCTION__,
			Form("Encountered unexpected number of zeros (=%zu) in the compression!",zero_count));
	  status = false;
	};
	
	zero_count=0;
      }
    }
  }
  
  return status;

}

#endif
