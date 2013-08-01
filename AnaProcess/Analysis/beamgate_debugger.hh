/**
 * \file beamgate_debugger.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for beamgate_debugger class
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup Analysis

    @{*/
#ifndef BEAMGATE_DEBUGGER_HH
#define BEAMGATE_DEBUGGER_HH

#include <TH2D.h>

#include "event_waveform.hh"
#include "ana_base.hh"

/**
   \class beamgate_debugger
   Analysis module to perform a simple analysis meant to be a data quality
   check for beamgate events.
 */
class beamgate_debugger : public ana_base {

public:

  /// Default constructor
  beamgate_debugger();

  /// Default destructor
  virtual ~beamgate_debugger(){};

  /// Implementation of ana_base::initialize
  virtual bool initialize();

  /// Implementation of ana_base::analyze
  virtual bool analyze(storage_manager* data);

  /// Implementation of ana_base::finalize
  virtual bool finalize();
  
  /// Setter for maximum ADC count to be plotted
  void set_max_adc(PMT::ch_adc_t val){_adc_max=val;};

  /// Setter for beam window width
  void set_beam_window_size(PMT::word_t n){_beam_window_width=n;};

  /// Setter for number of samples to be used for pedestal estimation.
  /**
      This is taken from the beginning of the waveform.
   */
  void set_pre_ped_nsample(uint16_t n){_pre_nsample=n;};

  /// Setter for number of samples to be used for pedestal estimation.
  /**
     This is taken from the end of the waveform.
   */
  void set_post_ped_nsample(uint16_t n){_post_nsample=n;};

private:

  /// Initialize data members
  void reset();

  /// Prepare histograms
  void prepare_histo();

  /// A method to calculate pedestal mean and rms
  void get_ped_info(double &mean, double &rms, const std::vector<double> *sample)const {if(sample) {mean=0;rms=0;}};

  TH2D *_hSampleSize;
  TH2D *_hPrePedMean,  *_hPostPedMean;
  TH2D *_hPrePedRMS,   *_hPostPedRMS;
  TH2D *_hMaxADC;
  TH2D *_hMaxTime;

  PMT::ch_adc_t    _adc_max;
  PMT::ch_number_t _ch_cnt;
  PMT::word_t      _beam_window_width;

  uint16_t _pre_nsample;
  uint16_t _post_nsample;

  std::vector<double> _pre_sample, _post_sample;
  
};

#endif

/** @} */ // end of doxygen group 
