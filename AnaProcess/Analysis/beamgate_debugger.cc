#ifndef BEAMGATE_DEBUGGER_CC
#define BEAMGATE_DEBUGGER_CC

#include "beamgate_debugger.hh"

beamgate_debugger::beamgate_debugger() 
  : ana_base(), _pre_sample(), _post_sample()
{

  _name="beamgate_debugger";
  _hPrePedMean=0;
  _hPrePedRMS=0;
  
  _hPostPedMean=0;
  _hPostPedRMS=0;

  _hMaxADC=0;
  _hMaxTime=0;

  _hSampleSize=0;

  _pre_nsample=6;
  _post_nsample=6;

}

bool beamgate_debugger::initialize(){
  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__FUNCTION__,"called...");

  reset();
  prepare_histo();
  _pre_sample.reserve(_pre_nsample);
  _post_sample.reserve(_post_nsample);

  _ch_cnt=PMT::NUM_PMT_CHANNEL;
  _beam_window_width=PMT::BEAMGATE_NSAMPLE;
  _adc_max=PMT::MAX_ADC;  

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__FUNCTION__,"ends...");

  return true;
}

void beamgate_debugger::reset(){
  
  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__FUNCTION__,"called...");
  
  if(_hSampleSize)   delete  _hSampleSize;
  if(_hPrePedMean)   delete  _hPrePedMean;
  if(_hPrePedRMS)    delete  _hPrePedRMS;
  if(_hPostPedMean)  delete  _hPostPedMean;
  if(_hPostPedRMS)   delete  _hPostPedRMS;
  if(_hMaxADC)       delete  _hMaxADC;
  if(_hMaxTime)      delete  _hMaxTime;

  _hSampleSize=0;
  _hPrePedMean=0;
  _hPostPedMean=0;
  _hPrePedRMS=0;
  _hPostPedRMS=0;
  _hMaxADC=0;
  _hMaxTime=0;
  _pre_sample.clear();
  _post_sample.clear();

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__FUNCTION__,"ends...");  
}

void beamgate_debugger::prepare_histo(){

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__FUNCTION__,"called...");

  if(!_hSampleSize)
    _hSampleSize = new TH2D("hSampleSize",
			    "Number of ADC words; Channel Number; Number of words",
			    _ch_cnt,  -0.5, _ch_cnt-0.5,
			    int(_beam_window_width*1.5), -0.5, (double)(int(_beam_window_width*1.5)-0.5));
  else _hSampleSize->Reset();

  if(!_hPrePedMean)
    _hPrePedMean = new TH2D("hPrePedMean",
			      Form("Pedestal mean from first %d samples; Channel Number; Ped. Mean",
				   _pre_nsample),
			      _ch_cnt, -0.5, _ch_cnt-0.5,
			      80,2030,2070);
  else _hPrePedMean->Reset();

  if(!_hPostPedMean)
    _hPostPedMean = new TH2D("hPostPedMean",
			      Form("Pedestal mean from last %d samples; Channel Number; Ped. Mean",
				   _post_nsample),
			      _ch_cnt, -0.5, _ch_cnt-0.5,
			      80,2030,2070);
  else _hPostPedMean->Reset();

  if(!_hPrePedRMS)
    _hPrePedRMS = new TH2D("hPrePedRMS",
			   Form("Pedestal RMS from first %d samples; Channel Number; Ped. RMS",
				_pre_nsample),
			   _ch_cnt, -0.5, _ch_cnt-0.5,
			   40,0,20);
  else _hPrePedRMS->Reset();

  if(!_hPostPedRMS)
    _hPostPedRMS = new TH2D("hPostPedRMS",
			   Form("Pedestal RMS from first %d samples; Channel Number; Ped. RMS",
				_post_nsample),
			   _ch_cnt, -0.5, _ch_cnt-0.5,
			   40,0,20);
  else _hPostPedRMS->Reset();

  if(!_hMaxADC)
    _hMaxADC = new TH2D("hMaxADC",
			"Maximum ADC Counts; Channel Number; Max ADC",
			_ch_cnt, -0.5, _ch_cnt-0.5,
			_adc_max,-0.5,_adc_max-0.5);
  else _hMaxADC->Reset();

  if(!_hMaxTime)
    _hMaxTime = new TH2D("hMaxTime",
			"Maximum Time Counts; Channel Number; Max Time",
			_ch_cnt, -0.5, _ch_cnt-0.5,
			 100,0,_beam_window_width*1.5);
  else _hMaxTime->Reset();

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__FUNCTION__,"ends...");
}

bool beamgate_debugger::analyze(storage_manager* data){

  bool beam_wf=false;

  double ped_mean=0;
  double ped_rms=0;

  PMT::ch_adc_t max_adc=0;
  UShort_t max_time=0;
  UShort_t index=0;

  pmt_wf_collection *ewf = (pmt_wf_collection*)(data->get_data(DATA_STRUCT::PMT_WF_COLLECTION));

  for(std::vector<pmt_waveform>::const_iterator wf_iter(ewf->begin());
      wf_iter!=ewf->end();
      ++wf_iter){

    beam_wf=false;
    switch((*wf_iter).disc_id()) {
    case PMT::BEAM_DISC:
    case PMT::BEAM_WIN:
      beam_wf=true;
      break;
    case PMT::BASE_DISC:
    case PMT::COSMIC_DISC:
    case PMT::MICHEL_DISC:
      beam_wf=( (*wf_iter).size()>_beam_window_width );
      break;
    case PMT::DISC_MAX:
      break;
    }
    if(!beam_wf) continue;

    _hSampleSize->Fill((*wf_iter).channel_number()+1,((*wf_iter).size()));

    ped_mean=0;
    ped_rms=0;
    max_adc=0;
    max_time=0;
    index=0;

    _pre_sample.clear();
    _post_sample.clear();

    for(pmt_waveform::const_iterator adc_iter((*wf_iter).begin());
	adc_iter!=(*wf_iter).end();
	++adc_iter) {
      
      if( index < _pre_nsample )

	_pre_sample.push_back((double)((*adc_iter)));

      if( index >= ((*wf_iter).size() - _post_nsample) )

	_post_sample.push_back((double)((*adc_iter)));

      if( (*adc_iter) > max_adc ) {
	max_adc  = (*adc_iter);
	max_time = index;
      }
      
      index++;
    }

    get_ped_info(ped_mean,ped_rms,&_pre_sample);
    _hPrePedMean->Fill((*wf_iter).channel_number(), ped_mean);
    _hPrePedRMS->Fill((*wf_iter).channel_number(), ped_rms);

    get_ped_info(ped_mean,ped_rms,&_post_sample);
    _hPostPedMean->Fill((*wf_iter).channel_number(), ped_mean);
    _hPostPedRMS->Fill((*wf_iter).channel_number(), ped_rms);

    _hMaxADC->Fill((*wf_iter).channel_number(), max_adc);
    _hMaxTime->Fill((*wf_iter).channel_number(), max_time);
  }


  return true;
  
}

bool beamgate_debugger::finalize(){

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__FUNCTION__,"called...");

  _fout->cd();
  _hPrePedMean->Write();
  _hPrePedRMS->Write();
  _hPostPedMean->Write();
  _hPostPedRMS->Write();
  _hMaxADC->Write();
  _hMaxTime->Write();

  if(_verbosity[MSG::DEBUG])
    Message::send(MSG::DEBUG,__FUNCTION__,"ends...");

  return true;

}

#endif
