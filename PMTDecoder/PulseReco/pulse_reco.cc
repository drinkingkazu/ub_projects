#ifndef PULSE_RECO_CC
#define PULSE_RECO_CC

#include "pulse_reco.hh"

bool pulse_reco::initialize(){
  
  if(!_reco_algo) {

    Message::send(MSG::ERROR,__PRETTY_FUNCTION__,"Pulse reconstruction algorithm not set!");
		  
    return false;
  }

  _ped_Mean  = new TH2D("_ped_Mean","Pedestal Mean vs. Channel Number; Channel Number; Ped. Mean [ADC]",
			40,-0.5,39.5,100,1850,2150);

  _ped_RMS   = new TH2D("_ped_RMS","Pedestal RMS vs. Channel Number; Channel Number; Ped. RMS [ADC]",
			40,-0.5,39.5,100,0,20);
  
  _channel_Q = new TH2D("_channel_Q","Summed Charge vs. Channel Number; Channel Number; Charge;",
			40,-0.5,39.5,200,0,100000);

  _channel_A = new TH2D("_channel_Q","Summed Amplitude vs. Channel Number; Channel Number; Amplitude;",
			40,-0.5,39.5,200,0,20000);

  _event_Q   = new TH1D("_event_Q","Summed Charge (all Ch.); Charge; Entries",
			200,0,100000);

  _event_A   = new TH1D("_event_A","Summed Amplitude (all Ch.); Amplitude; Entries",
			200,0,20000);

  return true;

}

bool pulse_reco::analyze(storage_manager* storage){

  event_waveform *waveforms = (event_waveform*)(storage->get_data(DATA_STRUCT::WF_COLLECTION));

  bool status = true;

  pulse_collection *pulses = (pulse_collection*)(storage->get_data(DATA_STRUCT::PULSE_COLLECTION));
  
  double event_charge = 0;
  double event_amplitude = 0;
  int    npulse=0;

  for(size_t i=0; i<40; i++){

    _channel_Q_m[i]=0.;
    _channel_A_m[i]=0.;

  }

  for(event_waveform::iterator iter(waveforms->begin());
      iter!=waveforms->end();
      ++iter){
  
    switch(_ped_method){

    case kHEAD:

      _ped_algo.analyze_head(&(*iter));

      _reco_algo->set_ped_mean(_ped_algo.mean_head());

      _reco_algo->set_ped_rms(_ped_algo.rms_head());

      status = status && _reco_algo->reco(&(*iter));

      break;

    case kTAIL:

      _ped_algo.analyze_tail(&(*iter));

      _reco_algo->set_ped_mean(_ped_algo.mean_tail());

      _reco_algo->set_ped_rms(_ped_algo.rms_tail());

      status = status && _reco_algo->reco(&(*iter));

      break;

    case kBOTH:

      _ped_algo.analyze_head(&(*iter));

      _ped_algo.analyze_tail(&(*iter));
      
      if( _ped_algo.rms_head() < _ped_algo.rms_tail()) {

	_reco_algo->set_ped_mean(_ped_algo.mean_head());

	_reco_algo->set_ped_rms(_ped_algo.rms_head());

      }else{

	_reco_algo->set_ped_mean(_ped_algo.mean_tail());

	_reco_algo->set_ped_rms(_ped_algo.rms_tail());

      }

      status = status && _reco_algo->reco(&(*iter));
      
      break;
    }

    for(size_t i=0; i < _reco_algo->get_npulse(); ++i) {

      // Fill output data product for waveform-wise info
      pulse_info pulse;

      pulse.set_timeslice        ( (*iter).timeslice()        );
      pulse.set_channel_number   ( (*iter).channel_number()   );
      pulse.set_channel_frame_id ( (*iter).channel_frame_id() );
      pulse.set_disc_id          ( (*iter).disc_id()          );

      pulse.set_ped_mean   ( _reco_algo->ped_mean()            );
      pulse.set_ped_rms    ( _reco_algo->ped_rms()             );
      pulse.set_charge     ( _reco_algo->get_pulse(i)->area    );
      pulse.set_pulse_peak ( _reco_algo->get_pulse(i)->peak    );
      pulse.set_start_time ( _reco_algo->get_pulse(i)->t_start );
      pulse.set_max_time   ( _reco_algo->get_pulse(i)->t_max   );
      pulse.set_end_time   ( _reco_algo->get_pulse(i)->t_end   );

      pulses->push_back(pulse);

      // Accumulate event-wise charge/amplitude sum
      event_charge    += _reco_algo->get_pulse(i)->area;
      event_amplitude += _reco_algo->get_pulse(i)->peak;

      // Accumulate channel-wise charge/amplitude sum
      _channel_Q_m[pulse.channel_number()] += pulse.charge();
      _channel_A_m[pulse.channel_number()] += pulse.pulse_peak();

      // Fill waveform-wise histograms
      _ped_Mean->Fill  ( pulse.channel_number(), pulse.ped_mean() );
      _ped_RMS->Fill   ( pulse.channel_number(), pulse.ped_rms()  );

      npulse++;

    } // end of reco-ed pulse loop

  } // end of PMT waveform loop

  // Fill output data product for event-wise info
  pulses->set_sum_charge ( event_charge    );
  pulses->set_sum_peak   ( event_amplitude );
  pulses->set_npulse     ( npulse          );


  // Fill event-wise histogram
  _event_Q->Fill(event_charge);
  _event_A->Fill(event_amplitude);

  // Fill channel-wise histograms
  for(size_t i=0; i<_channel_Q_m.size(); ++i){
    
    if(_channel_Q_m[i])

      _channel_Q->Fill((double)(i), _channel_Q_m[i]);

    if(_channel_A_m[i])

      _channel_A->Fill((double)(i), _channel_A_m[i]);

  }

  return status;
  
}

bool pulse_reco::finalize(){

  _fout->cd();
  
  _ped_Mean->Write();
  _ped_RMS->Write();
  _channel_Q->Write();
  _channel_A->Write();
  _event_Q->Write();
  _event_A->Write();

  return true;

}

#endif
