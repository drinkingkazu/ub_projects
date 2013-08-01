#ifndef PMTBASELINE_CC
#define PMTBASELINE_CC

#include "pmtbaseline.hh"
#include "event_waveform.hh"

void pmtbaseline::clear_event(){
  bgpoints  = 0;
  fpedmean  = 0.0;
  fpedrms   = 0.0;
  ftailmean = 0.0;
  ftailrms  = 0.0;
}

double pmtbaseline::mean(std::vector<uint16_t> adcs,uint32_t bgpoints){
  double mean = 0;
  uint16_t holder = 0;
  
  for(uint16_t u=0;u<bgpoints;u++)
    holder += adcs.at(u);
  
  mean=(double)holder/(double)bgpoints; 
  return mean;
}
double pmtbaseline::rms(std::vector<uint16_t> adcs,uint32_t bgpoints, double mean){
  
  double rms    = 0;
  double holder = 0;

  for(uint16_t u=0;u<bgpoints;u++){
    holder += pow((double)adcs.at(u)-mean,2)/bgpoints;
  }
  
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


double pmtbaseline::tailmean(std::vector<uint16_t> adcs,uint32_t rdpoints){
  double mean = 0;
  uint16_t holder = 0;
  
  for(uint16_t u=adcs.size()-1;u>=(adcs.size()-rdpoints);u--)
    holder += adcs.at(u);
  mean=(double)holder/(double)rdpoints; 
  return mean;
    
}

double pmtbaseline::tailrms(std::vector<uint16_t> adcs,uint32_t rdpoints, double mean){
  
  double rms    = 0;
  double holder = 0;
  for(uint16_t u=adcs.size()-1;u>=(adcs.size()-rdpoints);u--)
    holder += pow((double)adcs.at(u)-mean,2)/rdpoints;
  
  rms = sqrt(holder);
  return rms;
}

void pmtbaseline::histosetup(){
  
  pedMean     = new TH2D("bg_pedMean","Ped Mean (bgpoints)",40,0,40,400,2000,2200);
  pedRMS      = new TH2D("bg_pedRMS","Ped RMS (bdpoints)",40,0,40,300,0,30);
  tailstudy   = new TH2D("rd_tailstudy","Tail Study",16,0,16,600,1900,2200);

  pedMeanAll  = new TH1D("bg_pedMeanAll","Ped Mean (all chan)",400 ,2000,2200 );
  pedRMSAll   = new TH1D("bg_pedRMSAll" ,"Ped RMS  (all chan)" ,300 ,0   ,30   );

  pedMeanCutrms  = new TH1D("bg_pedMeanCut","Ped Mean (rms>0.5 all chan)",400 ,2000,2200 );
  pedRMSCutrms   = new TH1D("bg_pedRMSCut" ,"Ped RMS  (rms>0.5 all chan)" ,300 ,0   ,30   );
 
  channels    = new TH1D("bg_channels","Beam Gate Channels",40,0,40); 
  tailMean    = new TH1D("rd_tailMean","Ped Tail Mean (rdpoints)",400,2000,2200);
  tailRMS     = new TH1D("rd_tailRMS","Ped Tail RMS (rdpoints)",100,0,10);
  
  tailMeanCutrms    = new TH1D("rd_tailMeanCut","Ped Tail Mean (rms>0.5)",400,2000,2200);
  tailRMSCutrms     = new TH1D("rd_tailRMSCut","Ped Tail RMS (rms>0.5)",100,0,10);
  reco_time = new TH1D("reco_time", "Reconstructed Start Time", 100, 0 , 50);
  reco_time_diff = new TH1D("reco_time_diff", "Reconstructed Start Time Diff", 100, -5, 5);
  /*
  peakheights = new TH1D("peakheights","Subtracted Pulse Heights",500,0,1000);
  peakareas = new TH1D("peakareas","Areas",1125,0,5000);
  nptstaken = new TH1D("npointstake","Num. Points Taken for A.",40,0,80);
  */


}

bool pmtbaseline::initialize() {
  std::cout << "Initializing pmtbaseline analysis" << std::endl;
  std::cout << "Setting up histograms" << std::endl;
  histosetup();  
  return true;
}

bool pmtbaseline::analyze(storage_manager* storage) {
  //New event
  clear_event();
  
  //Set points to hold number of kept points
  bgpoints = 5;  //Beamgate
  rdpoints = 5;  //Random
  max      = 15;
  nsigma   = 5.0;
  min_peak = 3.0;
  
  double event_charge=0;
  double event_amplitude=0;
  int npulse=0;
  use_tail=false;

  //Get event waveform from storage
  event_waveform   *ewform = (event_waveform*)(storage->get_data(DATA_STRUCT::WF_COLLECTION));
  pulse_collection *pulses = (pulse_collection*)(storage->get_data(DATA_STRUCT::PULSE_COLLECTION));
  
  //looping through all channels
  for(size_t i=0; i<ewform->size(); ++i){
    const pmt_waveform* pmt_data = &(ewform->at(i));
    fpedmean = mean((*pmt_data),bgpoints         );
    fpedrms  = rms ((*pmt_data),bgpoints,fpedmean);      
    
    if(fpedrms>0.5 && use_tail){
      fpedmean = tailmean((*pmt_data),bgpoints         );
      fpedrms  = tailrms ((*pmt_data),bgpoints,fpedmean);      
    } 
    // Mean rms can be still bad! But we continue...(warning)
    
    pedMean     -> Fill( (double)i, fpedmean );
    pedRMS      -> Fill( (double)i, fpedrms  );
    pedMeanAll  -> Fill(  fpedmean            );
    pedRMSAll   -> Fill(  fpedrms             );
    channels    -> Fill( (double)i );
    
    //Cut on high RMS
    if(fpedrms>0.5){
      pedMeanCutrms->Fill(fpedmean);
      pedRMSCutrms ->Fill(fpedrms);
      
      for(uint16_t k=0;k<(*pmt_data).size();k++){
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
      if(!fire && (*adc_itr)>(nsigma*fpedrms+fpedmean) && (*adc_itr)>(min_peak+fpedmean)) {
	fire    = true;
	t_start = time_counter;
	t_start_reco = t_start + time_reconstructor(fpedmean, adc_itr);
	t_start_diff = t_start - t_start_reco;
	reco_time->Fill(t_start_reco);
	reco_time_diff->Fill(t_start_diff);
	q_pulse = 0;
	a_pulse = 0;
      }
      
      if(fire) {
	if((*adc_itr)<(nsigma*fpedrms+fpedmean) || (*adc_itr)<(min_peak+fpedmean)) {
	  npulse++;
	  fire  = false;
	  t_end = time_counter;  	  
	
	  pulse_info pulse;
	  pulse.set_ped_mean  ( fpedmean );
	  pulse.set_ped_rms   ( fpedrms  );
	  pulse.set_charge    ( q_pulse  );
	  pulse.set_pulse_peak( a_pulse  );
	  pulse.set_start_time( t_start  );
	  pulse.set_start_time_reco( t_start_reco );
	  pulse.set_end_time  ( t_end    );
	  pulse.set_max_time  ( t_max    );
	  pulse.set_channel_number((PMT::ch_number_t)i);
	  pulse.set_timeslice((*pmt_data).timeslice());
	  pulse.set_channel_frame_id((*pmt_data).channel_frame_id());
	  pulse.set_disc_id((*pmt_data).disc_id());
	  pulses->push_back(pulse);
	  
	  event_charge     += pulse.charge(); 
	  event_amplitude  += pulse.pulse_peak();
	  
	  
	
	} else {
	  q_pulse += (*adc_itr) - fpedmean;
	  if((*adc_itr) > (a_pulse + fpedmean)) {
	    a_pulse= (*adc_itr) - fpedmean;
	    t_max=time_counter;
	  }	
	}
	
      }//end fire
      
      
      time_counter++;
    } //End ADCs
    
    
    //Study tail
    if(study_tail && i == 6){
      for(int k=3;k<=max;++k)
	{
	  ftailmean  = tailmean((*pmt_data),k);
	  ftailrms   = tailrms ((*pmt_data),k,ftailmean);
	  tailstudy -> Fill((double)k,ftailmean);
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
  
  std::cout << "Finalizing pmtbaseline analysis" << std::endl;
  std::cout << "Writing histos to _fout         " << std::endl;
  
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
