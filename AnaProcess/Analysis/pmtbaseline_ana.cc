#ifndef PMTBASELINE_ANA_CC
#define PMTBASELINE_ANA_CC

#include "pmtbaseline_ana.hh"

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
  // event-wise data. For a reference of event_waveform class instance, 
  // see the class index in the documentation.
  // 
  // Example to print out event id on screen...
  //
  //const event_waveform* event_wf = (event_waveform*)(storage->get_data(DATA_STRUCT::WF_COLLECTION));
  //std::cout << Form("Event ID: %d",event_wf->event_id()) << std::endl;
  //

  // Get a pointer to pulse collection object
  const pulse_collection* pulses = (pulse_collection*)(storage->get_data(DATA_STRUCT::PULSE_COLLECTION));

  // Loop over pulse collection and fill histograms
  for(pulse_collection::const_iterator iter(pulses->begin()); iter!=pulses->end(); ++iter){

    PMT::ch_number_t ch=(*iter).channel_number();

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
				     600,0,3000);
      _hArray_Peak[ch]    = new TH1D(Form("hPeak_Ch%03d",ch),
				     Form("Max. Amplitude for Ch %03d; Peak Amp. [ADC]; Entries",ch),
				     500,0,500);
    }
    
    // Fill histograms
    _hArray_PedMean[ch]->Fill((*iter).ped_mean());
    _hArray_PedRMS[ch]->Fill((*iter).ped_rms());
    _hArray_Charge[ch]->Fill((*iter).charge());
    _hArray_Peak[ch]->Fill((*iter).pulse_peak());
    
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
