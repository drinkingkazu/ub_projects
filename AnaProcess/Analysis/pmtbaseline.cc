#ifndef PMTBASELINE_CC
#define PMTBASELINE_CC

#include "pmtbaseline.hh"
#include "pmt_waveform.hh"

pmtbaseline::pmtbaseline(){

  _name="pmtbaseline"; 
  _fout=0;

  _study_tail=false;
  _use_tail=false;
  _bgpoints  = 1;
  _rdpoints  = 1;
  _nsigma   = 5.0;  // 5 times standard deviaiton ~ 2.5 ADC count
  _min_peak = 5.0;  // 2.5 ADC count above baseline

  clear_event();

}

void pmtbaseline::clear_event(){
  _fpedmean  = 0.0;
  _fpedrms   = 0.0;
  _ftailmean = 0.0;
  _ftailrms  = 0.0;

}

double pmtbaseline::mean(std::vector<UShort_t> adcs,UInt_t bgpoints){
  double mean = 0;
  UShort_t holder = 0;
  size_t index=0;

  for(index=0;index<bgpoints && index<adcs.size() ;index++)
    holder += adcs.at(index);

  if( index!=bgpoints )
    Message::send(MSG::WARNING,__FUNCTION__,
		  Form("Waveform too short! Length: %d (need %d for baseline estimate).",(int)(adcs.size()),bgpoints));

  mean=((double)holder)/((double)index);
  return mean;
}
double pmtbaseline::rms(std::vector<UShort_t> adcs,UInt_t bgpoints, double mean){
  
  double rms    = 0;
  double holder = 0;
  size_t index  = 0;
  for(index=0;index<bgpoints && index<adcs.size();index++){
    holder += pow((double)adcs.at(index)-mean,2);
  }

  if( index!=bgpoints )
    Message::send(MSG::WARNING,__FUNCTION__,
		  Form("Waveform too short! Length: %d (need %d for baseline estimate).",(int)(adcs.size()),bgpoints));

  holder=holder/((double)index);
  
  rms = sqrt(holder);
  return rms;
}

//calculate begin time
double pmtbaseline::time_reconstructor(double fpedmean, PMT::ch_waveform_t::const_iterator adc_itr){
  double y1 = (*adc_itr)-fpedmean;
  adc_itr += 1;
  double y2 = (*adc_itr)-fpedmean;
  //x-axis measured in time_counter unit increments.
  //fine fraction of time_counter increments that designates start time
  double slope = (y2-y1)/1.0; //because Dx = 1 time_step
  //y = slope*x + x_0
  double time_reconstructed = y1/slope;
  return time_reconstructed;
}


double pmtbaseline::tailmean(std::vector<UShort_t> adcs,UInt_t rdpoints){
  double mean = 0;
  UShort_t holder = 0;
  size_t index=0;
  for(index=adcs.size()-1;index>=(adcs.size()-rdpoints); index--)
    holder += adcs.at(index);
  
  if( (adcs.size()-index-1)!=rdpoints )
    Message::send(MSG::WARNING,__FUNCTION__,
		  Form("Waveform too short! Length: %d (need %d for baseline estimate).",(int)(adcs.size()),rdpoints));

  mean=(double)holder/(double)rdpoints; 
  return mean;
    
}

double pmtbaseline::tailrms(std::vector<UShort_t> adcs,UInt_t rdpoints, double mean){
  
  double rms    = 0;
  double holder = 0;
  size_t index  = 0;
  for(index=adcs.size()-1;index>=(adcs.size()-rdpoints);index--)
    holder += pow((double)adcs.at(index)-mean,2)/rdpoints;

  if( (adcs.size()-index-1)!=rdpoints )
    Message::send(MSG::WARNING,__FUNCTION__,
		  Form("Waveform too short! Length: %d (need %d for baseline estimate).",(int)(adcs.size()),rdpoints));
  
  rms = sqrt(holder);
  return rms;
}

