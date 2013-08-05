/**
 * \file pmtbaseline.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class pmtbaseline
 *
 * @author vic.genty
 */

/** \addtogroup Analysis

    @{*/

#ifndef PMTBASELINE_HH
#define PMTBASELINE_HH
#include <map>
#include <vector>
#include "ana_base.hh"
#include <TH1D.h>
#include <TH2D.h>
#include <math.h>
#include <TGraph.h>
#include <TGraphErrors.h>
/**
   \class pmtbaseline
   User custom analysis class made by vic.genty
*/
class pmtbaseline : public ana_base{

public:
  
  /// Default constructor
  pmtbaseline(){ _name="pmtbaseline"; _fout=0;};
  
  /// Default destructor
  virtual ~pmtbaseline(){};  

  /// Initialize method to be called before the event loop
  virtual bool initialize();

  /// Analyze method to be called per event in a loop
  virtual bool analyze(storage_manager* storage);

  /// Finalize method to be called after the event loop
  virtual bool finalize();

  /**
     Setter for # of samples to be used for pedestal calculation.
     By default this sample is taken from the beginning of the waveform
  */
  void set_nsample_ped(uint32_t n){_bgpoints=n;};

  /** 
      Setter for # of samples to be used for pedestal calculation
      This sets # of samples to be used from the tail of the waveform
      in case pedestal sample from the beginning of the waveform is not good AND
      the user call use_tail function to allow this to happen.
  */
  void set_nsample_ped_tail(uint32_t n){_rdpoints=n;};

  /// Sets number of standard deviations from baseline mean to claim a pulse
  void set_nsigma_threshold(double thres){_nsigma=thres;};

  /// Sets absolute adc count (baseline subtracted, hence double type) to claim a pulse
  void set_adc_threshold(double thres){_min_peak=thres;};

  /// Sets boolean to use the tial pedestal sample
  void set_use_tail(bool use){_use_tail=use;};

private:
  //Clear for each new run
  void clear_event();
  
  //Functions
  double mean(std::vector<uint16_t>,uint32_t);
  double rms (std::vector<uint16_t>,uint32_t,double);
  double tailmean(std::vector<uint16_t>,uint32_t);
  double tailrms (std::vector<uint16_t>,uint32_t,double);
  double time_reconstructor(double, PMT::ch_waveform_t::const_iterator);

  //Histogram preparation
  void histosetup();

  //Histograms and graphs 
  TH2D *pedMean;
  TH2D *pedRMS;
  TH2D *tailstudy;

  TH1D *tailMean;
  TH1D *tailRMS;  
  TH1D *channels;
  TH1D *pedMeanAll;
  TH1D *pedRMSAll;

  TH1D *tailMeanCutrms;
  TH1D *tailRMSCutrms;

  TH1D *pedMeanCutrms;
  TH1D *pedRMSCutrms;

  
  TH1D *peakheights;
  TH1D *peakareas;
  TH1D *nptstaken;
  TH1D *reco_time;
  TH1D *reco_time_diff;
  
  TGraphErrors *rdbadwaveforms;
  TGraphErrors *bgbadwaveforms;

  //Private vars
  bool _use_tail;
  bool _study_tail;
  
  uint32_t _bgpoints;   //Beam gate points
  uint32_t _rdpoints;   //Random tail points
  
  double _ftailmean;    //Random ped mean
  double _ftailrms;     //Random ped rms
  double _fpedmean;     //Beamgate ped mean
  double _fpedrms;      //Beamgate ped rms
 
  double _nsigma;       // number of sigma offset from baseline to claim a pulse
  double _min_peak;     // absolute adc count threshold (baseline subtracted) to claim a pulse
  int _max;             // maximum amplitude of a pulse

  
  //Vectors for TGraphs
  std::vector<double> rd_waveforms;
  std::vector<double> rdticks;
  
  std::vector<double> bg_waveforms;
  std::vector<double> bgticks;
  
};
  
#endif
  
		    /** @} */ // end of doxygen group 
  
