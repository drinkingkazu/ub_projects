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
  virtual bool initialize();
  virtual bool analyze(const storage_manager* storage);
  virtual bool finalize();
  
  
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
  bool use_tail;
  bool study_tail;
  
  uint32_t bgpoints;   //Beam gate points
  uint32_t rdpoints;   //Random tail points
  
  double ftailmean;    //Random ped mean
  double ftailrms;     //Random ped rms
  double fpedmean;     //Beamgate ped mean
  double fpedrms;      //Beamgate ped rms
 
  double nsigma;

  double min_peak;
  
  int max;

  
  //Vectors for TGraphs
  std::vector<double> rd_waveforms;
  std::vector<double> rdticks;
  
  std::vector<double> bg_waveforms;
  std::vector<double> bgticks;
  
};
  
#endif
  
		    /** @} */ // end of doxygen group 
  