void pmtbaseline::histosetup(){
  
  pedMean     = new TH2D("bg_pedMean","Ped Mean (_bgpoints)",40,0,40,400,2000,2200);
  pedRMS      = new TH2D("bg_pedRMS","Ped RMS (bdpoints)",40,0,40,300,0,30);
  tailstudy   = new TH2D("rd_tailstudy","Tail Study",16,0,16,600,1900,2200);

  pedMeanAll  = new TH1D("bg_pedMeanAll","Ped Mean (all chan)",400 ,2000,2200 );
  pedRMSAll   = new TH1D("bg_pedRMSAll" ,"Ped RMS  (all chan)" ,300 ,0   ,30   );

  pedMeanCutrms  = new TH1D("bg_pedMeanCut","Ped Mean (rms>0.5 all chan)",400 ,2000,2200 );
  pedRMSCutrms   = new TH1D("bg_pedRMSCut" ,"Ped RMS  (rms>0.5 all chan)" ,300 ,0   ,30   );
 
  channels    = new TH1D("bg_channels","Beam Gate Channels",40,0,40); 
  tailMean    = new TH1D("rd_tailMean","Ped Tail Mean (_rdpoints)",400,2000,2200);
  tailRMS     = new TH1D("rd_tailRMS","Ped Tail RMS (_rdpoints)",100,0,10);
  
  tailMeanCutrms    = new TH1D("rd_tailMeanCut","Ped Tail Mean (rms>0.5)",400,2000,2200);
  tailRMSCutrms     = new TH1D("rd_tailRMSCut","Ped Tail RMS (rms>0.5)",100,0,10);
  reco_time = new TH1D("reco_time", "Reconstructed Start Time", 100, 0 , 50);
  reco_time_diff = new TH1D("reco_time_diff", "Reconstructed Start Time Diff: start - reco", 100, -5, 5);
  /*
    peakheights = new TH1D("peakheights","Subtracted Pulse Heights",500,0,1000);
    peakareas = new TH1D("peakareas","Areas",1125,0,5000);
    nptstaken = new TH1D("npointstake","Num. Points Taken for A.",40,0,80);
  */
}

bool pmtbaseline::initialize() {
  histosetup();  
  return true;
}

