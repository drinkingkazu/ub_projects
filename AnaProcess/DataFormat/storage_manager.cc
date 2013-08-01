#ifndef STORAGE_MANAGER_CC
#define STORAGE_MANAGER_CC
#include "storage_manager.hh"

storage_manager* storage_manager::me=0;

storage_manager::storage_manager(storage_manager::MODE mode)
  : decoder_base(), _treename("pmt_data")
{
  _name="storage_manager";
  //_event_wf=0;
  _fout=0;
  _out_fname="";
  _status=INIT;

  reset();
  _mode=mode;

  for(size_t i=0; i<DATA_STRUCT::DATA_TYPE_MAX; ++i) {
    _read_data_array[i]=false;
    _write_data_array[i]=false;
    _in_ch[i]=0;
    _out_ch[i]=0;
    _ptr_data_array[i]=0;
  }

  _read_data_array[DATA_STRUCT::WF_COLLECTION]=true;
  _write_data_array[DATA_STRUCT::WF_COLLECTION]=true;

};

data_base* storage_manager::get_data(DATA_STRUCT::DATA_TYPE type){

  return _ptr_data_array[type];

}

void storage_manager::reset(size_t len)
{
  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called ...");
  
  switch(_status){
  case READY_IO:
  case OPENED:
  case OPERATING:
    close();
    break;
  case INIT:
  case CLOSED:
    break;
  }
  _index=0;
  _nevents=0;
  _nevents_written=0;
  _nevents_read=0;
  _mode=UNDEFINED;
  _status=INIT;

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"ends ...");  
};

bool storage_manager::is_open()
{
  bool status=true;
  switch(_status){
  case OPENED:
  case READY_IO:
  case OPERATING:
    break;
  case INIT:
  case CLOSED:
    status=false;
  }

  return status;
}

bool storage_manager::is_ready_io()
{
  bool status=true;
  switch(_status){
  case READY_IO:
  case OPERATING:
    break;
  case INIT:
  case OPENED:
  case CLOSED:
    status=false;
  }
  return status;
}

bool storage_manager::open()
{
  bool status=true;

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called ...");

  if((_mode==READ || _mode==BOTH) && _in_fnames.size()==0){
    Message::send(MSG::ERROR,
		  __FUNCTION__,
		  "Open attempt w/o specifing a file name!");
    return false;
  }
  
  if((_mode==WRITE || _mode==BOTH) && _out_fname.size()==0){
    Message::send(MSG::ERROR,
		  __FUNCTION__,
		  "Open attempt w/o specifing a file name!");
    return false;
  }

  switch(_mode){

  case BOTH:
  case READ:

    for(std::vector<std::string>::const_iterator iter(_in_fnames.begin());
	iter!=_in_fnames.end();
	++iter) {

      TFile *fin = TFile::Open((*iter).c_str(),"READ");
      if(!fin) {
	sprintf(_buf,"Open attempt failed for a file: %s", (*iter).c_str());
	Message::send(MSG::ERROR,__FUNCTION__,_buf);
	status=false;
      }else{
	sprintf(_buf,"Opening a file in READ mode: %s", (*iter).c_str());
	Message::send(MSG::NORMAL,__FUNCTION__,_buf);
	fin->Close();
      }
      
    }

    if(_mode==READ) break;
  case WRITE:
    sprintf(_buf,"Opening a file in WRITE mode: %s",_out_fname.c_str());
    Message::send(MSG::NORMAL,__FUNCTION__,_buf);
    _fout=TFile::Open(_out_fname.c_str(),"RECREATE");
    if(!_fout) {
      sprintf(_buf,"Open attempt failed for a file: %s", _out_fname.c_str());
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      status=false;
    }
    break;

  case UNDEFINED:
    Message::send(MSG::ERROR,
		  __FUNCTION__,
		  "Open attempt w/o specifing I/O mode!");
    status=false;
    break;
  }
  
  if(!status) return status;

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"ends ...");

  _status=OPENED;
  return prepare_tree();
};

