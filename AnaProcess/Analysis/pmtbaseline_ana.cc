#ifndef PMTBASELINE_ANA_CC
#define PMTBASELINE_ANA_CC

#include "pmtbaseline_ana.hh"

pmtbaseline_ana::pmtbaseline_ana(){

  _name="pmtbaseline_ana"; 
  _fout=0;
  reset_cuts();

}

void pmtbaseline_ana::reset_cuts() {

  _cut_tstart_reco = std::make_pair(-1,2000);
  _cut_tstart   = std::make_pair(-1,2000);
  _cut_tend     = std::make_pair(-1,2000);
  _cut_amp      = std::make_pair(-1,4096);
  _cut_charge   = std::make_pair(-1,4096*2000);
  _cut_pedbase  = std::make_pair(-1,4096);
  _cut_pedrms   = std::make_pair(0,4096);
  _cut_channels = std::make_pair(0,PMT::INVALID_CH);
  _cut_event_id = std::make_pair(0,0xffffffff);
  _cut_npulse   = std::make_pair(0,0xffffffff);
  _cut_sum_charge = std::make_pair(0,4096*2000*PMT::NUM_PMT_CHANNEL);
  _cut_sum_peak = std::make_pair(0,4096*PMT::NUM_PMT_CHANNEL);
  
}

bool pmtbaseline_ana::initialize() {

  //
  // This function is called in the beggining of event loop
  // Do all variable initialization you wish to do here.
  // If you need, you have an output root file pointer "_fout".

  _hArray_PedMean.clear();
  _hArray_PedRMS.clear();
  _hArray_Charge.clear();
  _hArray_Peak.clear();

  return true;
}

bool pmtbaseline_ana::analyze(storage_manager* storage) {

  //
  // Do your event-by-event analysis here. This function is called for 
  // each event in the loop. You have "data" pointer which contains 
  // event-wise data. For a reference of pmt_wf_collection class instance, 
  // see the class index in the documentation.
  // 
  // Example to print out event id on screen...
  //
  //const pmt_wf_collection* event_wf = (pmt_wf_collection*)(storage->get_data(DATA_STRUCT::WF_COLLECTION));
  //std::cout << Form("Event ID: %d",event_wf->event_id()) << std::endl;
  //

  const pmt_wf_collection* wfs = (pmt_wf_collection*)(storage->get_data(DATA_STRUCT::PMT_WF_COLLECTION));
  const pulse_collection* pulses = (pulse_collection*)(storage->get_data(DATA_STRUCT::PULSE_COLLECTION));

  PMT::word_t event_id = wfs->event_id();
  double sum_charge    = pulses->sum_charge();
  double sum_peak      = pulses->sum_peak();
  uint32_t npulse      = pulses->npulse();

  // Check if this event is in the range of users' interest
  if(event_id < _cut_event_id.first || _cut_event_id.second < event_id)
    return true;
  else if(sum_charge < _cut_sum_charge.first || _cut_sum_charge.second < sum_charge)
    return true;
  else if(sum_peak < _cut_sum_peak.first || _cut_sum_peak.second < sum_peak)
    return true;
  else if(npulse < _cut_npulse.first || _cut_npulse.second < npulse)
    return true;

  // Loop over pulse collection and fill histograms
  for(pulse_collection::const_iterator iter(pulses->begin()); iter!=pulses->end(); ++iter){

    PMT::ch_number_t ch((*iter).channel_number());
    double      t_start  = (*iter).start_time();
    double      t_end    = (*iter).end_time();
    double      charge   = (*iter).charge();
    double      amp      = (*iter).pulse_peak();
    double      ped_base = (*iter).ped_mean();
    double      ped_rms  = (*iter).ped_rms();
    double      t_start_reco = (*iter).start_time_reco();

    // Check if this pulse passes the criteria
    if(ch < _cut_channels.first || _cut_channels.second < ch)
      continue;
    if(t_start_reco < _cut_tstart_reco.first || _cut_tstart_reco.second < t_start_reco)
      continue;
    if(t_start < _cut_tstart.first || _cut_tstart.second < t_start)
      continue;
    if(t_end < _cut_tend.first || _cut_tend.second < t_end)
      continue;
    if(charge < _cut_charge.first || _cut_charge.second < charge)
      continue;
    if(amp < _cut_amp.first || _cut_amp.second < amp)
      continue;
    if(ped_base < _cut_pedbase.first || _cut_pedbase.second < ped_base)
      continue;
    if(ped_rms < _cut_pedrms.first || _cut_pedrms.second < ped_rms)
      continue;    

    // Check if a histogram of this pulse's channel already exists. If not, add.
    if(_hArray_PedMean.find(ch) == _hArray_PedMean.end()) {

      _hArray_PedMean[ch] = new TH1D(Form("hPedMean_Ch%03d",ch),
				     Form("Pedestal Mean for Ch %03d; ADC; Entries",ch),
				     250,2000,2100);
      _hArray_PedRMS[ch]  = new TH1D(Form("hPedRMS_Ch%03d",ch),
				     Form("Pedestal RMS for Ch %03d; RMS; Entries",ch),
				     50,0,10);
      _hArray_Charge[ch]  = new TH1D(Form("hCharge_Ch%03d",ch),
				     Form("Integrated Charge Sum for Ch %03d; Charge [arbitrary];Entries",ch),
				     600,0,600);
      _hArray_Peak[ch]    = new TH1D(Form("hPeak_Ch%03d",ch),
				     Form("Max. Amplitude for Ch %03d; Peak Amp. [ADC]; Entries",ch),
				     500,0,200);
    }
    
    // Fill histograms
    _hArray_PedMean[ch]->Fill(ped_base);
    _hArray_PedRMS[ch]->Fill(ped_rms);
    _hArray_Charge[ch]->Fill(charge);
    _hArray_Peak[ch]->Fill(amp);
    
  }

  return true;
}

bool pmtbaseline_ana::finalize() {

  // This function is called at the end of event loop.
  // Do all variable finalization you wish to do here.
  // If you need, you can store your ROOT class instance in the output
  // file. You have an access to the output file through "_fout" pointer.

  _fout->cd();
  for(std::map<PMT::ch_number_t,TH1D*>::iterator iter(_hArray_PedMean.begin());
      iter!=_hArray_PedMean.end();
      ++iter){
    
    PMT::ch_number_t ch=(*iter).first;

    _hArray_PedMean[ch]->Write();
    _hArray_PedRMS[ch]->Write();
    _hArray_Charge[ch]->Write();
    _hArray_Peak[ch]->Write();

  }

  return true;
}

#endif
