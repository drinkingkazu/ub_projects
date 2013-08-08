#ifndef PULSE_ANALYZER_CC
#define PULSE_ANALYZER_CC

#include "pulse_analyzer.hh"

pulse_analyzer::pulse_analyzer() { 
  _name="pulse_analyzer"; 

  clear_event();
}

void pulse_analyzer::clear_event(){

}

bool pulse_analyzer::initialize() {
  amplitude   = new TH1D("pulse_peak", "Amplitude", 205, 0, 4100);
  return true;
}

bool pulse_analyzer::analyze(storage_manager* storage) {
  
  pulse_collection *pulses = (pulse_collection*)(storage->get_data(DATA_STRUCT::PULSE_COLLECTION));
  event_waveform   *wfs    = (event_waveform*)(storage->get_data(DATA_STRUCT::WF_COLLECTION));
  if(pulses->size()==0) {
    Message::send(MSG::ERROR,__FUNCTION__,"No Pulse Found!");
    return false;
  }
  if(wfs->size()==0) {
    Message::send(MSG::ERROR,__FUNCTION__,"No Waveforms Found!");
    return false;
  }

  _event_id    = wfs->event_id();
  _sum_charge  = pulses->sum_charge();
  _sum_peak    = pulses->sum_peak();
  _npulse      = pulses->npulse();

  
  // Fill map for this event
  for(pulse_collection::const_iterator iter(pulses->begin());
      iter!=pulses->end();
      ++iter){

    //PMT::ch_number_t ch((*iter).channel_number());
    //PMT::word_t sample   = (*iter).timeslice();
    //PMT::word_t frame    = (*iter).frame_id();
    //double      t_start  = (*iter).start_time();
    //double      t_end    = (*iter).end_time();
    //double      charge   = (*iter).charge();
    double      amp      = (*iter).pulse_peak();
    amplitude->Fill(amp);
    //double      ped_base = (*iter).ped_mean();
    //double      ped_rms  = (*iter).ped_rms();
    //double      t_start_reco = (*iter).start_time_reco();

    int temp_ctr = 0;
    for(event_waveform::const_iterator ch_iter(wfs->begin());
      ch_iter!=wfs->end();
      ++ch_iter){
    
      temp_ctr++;
    }
  }
  return true;
}

bool pulse_analyzer::finalize() {

  // This function is called at the end of event loop.
  // Do all variable finalization you wish to do here.
  // If you need, you can store your ROOT class instance in the output
  // file. You have an access to the output file through "_fout" pointer.
  amplitude->Write();
  return true;
}

#endif
