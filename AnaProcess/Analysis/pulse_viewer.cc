#ifndef PULSE_VIEWER_CC
#define PULSE_VIEWER_CC

#include "pulse_viewer.hh"

pulse_viewer::pulse_viewer() { 
  _name="pulse_viewer"; 
  _cWF=0;
  _hWF=0;
  _lStart=0;
  _lEnd=0;
  _lBase=0;
  _lRMSTop=0;
  _lRMSBottom=0;
  _lTop=0;
  
  clear_viewer();
  reset_cuts();

}

void pulse_viewer::reset_cuts() {

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

void pulse_viewer::clear_viewer() {
  
  if(_hWF) delete _hWF;
  if(_lStart) delete _lStart;
  if(_lEnd) delete _lEnd;
  if(_lBase) delete _lBase;
  if(_lRMSTop) delete _lRMSTop;
  if(_lRMSBottom) delete _lRMSBottom;
  if(_lTop) delete _lTop;
}

bool pulse_viewer::initialize() {

  if(!_cWF) _cWF = new TCanvas("_cWF","",800,500);
  clear_map();
  clear_viewer();
  return true;
}

void pulse_viewer::clear_map(){

  _pulse_frame_id.clear();
  _pulse_sample_number.clear();
  _pulse_tstart.clear();
  _pulse_tstart_reco.clear();
  _pulse_tend.clear();
  _pulse_amp.clear();
  _pulse_charge.clear();
  _pulse_pedbase.clear();
  _pulse_pedrms.clear();
  _waveforms.clear();
  _pulse_count.clear();
  _channels.clear();

}

void pulse_viewer::add_channel_entry(PMT::ch_number_t ch){

  _channels.insert(ch);
  _pulse_frame_id[ch]=std::vector<PMT::word_t>();
  _pulse_sample_number[ch]=std::vector<PMT::word_t>();
  _pulse_tstart[ch]=std::vector<double>();
  _pulse_tstart_reco[ch]=std::vector<double>();
  _pulse_tend[ch]=std::vector<double>();
  _pulse_amp[ch]=std::vector<double>();
  _pulse_charge[ch]=std::vector<double>();
  _pulse_pedbase[ch]=std::vector<double>();
  _pulse_pedrms[ch]=std::vector<double>();
  //_waveforms[ch]=std::vector<std::vector<PMT::ch_adc_t> >();
  _pulse_count[ch]=std::make_pair(0,0);

  _waveforms[ch]=std::map<PMT::word_t,std::map<PMT::word_t,std::vector<PMT::ch_adc_t> > >();
}

bool pulse_viewer::analyze(storage_manager* storage) {
  
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

  // Check if this event is in the range of users' interest
  if(_event_id < _cut_event_id.first || _cut_event_id.second < _event_id)
    return true;
  else if(_sum_charge < _cut_sum_charge.first || _cut_sum_charge.second < _sum_charge)
    return true;
  else if(_sum_peak < _cut_sum_peak.first || _cut_sum_peak.second < _sum_peak)
    return true;
  else if(_npulse < _cut_npulse.first || _cut_npulse.second < _npulse)
    return true;

  // Clear map
  clear_map();

  // Fill map for this event
  for(pulse_collection::const_iterator iter(pulses->begin());
      iter!=pulses->end();
      ++iter){

    PMT::ch_number_t ch((*iter).channel_number());
    PMT::word_t sample   = (*iter).timeslice();
    PMT::word_t frame    = (*iter).frame_id();
    double      t_start  = (*iter).start_time();
    double      t_end    = (*iter).end_time();
    double      charge   = (*iter).charge();
    double      amp      = (*iter).pulse_peak();
    double      ped_base = (*iter).ped_mean();
    double      ped_rms  = (*iter).ped_rms();
    double      t_start_reco = (*iter).start_time_reco();

    // Check if this pulse passes the criteria
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

    if(_channels.find(ch)==_channels.end())
      add_channel_entry(ch);

    _pulse_frame_id[ch].push_back(frame);
    _pulse_sample_number[ch].push_back(sample);
    _pulse_tstart[ch].push_back(t_start);
    _pulse_tstart_reco[ch].push_back(t_start_reco);
    _pulse_tend[ch].push_back(t_end);
    _pulse_amp[ch].push_back(amp);
    _pulse_charge[ch].push_back(charge);
    _pulse_pedbase[ch].push_back(ped_base);
    _pulse_pedrms[ch].push_back(ped_rms);
    
    //_waveforms[ch].push_back(std::vector<PMT::ch_adc_t>());
    if(_waveforms[ch].find(frame)==_waveforms[ch].end())
      _waveforms[ch][frame]=std::map<PMT::word_t,std::vector<PMT::ch_adc_t> >();

    if(_waveforms[ch][frame].find(sample) == _waveforms[ch][frame].end())
      _waveforms[ch][frame][sample]=std::vector<PMT::ch_adc_t>();

    _pulse_count[ch].first+=1;
  }

  for(event_waveform::const_iterator ch_iter(wfs->begin());
      ch_iter!=wfs->end();
      ++ch_iter){
    PMT::ch_number_t ch        = (*ch_iter).channel_number();
    PMT::word_t this_timeslice = (*ch_iter).timeslice();
    PMT::word_t this_frame_id  = (*ch_iter).channel_frame_id();

    if(_waveforms.find(ch) == _waveforms.end())
      continue;
    if(_waveforms[ch].find(this_frame_id)==_waveforms[ch].end())
      continue;
    if(_waveforms[ch][this_frame_id].find(this_timeslice) == _waveforms[ch][this_frame_id].end())
      continue;

    if(_waveforms[ch][this_frame_id][this_timeslice].size()) {
      sprintf(_buf,"Found already filled waveform (ch=%d, frame=%d, sample=%d)",
	      ch,this_frame_id,this_timeslice);
      Message::send(MSG::ERROR,__FUNCTION__,_buf);
      continue;
    }

    // Fill the waveform vector
    for(pmt_waveform::const_iterator adc_iter((*ch_iter).begin());
	adc_iter!=(*ch_iter).end();
	++adc_iter){      
      _waveforms[ch][this_frame_id][this_timeslice].push_back((*adc_iter));
    }
        
  } // Finieh processing for this channel.

  _ch_iter=_channels.begin();

  return true;
}

short pulse_viewer::get_npulse(PMT::ch_number_t ch){
  
  if(_channels.find(ch)==_channels.end())
    return 0;

  return _pulse_count[ch].first;

}

PMT::ch_number_t pulse_viewer::next_channel(){

  PMT::ch_number_t ch=PMT::INVALID_CH;

  if(_ch_iter==_channels.end())
    return ch;

  else
    ch=(*_ch_iter);
  
  _ch_iter++;

  return ch;
}

TH1D* pulse_viewer::next_pulse(PMT::ch_number_t ch){

  if(_channels.find(ch)==_channels.end()){
    sprintf(_buf,"No pulse found for channel %d!",ch);
    Message::send(MSG::ERROR,__FUNCTION__,_buf);
    return 0;
  }

  short this_index = _pulse_count[ch].second;

  if(this_index >= _pulse_count[ch].first){
    sprintf(_buf,"No more pulse found on ch %d... (%d/%d)",ch,this_index,_pulse_count[ch].first);
    Message::send(MSG::NORMAL,__FUNCTION__,_buf);
    return 0;
  }
  else if(this_index < 0) {
    sprintf(_buf,"First pulse on Ch: %d",ch);
    Message::send(MSG::INFO,__FUNCTION__,_buf);
    this_index=0;
    _pulse_count[ch].second=this_index+1;
  }else{
    sprintf(_buf,"Pulse index %d for ch. %d",this_index,ch);
    Message::send(MSG::NORMAL,__FUNCTION__,_buf);
    _pulse_count[ch].second+=1;
  }

  return get_waveform(ch,(size_t)this_index);

}


TH1D* pulse_viewer::previous_pulse(PMT::ch_number_t ch){

  if(_channels.find(ch)==_channels.end()){
    sprintf(_buf,"No pulse found for channel %d!",ch);
    Message::send(MSG::ERROR,__FUNCTION__,_buf);
    return 0;
  }

  short this_index = _pulse_count[ch].second;

  if(this_index < 0){
    sprintf(_buf,"No more pulse found on ch %d... (%d/%d)",ch,this_index,_pulse_count[ch].first);
    Message::send(MSG::NORMAL,__FUNCTION__,_buf);
    return 0;
  }
  else if(this_index >= _pulse_count[ch].first) {
    sprintf(_buf,"Last pulse on Ch: %d",ch);
    Message::send(MSG::INFO,__FUNCTION__,_buf);
    this_index=_pulse_count[ch].first-1;
    _pulse_count[ch].second=this_index-1;
  }else{
    sprintf(_buf,"Pulse index %d for ch. %d",this_index,ch);
    Message::send(MSG::NORMAL,__FUNCTION__,_buf);
    _pulse_count[ch].second-=1;
  }

  return get_waveform(ch,(size_t)this_index);
  
}


TH1D* pulse_viewer::get_waveform(PMT::ch_number_t ch, size_t index) {

  clear_viewer();
  
  bool is_wf_stored=true;
  PMT::word_t sample=_pulse_sample_number[ch][index];
  PMT::word_t frame =_pulse_frame_id[ch][index];

  if(_waveforms.find(ch)==_waveforms.end())
    is_wf_stored=false;
  else if(_waveforms[ch].find(frame)==_waveforms[ch].end())
    is_wf_stored=false;
  else if(_waveforms[ch][frame].find(sample)==_waveforms[ch][frame].end())
    is_wf_stored=false;

  if(!is_wf_stored){
    sprintf(_buf,"Waveform not stored for ch=%d, frame=%d, sample=%d!",
	    ch,frame,sample);
    Message::send(MSG::ERROR,__FUNCTION__,_buf);
    return 0;
  }

  _hWF = new TH1D("hWF",
		  Form("Channel %d Reconstructed Pulse; Time-Slice; Amplitude",ch),
		  _waveforms[ch][frame][sample].size(),
		  -0.5,
		  (double)(_waveforms[ch][frame][sample].size())-0.5);

  for(size_t adc_index=0; adc_index<_waveforms[ch][frame][sample].size(); ++adc_index)
    _hWF->SetBinContent(adc_index+1,_waveforms[ch][frame][sample][adc_index]);

  //double x_min=_pulse_tend[ch][index]-20;
  //double x_max=_pulse_tend[ch][index]+20;
  double y_min=_pulse_pedbase[ch][index] - _pulse_pedrms[ch][index]*5 - 20;
  double y_max=_pulse_pedbase[ch][index] + _pulse_amp[ch][index]*1.5;
  
  _hWF->SetFillColor(kGray);
  _hWF->SetFillStyle(3004);
  _hWF->SetLineWidth(2);
  _hWF->SetMinimum(y_min);
  _hWF->SetMaximum(y_max);
  //_hWF->GetXaxis()->SetRangeUser(x_min,x_max);
    
  sprintf(_buf,"\n\n");
  sprintf(_buf,"%s Event ID       : %d\n",_buf,_event_id);
  sprintf(_buf,"%s Channel        : %d\n",_buf,ch);
  sprintf(_buf,"%s Start T        : %g\n",_buf,_pulse_tstart[ch][index]);
  sprintf(_buf,"%s Start T (RECO) : %g\n",_buf,_pulse_tstart_reco[ch][index]);
  sprintf(_buf,"%s End T          : %g\n",_buf,_pulse_tend[ch][index]);
  sprintf(_buf,"%s Ped. Mean      : %g\n",_buf,_pulse_pedbase[ch][index]);
  sprintf(_buf,"%s Ped. RMS       : %g\n",_buf,_pulse_pedrms[ch][index]);
  sprintf(_buf,"%s Peak Amp.      : %g\n",_buf,_pulse_amp[ch][index]);
  sprintf(_buf,"%s Charge         : %g\n",_buf,_pulse_charge[ch][index]);
  Message::send(MSG::NORMAL,__FUNCTION__,_buf);

  _lBase = new TLine(_hWF->GetXaxis()->GetXmin(), _pulse_pedbase[ch][index], 
		     _hWF->GetXaxis()->GetXmax(), _pulse_pedbase[ch][index]);
  _lBase->SetLineWidth(2);
  _lBase->SetLineStyle(2);
  _lBase->SetLineColor(kBlack);

  _lRMSTop = new TLine(_hWF->GetXaxis()->GetXmin(), _pulse_pedbase[ch][index] + _pulse_pedrms[ch][index],
		       _hWF->GetXaxis()->GetXmax(), _pulse_pedbase[ch][index] + _pulse_pedrms[ch][index]);
  _lRMSTop->SetLineWidth(2);
  _lRMSTop->SetLineStyle(2);
  _lRMSTop->SetLineColor(kGray);

  _lRMSBottom = new TLine(_hWF->GetXaxis()->GetXmin(), _pulse_pedbase[ch][index] - _pulse_pedrms[ch][index],
			  _hWF->GetXaxis()->GetXmax(), _pulse_pedbase[ch][index] - _pulse_pedrms[ch][index]);
  _lRMSBottom->SetLineWidth(2);
  _lRMSBottom->SetLineStyle(2);
  _lRMSBottom->SetLineColor(kGray);

  _lStart = new TLine(_pulse_tstart[ch][index], _pulse_pedbase[ch][index],
		      _pulse_tstart[ch][index], _pulse_pedbase[ch][index]+_pulse_amp[ch][index]);
  _lStart->SetLineWidth(2);
  _lStart->SetLineStyle(2);
  _lStart->SetLineColor(kBlue);

  _lEnd = new TLine(_pulse_tend[ch][index], _pulse_pedbase[ch][index],
		    _pulse_tend[ch][index], _pulse_pedbase[ch][index]+_pulse_amp[ch][index]);
  _lEnd->SetLineWidth(2);
  _lEnd->SetLineStyle(2);
  _lEnd->SetLineColor(kRed);

  _lTop = new TLine(_pulse_tstart[ch][index], _pulse_pedbase[ch][index] + _pulse_amp[ch][index],
		    _pulse_tend[ch][index],   _pulse_pedbase[ch][index] + _pulse_amp[ch][index]);
  _lTop->SetLineWidth(2);
  _lTop->SetLineStyle(2);
  _lTop->SetLineColor(kMagenta-7);
  
  _cWF->cd();
  _hWF->Draw();
  _lBase->Draw();
  _lStart->Draw();
  _lEnd->Draw();
  _lRMSTop->Draw();
  _lRMSBottom->Draw();
  _lTop->Draw();

  _cWF->Modified();
  _cWF->Update();

  _cWF->Draw();

  return _hWF;
}

bool pulse_viewer::finalize() {

  // This function is called at the end of event loop.
  // Do all variable finalization you wish to do here.
  // If you need, you can store your ROOT class instance in the output
  // file. You have an access to the output file through "_fout" pointer.
  delete _cWF;
  clear_viewer();
  return true;
}

void pulse_viewer::display_cut_ranges(){

  std::string msg("Displaying the currently set values for cuts...\n");

  msg = "\n";
  msg += Form(" Event-wise parameters...\n");
  msg += Form("    Event ID            : %d -> %d\n",_cut_event_id.first,_cut_event_id.second);
  msg += Form("    Summed Charge       : %g -> %g\n",_cut_sum_charge.first,_cut_sum_charge.second);
  msg += Form("    Summed Peak         : %g -> %g\n",_cut_sum_peak.first,_cut_sum_peak.second);
  msg += Form("    Num. Pulses         : %d -> %d\n",_cut_npulse.first,_cut_npulse.second);
  msg += "\n";
  msg += Form(" Pulse-wise parameters...\n");
  msg += Form("    Pulse Charge        : %g -> %g\n",_cut_charge.first,_cut_charge.second);
  msg += Form("    Pulse Peak          : %g -> %g\n",_cut_amp.first,_cut_amp.second);
  msg += Form("    Pulse Start T       : %g -> %g\n",_cut_tstart.first,_cut_tstart.second);
  msg += Form("    Pulse Start T (RECO): %g -> %g\n",_cut_tstart_reco.first,_cut_tstart_reco.second);
  msg += Form("    Pulse End T         : %g -> %g\n",_cut_tend.first,_cut_tend.second);
  msg += Form("    Pedestal Mean       : %g -> %g\n",_cut_pedbase.first,_cut_pedbase.second);
  msg += Form("    Pedestal RMS        : %g -> %g\n",_cut_pedrms.first,_cut_pedrms.second);

  Message::send(MSG::NORMAL,__FUNCTION__,msg.c_str());

}

#endif
