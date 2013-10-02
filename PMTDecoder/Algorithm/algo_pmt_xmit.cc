#ifndef ALGO_PMT_XMIT
#define ALGO_PMT_XMIT

#include "algo_pmt_xmit.hh"

//#########################################
algo_pmt_xmit::algo_pmt_xmit() 
  : algo_fem_decoder_base() 
{
//#########################################

  reset();

}

//#########################################
void algo_pmt_xmit::reset() {
//#########################################

  _event_data = 0;

  clear_event();

  algo_fem_decoder_base::reset();

}


//#########################################
void algo_pmt_xmit::clear_event() {
//#########################################

  algo_fem_decoder_base::clear_event();

  if(_event_data)

    _event_data->clear_data();
  
  _ch_data.clear_data();
  
  _channel_header_count = 0;

  _last_channel_number=PMT::INVALID_CH;
  _last_disc_id=PMT::DISC_MAX;

}

//#########################################
bool algo_pmt_xmit::process_ch_word(const PMT::word_t word, 
				    PMT::word_t &last_word) 
{
//#########################################

  bool status=true;

  if(_verbosity[MSG::DEBUG]){
    sprintf(_buf,"Processing ch-word: %x",word);
    Message::send(MSG::DEBUG,__FUNCTION__,_buf);
  }

  status = decode_ch_word(word,last_word);

  if(status){
    _nwords++;
    _checksum+=word;
  }

  last_word = word;

  return status;
}

//#########################################################
bool algo_pmt_xmit::check_event_quality(){
//#########################################################

  bool status = true;

  // Check if _checksum and _nwords agrees with that of event header.
  //if(_nwords != _header_info.nwords) {
  _nwords-=1;
  if(_nwords!=_header_info.nwords){

    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Disagreement on nwords: counted=%u, expected=%u",_nwords,_header_info.nwords));

    status = false;

  }

  //if(_checksum != _header_info.checksum) {
  if(_checksum!=_header_info.checksum){

    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Disagreement on checksum: summed=%x, expected=%x",_checksum,_header_info.checksum));

    backtrace();
    status = false;

  }

  return status;

}



//#################################################
bool algo_pmt_xmit::process_event_header(const PMT::word_t word, 
					 PMT::word_t &last_word) 
{
//#################################################

  bool status = true;

  if(!_event_data)

    _event_data = (pmt_wf_collection*)(_storage->get_data(DATA_STRUCT::PMT_WF_COLLECTION));
  
  else if(get_word_class(last_word) != PMT::EVENT_LAST_WORD){

    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Unexpected word (%x, previous=%x) while processing event header!",word,last_word));

    status = false;

  }

  last_word = word;

  return status;

}

//#########################################################
bool algo_pmt_xmit::process_fem_last_word(const PMT::word_t word,
						  PMT::word_t &last_word)
{
//#########################################################

  bool status = true;

  if(_verbosity[MSG::INFO])
      
    Message::send(MSG::INFO,__FUNCTION__,Form("End of FEM word: %x...",word));
  
  PMT::word_t last_word_class = get_word_class(last_word);
  if(last_word_class != PMT::CHANNEL_LAST_WORD &&
     last_word_class != PMT::FEM_FIRST_WORD) {

    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Unexpected word: %x (previous = %x)",word,last_word));

    status = false;

  }

  _nwords++;
  //_checksum+=word;

  if(status) store_event();

  last_word = word;

  return status;
}

//#########################################################
bool algo_pmt_xmit::process_event_last_word(const PMT::word_t word,
					    PMT::word_t &last_word)
{  
//#########################################################

  bool status = true;

  if(_verbosity[MSG::INFO])
    
    Message::send(MSG::INFO,__FUNCTION__,Form("End of event word: %x...",word));

  if(get_word_class(last_word)!=PMT::FEM_LAST_WORD){

    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Unexpected word: %x (previous = %x)",word,last_word));

    status = false;

  }

  last_word = word;

  return status;
}