bool storage_manager::prepare_tree(){
  
  bool status=true;

  std::vector<uint64_t> nevents_array(DATA_STRUCT::DATA_TYPE_MAX,0);
  
  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called ...");

  if(!_status==OPENED) {
    sprintf(_buf,"Unexpected function call @ _status=%d!",_status);
    Message::send(MSG::ERROR,__FUNCTION__,_buf);
    status=false;
  }

  for(size_t i=0; i<DATA_STRUCT::DATA_TYPE_MAX; ++i){

    if(!_status) break;

    if(_mode!=WRITE && _read_data_array[i]) {

      _in_ch[i]=new TChain(DATA_STRUCT::DATA_TREE_NAME[(DATA_STRUCT::DATA_TYPE)i].c_str(),
			   Form("%s Tree",DATA_STRUCT::DATA_TREE_NAME[(DATA_STRUCT::DATA_TYPE)i].c_str()));

      for(size_t j=0; j<_in_fnames.size(); ++j)

	_in_ch[i]->AddFile(_in_fnames[j].c_str());

      nevents_array[i]=_in_ch[i]->GetEntries();
      
      if(nevents_array[i]) { 

	create_data_ptr((DATA_STRUCT::DATA_TYPE)i);

	_in_ch[i]->SetBranchAddress(Form("%s_branch",_ptr_data_array[i]->GetName()),&(_ptr_data_array[i]));

	if(!_nevents) _nevents = nevents_array[i];

      }else{
	delete _in_ch[i];
	_in_ch[i]=0;
      }
    }

    if(_mode!=READ && _write_data_array[i] ) {
      
      _out_ch[i]=new TTree(DATA_STRUCT::DATA_TREE_NAME[(DATA_STRUCT::DATA_TYPE)i].c_str(),
			   Form("%s Tree",DATA_STRUCT::DATA_TREE_NAME[(DATA_STRUCT::DATA_TYPE)i].c_str()));
      
      create_data_ptr((DATA_STRUCT::DATA_TYPE)i);
      
      _out_ch[i]->Branch(Form("%s_branch",_ptr_data_array[i]->GetName()),
			 _ptr_data_array[i]->GetName(),
			 &(_ptr_data_array[i]));

    }

    _nevents_written=0;
    _nevents_read=0;
    _index=0;

  }

  if( _mode!=WRITE && _nevents==0) {
    Message::send(MSG::ERROR,__FUNCTION__,"Did not find any relevant data tree!");
    status=false;
  }

  for(size_t i=0; i<DATA_STRUCT::DATA_TYPE_MAX; ++i) {

    if(nevents_array[i] && _nevents!=nevents_array[i]) {
      sprintf(_buf,"Different number of entries found on tree: %s",DATA_STRUCT::DATA_TREE_NAME[(DATA_STRUCT::DATA_TYPE)i].c_str());
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      status=false;
    }

  }

  if(status) _status=READY_IO;

  else close();

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"ends ...");

  return status;
}

void storage_manager::create_data_ptr(DATA_STRUCT::DATA_TYPE type){

  if(_ptr_data_array[type]) return;

  switch(type){
  case DATA_STRUCT::WF_COLLECTION:
    _ptr_data_array[type] = (data_base*)(new event_waveform());
    break;
  case DATA_STRUCT::TRIG_INFO:
    _ptr_data_array[type] = (data_base*)(new trig_info());
    break;
  case DATA_STRUCT::PULSE_COLLECTION:
    _ptr_data_array[type] = (data_base*)(new pulse_collection());
    break;
  case DATA_STRUCT::USER_COLLECTION:
    _ptr_data_array[type] = (data_base*)(new user_collection());
    break;
  case DATA_STRUCT::DATA_TYPE_MAX:
    break;
  }

  return;
}

