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
  pmtbaseline();
  
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
  void set_nsample_ped(UInt_t n){_bgpoints=n;};

  /** 
      Setter for # of samples to be used for pedestal calculation
      This sets # of samples to be used from the tail of the waveform
      in case pedestal sample from the beginning of the waveform is not good AND
      the user call use_tail function to allow this to happen.
  */
  void set_nsample_ped_tail(UInt_t n){_rdpoints=n;};

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
  /// Calculate mean using input number of samples
  double mean(std::vector<UShort_t>,UInt_t);
  /// Calculate standard deviation using input number of samples & mean
  double rms (std::vector<UShort_t>,UInt_t,double);
  /// Same as mean(), but uses samples from the tail
  double tailmean(std::vector<UShort_t>,UInt_t);
  /// Same as rms(), but uses samples from the tail.
  double tailrms (std::vector<UShort_t>,UInt_t,double);
  /// Sophisiticated time reconstruction algorithm by applying linear extrapolation of rising slope.
  double time_reconstructor(double, PMT::ch_waveform_t::const_iterator);

  //Histogram preparation
  void histosetup();

  //Histograms and graphs 
  TH2D *pedMean;   ///< pedestal mean vs. channels
  TH2D *pedRMS;    ///< pedestal RMS vs. channels
  TH2D *tailstudy; ///< pedestal mean vs. channels where mean is taken from tail samples

  TH1D *tailMean;  ///< 1D histogram of pedestal mean taken from tail samples
  TH1D *tailRMS;   ///< 1D histogram of pedestal rms taken from tail samples
  TH1D *channels;  ///< 1D histogram of participating channels
  TH1D *pedMeanAll;///< 1D histogram of pedetal mean (both from head & tail)
  TH1D *pedRMSAll; ///< 1D histogram of pedetal RMS (both from head & tail)

  TH1D *tailMeanCutrms; ///< 1D histogram of tail pedestal mean for RMS>0.5
  TH1D *tailRMSCutrms;  ///< 1D histogram of tail pedestal rms for RMS>0.5

  TH1D *pedMeanCutrms;  ///< 1D histogram of head pedestal mean for RMS>0.5
  TH1D *pedRMSCutrms;   ///< 1D histogram of head pedestal rms for RMS>0.5

  /*  
  TH1D *peakheights; ///< 1D histogram of pulse amplitude
  TH1D *peakareas;   ///< 1D histogram of pulse charge
  TH1D *nptstaken;   
  */
  TH1D *reco_time;
  TH1D *reco_time_diff;
  
  TGraphErrors *rdbadwaveforms;
  TGraphErrors *bgbadwaveforms;

  //Private vars
  bool _use_tail;
  bool _study_tail;
  
  UInt_t _bgpoints;   //Beam gate points
  UInt_t _rdpoints;   //Random tail points
  
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
  
