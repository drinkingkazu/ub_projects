#ifndef PULSE_RECO_CC
#define PULSE_RECO_CC

#include "pulse_reco.hh"

bool pulse_reco::initialize(){
  
  if(!_reco_algo) return false;

  return true;

}

bool pulse_reco::analyze(storage_manager* storage){

  event_waveform *waveforms = (event_waveform*)(storage->get_data(DATA_STRUCT::WF_COLLECTION));

  bool status = true;

  pulse_collection *pulses = (pulse_collection*)(storage->get_data(DATA_STRUCT::PULSE_COLLECTION));
  
  double event_charge = 0;
  double event_amplitude = 0;
  int    npulse=0;

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
      
      pulse_info pulse;

      pulse.set_timeslice        ( (*iter).timeslice()        );
      pulse.set_channel_number   ( (*iter).channel_number()   );
      pulse.set_channel_frame_id ( (*iter).channel_frame_id() );
      pulse.set_disc_id          ( (*iter).disc_id()          );

      pulse.set_ped_mean   ( _reco_algo->ped_mean()            );
      pulse.set_ped_rms    ( _reco_algo->ped_mean()            );
      pulse.set_charge     ( _reco_algo->get_pulse(i)->area    );
      pulse.set_pulse_peak ( _reco_algo->get_pulse(i)->peak    );
      pulse.set_start_time ( _reco_algo->get_pulse(i)->t_start );
      pulse.set_max_time   ( _reco_algo->get_pulse(i)->t_max   );
      pulse.set_end_time   ( _reco_algo->get_pulse(i)->t_end   );

      pulses->push_back(pulse);

      event_charge    += _reco_algo->get_pulse(i)->area;
      event_amplitude += _reco_algo->get_pulse(i)->peak;

      npulse++;
    } // end of reco-ed pulse loop
  } // end of PMT waveform loop

  pulses->set_sum_charge ( event_charge    );
  pulses->set_sum_peak   ( event_amplitude );
  pulses->set_npulse     ( npulse          );

  return status;
  
}

bool pulse_reco::finalize(){

  return true;

}

#endif
