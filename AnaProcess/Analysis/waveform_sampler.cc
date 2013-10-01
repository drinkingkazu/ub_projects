#ifndef WAVEFORM_SAMPLER_CC
#define WAVEFORM_SAMPLER_CC

#include "waveform_sampler.hh"

waveform_sampler::waveform_sampler() : _sel_info(), _hWF_map()
{ 
  _name="waveform_sampler"; 
  _fout=0;
  _selector=0;
  _wf_length = 0;
};

bool waveform_sampler::initialize() {

  if(!(_selector)) {

    Message::send(MSG::ERROR,__FUNCTION__,
		  "pulse selection algorithm has not been set! Aborting...");

    return false;
  }

  if(_wf_length < 1) {

    Message::send(MSG::ERROR,__FUNCTION__,
		  "sample waveform length is set to < 1! Aborting...");
    
    return false;
  }

  _hWF_map.clear();

  return true;
}

void waveform_sampler::clear_event(){

  _sel_info.clear();

}

bool waveform_sampler::analyze(storage_manager* storage) {

  pulse_collection *pulses = (pulse_collection*)(storage->get_data(_selector->pulse_type()));
  
  pmt_wf_collection   *wfs    = (pmt_wf_collection*)(storage->get_data(DATA_STRUCT::PMT_WF_COLLECTION));

  if(!pulses){

    Message::send(MSG::ERROR,__FUNCTION__,
		  "Necessary input pulses not found!");

    return false;

  }

  if(!wfs){

    Message::send(MSG::ERROR,__FUNCTION__,
		  "Necessary input raw waveform not found!");

    return false;

  }

  clear_event();

  if(!(_selector->analyze(storage))) return false;

  //
  // Log ch number, frame id, sample number of selected pulse
  //
  for(auto pulse : *pulses){
    
    if(!(_selector->select_pulse(&pulse))) continue;
    
    PMT::ch_number_t ch     = pulse.channel_number();
    PMT::word_t      frame  = pulse.frame_id();
    PMT::word_t      sample = pulse.timeslice();
    
    if(_sel_info.find(ch)==_sel_info.end()) 
      
      _sel_info[ch] = std::map<PMT::word_t,std::set<PMT::word_t> >();
    
    if(_sel_info[ch].find(frame) == _sel_info[ch].end() )
      
      _sel_info[ch][frame] = std::set<PMT::word_t>(); 
    
    if(_sel_info[ch][frame].find(sample) == _sel_info[ch][frame].end())
      
      _sel_info[ch][frame].insert(sample);

  }
  
  //
  // Loop over waveforms that matches with the selected IDs
  //
  for(auto wf : *wfs){

    PMT::ch_number_t ch     = wf.channel_number();
    PMT::word_t      frame  = wf.channel_frame_id();
    PMT::word_t      sample = wf.timeslice();

    if(_sel_info.find(ch) == _sel_info.end()) continue;

    if(_sel_info[ch].find(frame) == _sel_info[ch].end()) continue;

    if(_sel_info[ch][frame].find(sample) == _sel_info[ch][frame].end()) continue;

    sample_waveform(&wf);

  }  

  /*
  for(pmt_wf_collection::iterator iter(wfs->begin());
      iter != wfs->end();
      ++iter){

    PMT::ch_number_t ch     = (*iter).channel_number();
    PMT::word_t      frame  = (*iter).channel_frame_id();
    PMT::word_t      sample = (*iter).timeslice();

    if(_sel_info.find(ch) == _sel_info.end()) continue;

    if(_sel_info[ch].find(frame) == _sel_info[ch].end()) continue;

    if(_sel_info[ch][frame].find(sample) == _sel_info[ch][frame].end()) continue;

    sample_waveform(&(*iter));

  }  
  */


  return true;
}

void waveform_sampler::sample_waveform(const pmt_waveform *wf){

  PMT::ch_number_t ch(wf->channel_number());

  int nbins = (_wf_length < (int)(wf->size())) ? _wf_length : (int)(wf->size());

  if(_hWF_map.find(ch)==_hWF_map.end()) {
    
    _hWF_map[ch] = new TH2D(Form("hWF_Ch%02d",ch),
			    Form("Raw Waveform Sample for Ch %d; Timeslice; ADC",ch),
			    nbins, -0.5, ((double)nbins)-0.5, 4096, -0.5,4095.5);
    
  }

  for(int i=0; i<nbins; i++)

    _hWF_map[ch]->Fill((double)i, (double)(wf->at(i)));
  
}

bool waveform_sampler::finalize() {

  if(_fout){
    
    _fout->cd();

    for(std::map<PMT::ch_number_t,TH2D*>::const_iterator iter(_hWF_map.begin());
	iter!=_hWF_map.end();
	++iter){

      (*iter).second->Write();

    }
  
  }

  return true;
}

#endif
