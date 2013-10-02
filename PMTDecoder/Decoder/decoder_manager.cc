#ifndef DECODER_MANAGER_CC
#define DECODER_MANAGER_CC

#include "decoder_manager.hh"

decoder_manager::decoder_manager()
  : decoder_base(),
    _fin(FORMAT::ASCII)
{
  _input_file="";
  _debug_mode=false;
  _fin=bin_io_handler();

  _read_block_size = 0;
  _read_by_block   = false;

  _storage=storage_manager::get();
  _storage->set_io_mode(storage_manager::WRITE);

  //if((_storage->input_filename()).size()==0)
  //  _storage->set_in_filename("out.root");

  _decoder=0;

  set_verbosity(MSG::NORMAL);
}


void decoder_manager::reset() {
  if(_storage->is_open()) {
    _storage->close();
    _storage->reset();
  }
  _fin.reset();

}

bool decoder_manager::open_file()
{
  return _fin.open();
}


bool decoder_manager::initialize()
{
  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__FUNCTION__," begins...");
  if(!_decoder){
    Message::send(MSG::ERROR,__FUNCTION__,"Algorithm not attached. Aborting.");
    return false;
  }
  if(!_storage){
    Message::send(MSG::ERROR,__FUNCTION__,"Stoarge I/O pointer is empty.");
    return false;
  }

  _decoder->set_debug_mode(_debug_mode);
  _decoder->set_verbosity(_verbosity_level);
  _storage->set_verbosity(_verbosity_level);
  _fin.set_verbosity(_verbosity_level);

  bool status=true;
  // Check if a file can be opened
  if(_fin.is_open())
    Message::send(MSG::WARNING,"Reading alrady-opened file contents!");
  else if(!_fin.open()) {
    Message::send(MSG::ERROR,__FUNCTION__,"Failed file I/O...");
    status=false;
  }

  if(!_storage->is_open())
    _storage->open();

  if(!_storage->is_ready_io()) {
    Message::send(MSG::ERROR,__FUNCTION__,
		  "Error in data output stream preparation.");
    status=false;
  }

 _decoder->set_storage_ptr(_storage);

  for(std::vector<ana_base*>::iterator iter(_analyzers.begin());
      iter!=_analyzers.end();
      ++iter)
    status = (status && (*iter)->initialize());

  return status;
}

bool decoder_manager::decode() {

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__FUNCTION__," begins...");

  bool status=true;
  PMT::word_t word = (_read_by_block) ? _fin.read_multi_word(_read_block_size) : _fin.read_word();
  uint32_t ctr=0;
  time_t watch;
  while(!(_fin.eof()) && status) {

    status=_decoder->process_word(word);

    /*
    if(status) {
      for(std::vector<ana_base*>::iterator iter(_analyzers.begin());
	  iter!=_analyzers.end();
	    ++iter)
	(*iter)->analyze(_storage);
    }
    */

    if(!status){
      //if(_decoder->backtrace_mode())
      //_decoder->backtrace();
      if(_debug_mode){
	Message::send(MSG::ERROR,__FUNCTION__,"Process status failure ... but continue since DEBUG mode!");
	status=true;
      }
    }

    word = (_read_by_block) ? _fin.read_multi_word(_read_block_size) : _fin.read_word();
    if(_storage->get_index()==(ctr*2000)){
      time(&watch);
      sprintf(_buf,"  ... processed %-6d events : %s",ctr*2000,ctime(&watch));
      Message::send(MSG::NORMAL,__FUNCTION__,_buf);
      ctr+=1;
    }
  }
  
  if(!status && !_debug_mode){
    sprintf(_buf,"Event loop terminated. Last event: %d  ... stored: %d events",
	    ((pmt_wf_collection*)(_storage->get_data(DATA_STRUCT::PMT_WF_COLLECTION)))->event_id(),
	    _storage->get_entries());
    Message::send(MSG::ERROR,__FUNCTION__,_buf);
  }else if(!(_decoder->is_event_empty())){
    Message::send(MSG::WARNING,
		  __FUNCTION__,
		  "Last event not stored by algorithm. Missing end-of-event word??");
    if(_decoder->check_event_quality()){
      sprintf(_buf,"Last event checksum agreed. Saving on file... event id: %d",
	      ((pmt_wf_collection*)(_storage->get_data(DATA_STRUCT::PMT_WF_COLLECTION)))->event_id());
      Message::send(MSG::WARNING,__FUNCTION__,_buf);
      _storage->next_event();
    }else{
      sprintf(_buf,"Skip saving the last event: %d ",
	      ((pmt_wf_collection*)(_storage->get_data(DATA_STRUCT::PMT_WF_COLLECTION)))->event_id());
      Message::send(MSG::WARNING,__FUNCTION__,_buf);
      status=false;
    }    
  }
    
  return status;
  
}

bool decoder_manager::finalize() {

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__FUNCTION__," begins...");  

  for(std::vector<ana_base*>::iterator iter(_analyzers.begin());
      iter!=_analyzers.end();
      ++iter)
    (*iter)->finalize();

  _storage->close();
  _fin.close();

  _decoder->reset();
  
  return true;
}

bool decoder_manager::run() {

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__FUNCTION__," begins...");

  bool status=true;
  if(initialize())
    status=decode();
  return (finalize() && status);
  
}
#endif