void storage_manager::delete_data_ptr(DATA_STRUCT::DATA_TYPE type){

  if(!_ptr_data_array[type]) return;

  delete _ptr_data_array[type];

  _ptr_data_array[type]=0;

  return;
}

bool storage_manager::close(){

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"called ...");

  bool status=true;
  switch(_status){
  case INIT:
  case CLOSED:
    Message::send(MSG::ERROR,__FUNCTION__,"Attempt to close file while not operating I/O!");
    status=false;
    break;
  case OPENED:
  case READY_IO:
    Message::send(MSG::WARNING,__FUNCTION__,"Closing a file without any I/O operation done!");
    break;
  case OPERATING:
    if(_mode!=READ){
      _fout->CurrentFile()->cd();

      for(size_t i=0; i<DATA_STRUCT::DATA_TYPE_MAX; i++) {
	if(!_out_ch[i]) continue;
	_out_ch[i]->Write();

	sprintf(_buf,"TTree \"%s\" written with %lld events...",
		_out_ch[i]->GetName(),
		_out_ch[i]->GetEntries());
	Message::send(MSG::NORMAL,__FUNCTION__,_buf);
      }
    }
    break;
  }

  if(status) {

    sprintf(_buf,"Closing the output: %s",_out_fname.c_str());
    Message::send(MSG::NORMAL,__FUNCTION__,_buf);
    if(_fout) {_fout->Write(); _fout->Close();}
    _fout=0;

    for(size_t i=0; i<DATA_STRUCT::DATA_TYPE_MAX; ++i) {
      
      if(_in_ch[i])  { delete _in_ch[i];  _in_ch[i]=0;  }
      if(_out_ch[i]) { _out_ch[i]=0; }
      if(_ptr_data_array[i]) delete_data_ptr((DATA_STRUCT::DATA_TYPE)i);
      
    }

  }

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__PRETTY_FUNCTION__,"ends ...");

  _status=CLOSED;
  return status;
}

bool storage_manager::go_to(uint32_t index) {

  bool status=true;
  if(_mode==WRITE){
    Message::send(MSG::ERROR,__FUNCTION__,
		  "Cannot move the data pointer back/forth in WRITE mode.");
    status=false;
  }else if(_nevents && _nevents<index){
    sprintf(_buf,"Requested index, %d, exceeds the total entries, %d!",
	    index,_nevents);
    Message::send(MSG::ERROR,__FUNCTION__,_buf);
    status=false;
  }else
    _index=index;

  if(status) status=next_event();

  return status;
}

bool storage_manager::next_event(){

  bool status=true;

  switch(_status){
  case READY_IO:
    // first event
    _status=OPERATING;
  case OPERATING:
    switch(_mode) {
    case READ:
      status=read_event();
      break;
    case WRITE:
      status=write_event();
      break;
    case BOTH:
      if(_nevents_read)
	status = write_event();
      if(status) status = read_event();
      break;
    case UNDEFINED:
      break;
    }
    break;
  case INIT:
  case OPENED:
  case CLOSED:
    Message::send(MSG::ERROR,__FUNCTION__,"Cannot perform I/O on a closed file!");
    status=false;
    break;
  }
  return status;
}

bool storage_manager::read_event(){

  if(_index>=_nevents)
    return false;

  for(size_t i=0; i<DATA_STRUCT::DATA_TYPE_MAX; ++i) { 

    if(_in_ch[i])
      _in_ch[i]->GetEntry(_index);

  }

  _index++;
  _nevents_read++;
  return true;
}

bool storage_manager::write_event(){
  
  for(size_t i=0; i<DATA_STRUCT::DATA_TYPE_MAX; ++i) {

    if(!_out_ch[i]) continue;

    _out_ch[i]->Fill();
    _ptr_data_array[i]->clear_data();

  }

  if(_mode==WRITE)
    _index++;
  _nevents_written++;
  //_event_wf->clear_data();
  return true;
}

#endif