bool pmtbaseline::analyze(storage_manager* storage) {
  //New event
  clear_event();
  
  //Set points to hold number of kept points
  _max      = 15;
  
  double event_charge=0;
  double event_amplitude=0;
  int npulse=0;

  //Get event waveform from storage
  pmt_wf_collection   *ewform = (pmt_wf_collection*)(storage->get_data(DATA_STRUCT::PMT_WF_COLLECTION));
  pulse_collection *pulses = (pulse_collection*)(storage->get_data(DATA_STRUCT::PULSE_COLLECTION));
  
  //looping through all channels
  for(size_t i=0; i<ewform->size(); ++i){
    const pmt_waveform* pmt_data = &(ewform->at(i));

    if(pmt_data->size()<1){
      Message::send(MSG::ERROR,__FUNCTION__,
		    Form("Found 0-length waveform: Event %d ... Ch. %d",ewform->event_id(),pmt_data->channel_number()));
      continue;
    }

    _fpedmean = mean((*pmt_data),_bgpoints         );
    _fpedrms  = rms ((*pmt_data),_bgpoints,_fpedmean);      


    if(_fpedrms>0.5 && _use_tail){
      _fpedmean = tailmean((*pmt_data),_bgpoints         );
      _fpedrms  = tailrms ((*pmt_data),_bgpoints,_fpedmean);      
    } 
    // Mean rms can be still bad! But we continue...(warning)

    pedMean     -> Fill( pmt_data->channel_number(), _fpedmean );
    pedRMS      -> Fill( pmt_data->channel_number(), _fpedrms  );
    pedMeanAll  -> Fill(  _fpedmean            );
    pedRMSAll   -> Fill(  _fpedrms             );
    channels    -> Fill( pmt_data->channel_number() );
    
    //Cut on high RMS
    if(_fpedrms>0.5){
      pedMeanCutrms->Fill(_fpedmean);
      pedRMSCutrms ->Fill(_fpedrms);
      
      for(UShort_t k=0;k<(*pmt_data).size();k++){
	bg_waveforms.push_back((double)(*pmt_data).at(k));
	bgticks.push_back((double)k);
      }
    }
    
    bool fire = false;
    int time_counter = 0;
    double t_start   = 0;
    double t_start_reco = 0;
    double t_start_diff = 0;
    double t_max     = 0;
    double t_end     = 0;
    double a_pulse   = 0;
    double q_pulse   = 0;
    
    //Going through all bins in waveform
    for(PMT::ch_waveform_t::const_iterator adc_itr((*pmt_data).begin());
	adc_itr!=(*pmt_data).end(); ++adc_itr) {
      if(!fire && (*adc_itr)>(_nsigma*_fpedrms+_fpedmean) && (*adc_itr)>(_min_peak+_fpedmean)) {
	fire    = true;
	t_start = time_counter;

	t_start_diff = time_reconstructor(_fpedmean, adc_itr);
	t_start_reco = t_start + time_reconstructor(_fpedmean, adc_itr);

	reco_time->Fill(t_start_reco);
	reco_time_diff->Fill(t_start_diff);
	q_pulse = 0;
	a_pulse = 0;
      }
      
      if(fire) {
	if((*adc_itr)<(_nsigma*_fpedrms+_fpedmean) || (*adc_itr)<(_min_peak+_fpedmean)) {
	  npulse++;
	  fire  = false;
	  t_end = time_counter;  	  
	
	  pulse_info pulse;
	  pulse.set_ped_mean  ( _fpedmean );
	  pulse.set_ped_rms   ( _fpedrms  );
	  pulse.set_charge    ( q_pulse  );
	  pulse.set_pulse_peak( a_pulse  );
	  pulse.set_start_time( t_start  );
	  pulse.set_start_time_reco( t_start_reco );
	  pulse.set_end_time  ( t_end    );
	  pulse.set_max_time  ( t_max    );
	  pulse.set_channel_number(pmt_data->channel_number());
	  pulse.set_timeslice((*pmt_data).timeslice());
	  pulse.set_channel_frame_id((*pmt_data).channel_frame_id());
	  pulse.set_disc_id((*pmt_data).disc_id());
	  pulses->push_back(pulse);
	  
	  event_charge     += pulse.charge(); 
	  event_amplitude  += pulse.pulse_peak();
	  
	} else {
	  q_pulse += (*adc_itr) - _fpedmean;
	  if((*adc_itr) > (a_pulse + _fpedmean)) {
	    a_pulse= (*adc_itr) - _fpedmean;
	    t_max=time_counter;
	  }	
	}
	
      }//end fire
      
      
      time_counter++;
    } //End ADCs
    
    
    //Study tail
    if(_study_tail && i == 6){
      for(int k=3;k<=_max;++k)
	{
	  _ftailmean  = tailmean((*pmt_data),k);
	  _ftailrms   = tailrms ((*pmt_data),k,_ftailmean);
	  tailstudy -> Fill((double)k,_ftailmean);
	}
    }
    
    
  } //End Channels
  
  pulses->set_sum_charge(event_charge   );  
  pulses->set_sum_peak  (event_amplitude);  
  pulses->set_npulse    (npulse         );
  
  /*/// The following is for modifying pulse info ... NOT FOR CREATING!
    pulse_collection* pulse_group = ((storage_manager*)storage)->get_pulse_collection_writeable();
    for(pulse_collection::iterator iter(pulse_group->begin());
    iter!=pulse_group->end();
    ++iter)
    {
    (*iter).channel_number();
    (*iter).set_start_time(10);
    }*/
  
  
  return true;
}

bool pmtbaseline::finalize() {

  int n = rdticks.size();
  rdbadwaveforms = new TGraphErrors(n,&rdticks[0],&rd_waveforms[0]);
  rdbadwaveforms->SetName("rd_badwforms");
  rdbadwaveforms->SetTitle("ADC count vs. Time tick - Random");
  
  
  int t = bgticks.size();
  bgbadwaveforms = new TGraphErrors(t,&bgticks[0],&bg_waveforms[0]);
  bgbadwaveforms->SetName("bg_badwforms");
  bgbadwaveforms->SetTitle("ADC count vs. Time tick - Beam Gate");
  if(_fout){
  _fout          -> cd();
  pedMean        -> Write();
  pedRMS         -> Write();
  channels       -> Write();
  tailMean       -> Write();
  tailRMS        -> Write();
  tailstudy      -> Write();
  rdbadwaveforms -> Write();
  bgbadwaveforms -> Write();
  tailMeanCutrms -> Write();
  tailRMSCutrms  -> Write();
  reco_time      -> Write();
  reco_time_diff -> Write();
  /*  peakheights -> Write();
  peakareas   -> Write();
  nptstaken   -> Write();*/
  }else{
    Message::send(MSG::ERROR,__FUNCTION__,"Analysis output file not available!");
  }
  return true;
}

#endif