//#########################################################
bool algo_pmt_xmit::store_event(){
//#########################################################

  bool status = check_event_quality();

  // Store if condition is satisfied
  if(status) {

    if(_verbosity[MSG::INFO]){

      Message::send(MSG::INFO,__FUNCTION__,
		    Form("Storing event %u with %zu channel entries...",
			 _event_data->event_id(), _event_data->size()));

    }

    _event_data->set_module_address    ( _header_info.module_address    );
    _event_data->set_module_id         ( _header_info.module_id         );
    _event_data->set_event_id          ( _header_info.event_id          );
    _event_data->set_event_frame_id    ( _header_info.event_frame_id    );
    _event_data->set_trigger_frame_id  ( _header_info.trigger_frame_id  );
    _event_data->set_trigger_timeslice ( _header_info.trigger_timeslice );
    _event_data->set_nwords            ( _header_info.nwords            );
    _event_data->set_checksum          ( _header_info.checksum          );

    status = _storage->next_event();

  }
  else
    
    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Skipping to store event %d...",_header_info.event_id));

  clear_event();

  return status;
}

//#########################################################
bool algo_pmt_xmit::decode_ch_word(const PMT::word_t word, 
				   PMT::word_t &last_word)
{
//#########################################################

  PMT::PMT_WORD word_class      = get_word_class(word);
  PMT::PMT_WORD last_word_class = get_word_class(last_word);
  bool status=true;

  if(_verbosity[MSG::DEBUG]){
    sprintf(_buf,"Decoding word: %x ... last word class: %d",word,last_word_class);
    Message::send(MSG::DEBUG,__FUNCTION__,_buf);
  }  
  
  switch(word_class){

  case PMT::EVENT_HEADER:
  case PMT::FEM_HEADER:
  case PMT::UNDEFINED_WORD:
  case PMT::FEM_LAST_WORD:
  case PMT::EVENT_LAST_WORD:
    Message::send(MSG::ERROR,__FUNCTION__,
		  Form("Unexpected word: %x (previous=%x)",word,last_word));
    status = false;
    break;
  case PMT::FEM_FIRST_WORD:
    //
    // First channel word in this event ... nothing special to do
    if(last_word_class!=PMT::FEM_HEADER){
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Found FEM first word (%x) in an unexpected place (previous=%x)!",word,last_word));
      status=false;
    }
    break;

  case PMT::CHANNEL_HEADER:
    //
    // Channel header ... read in channel info from this word
    if(last_word_class!=PMT::FEM_FIRST_WORD && 
       last_word_class!=PMT::CHANNEL_LAST_WORD) {
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Found channel header (%x) in an unexpected place (previous=%x)!",word,last_word));
      status=false;
    }else{
      // Initialize channel info, then fill with channel number & disc. id.
      store_ch_data();
      _channel_header_count=1;
      // Channel Number
      _ch_data.set_channel_number( word & 0x3f ); 
      // Discriminator ID
      _ch_data.set_disc_id((PMT::DISCRIMINATOR)((word & 0xfff)>>9));  
    }
    break;

  case PMT::CHANNEL_WORD:
  case PMT::CHANNEL_LAST_WORD:
    //
    // This is channel ADC count or possibly remaining channel header info
    // The first two sample are treated as channel header info
    if(last_word_class!=PMT::CHANNEL_HEADER && 
       last_word_class!=PMT::CHANNEL_WORD &&
       last_word_class!=PMT::CHANNEL_LAST_WORD ) {
      // Sanity check: the last word should be either channel header or data word.
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Found channel word (%x) in an unexpected place (previous=%x)!",word,last_word));

      status=false;
    }else{
      // Treat a case of missing very first channel header word: happens when there is no time in between two readout data.
      if(last_word_class==PMT::CHANNEL_LAST_WORD){
	// In this case, we should be missing the CHANNEL_HEADER because this pulse is from the same channel & discriminator id.
	// Do an operation that is done for the case of CHANNEL_HEADER, but use a stored value of channel number & disc. id.
	_channel_header_count=1;
	_ch_data.set_channel_number(_last_channel_number);
	_ch_data.set_disc_id(_last_disc_id);
	if(_verbosity[MSG::NORMAL])
	  Message::send(MSG::NORMAL,__FUNCTION__,
			Form("Found consecutively readout data arrays @ event %d (missing channel very first header)!",
			     _event_data->event_id())
			);
      }

      //
      // Ordinary operation for channel header / adc words
      //
      if(_channel_header_count==CHANNEL_HEADER_COUNT ) {

	// Finished reading in header words. This should be a mere ADC count.
	// Raise error if the last word class is not channel word
	if(last_word_class!=PMT::CHANNEL_WORD){

	  Message::send(MSG::ERROR,__FUNCTION__,
			Form("Found channel word (%x) in an unexpected place (previous=%x)!",word,last_word));

	  status = false;
	}	  
	else if(word_class!=PMT::CHANNEL_LAST_WORD)

	  _ch_data.push_back(word & 0xfff);	
	
      }else if(last_word_class==PMT::CHANNEL_HEADER   ) {
	// First of 2 channel header words. Record the values.

	// This gives upper 8 bits of 20-bit timeslice number
	_ch_data.set_timeslice( (word & 0x1f)<<12 );       

	// Lower 3 of 8 bits is the channel frame ID
	_ch_data.set_channel_frame_id( ((word & 0xff)>>5) +
				       (((_event_data->event_frame_id())>>3)<<3) ); 

	// Correct channel frame id roll-over w.r.t. event frame id
	_ch_data.set_channel_frame_id(round_diff(_event_data->event_frame_id(),
						 _ch_data.channel_frame_id(),
						 0x7));
	_channel_header_count++;
      }else if(last_word_class==PMT::CHANNEL_WORD     ) {
	// Second of 2 channel header words. Record the values & inspect them.
	_ch_data.set_timeslice(_ch_data.timeslice() + (word & 0xfff)); // This gives lower 12 bits of 20-bit timeslice number
	_channel_header_count++;

	if(_verbosity[MSG::INFO]){
	  sprintf(_buf,"Read-in headers for Ch. %-3d!",_ch_data.channel_number());
	  Message::send(MSG::INFO,_buf);
	}
      }

      // If this is channel's last word, store & clear channel info
      if(word_class==PMT::CHANNEL_LAST_WORD){
	if(_verbosity[MSG::INFO]){
	  sprintf(_buf,"Encountered the last word (%x) for channel %d",word,_ch_data.channel_number());
	  Message::send(MSG::INFO,_buf);
	  sprintf(_buf,"Event frame  : %d",_event_data->event_frame_id());
	  Message::send(MSG::INFO,_buf);
	  sprintf(_buf,"PMT frame    : %d",_ch_data.channel_frame_id());
	  Message::send(MSG::INFO,_buf);
	  sprintf(_buf,"Disc. ID     : %d",_ch_data.disc_id());
	  Message::send(MSG::INFO,_buf);
	  sprintf(_buf,"Start Time   : %d",_ch_data.timeslice());
	  Message::send(MSG::INFO,_buf);
	  sprintf(_buf,"# ADC sample : %zd",_ch_data.size());
	  Message::send(MSG::INFO,_buf);
	}
	store_ch_data();
      }
    }
    break;
  }

  return status;

}

void algo_pmt_xmit::store_ch_data(){

  _event_data->push_back(_ch_data);
  
  // Store this waveform's channel number and discriminator id.
  // This may be used in the next pulse in case the next pulse is coming with no time space in between.
  // In such case, the next pulse is missing the channel very first header word because it is supposed
  // to be combined.
  _last_channel_number=_ch_data.channel_number();
  _last_disc_id=_ch_data.disc_id();

  _channel_header_count=0;
  _ch_data.clear_data();

}


#endif
