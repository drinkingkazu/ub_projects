/**
 * \file pmtbaseline_ana.hh
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class pmtbaseline_ana
 *
 * @author kazuhiro
 */

/** \addtogroup Analysis

    @{*/

#ifndef PMTBASELINE_ANA_HH
#define PMTBASELINE_ANA_HH

#include "ana_base.hh"
#include <map>
#include <TH1D.h>

/**
   \class pmtbaseline_ana
   User custom analysis class made by kazuhiro
 */
class pmtbaseline_ana : public ana_base{

public:

  /// Default constructor
  pmtbaseline_ana();

  /// Default destructor
  virtual ~pmtbaseline_ana(){};

  /** IMPLEMENT in pmtbaseline_ana.cc!
      Initialization method to be called before the analysis event loop.
  */ 
  virtual bool initialize();

  /** IMPLEMENT in pmtbaseline_ana.cc! 
      Analyze a data event-by-event  
  */
  virtual bool analyze(storage_manager* storage);

  /** IMPLEMENT in pmtbaseline_ana.cc! 
      Finalize method to be called after all events processed.
  */
  virtual bool finalize();

  /// Setter for the range of pusle start time to select pulses of interest
  void set_range_pulse_start_time(double v1, double v2)   {_cut_tstart.first=v1;   _cut_tstart.second=v2;   };

  /// Setter for the range of reconstructed pusle start time to select pulses of interest
  void set_range_pulse_start_time_reco(double v1, double v2)   {_cut_tstart_reco.first=v1;   _cut_tstart_reco.second=v2;   };

  /// Setter for the range of pusle end time to select pulses of interest..
  void set_range_pulse_end_time(double v1, double v2)     {_cut_tend.first=v1;     _cut_tend.second=v2;     };

  /// Setter for the range of pusle amplitude to select pulses of interest..
  void set_range_pulse_amp(double v1, double v2)          {_cut_amp.first=v1;      _cut_amp.second=v2;      };

  /// Setter for the range of integrated charge to select pulses of interest..
  void set_range_pulse_charge(double v1,double v2)        {_cut_charge.first=v1;   _cut_charge.second=v2;   };

  /// Setter for the range of pedestal mean value to select pulses of interest..
  void set_range_ped_mean(double v1, double v2)           {_cut_pedbase.first=v1;  _cut_pedbase.second=v2;  };

  /// Setter for the range of pedestal rms to select pulses of interest..
  void set_range_ped_rms(double v1, double v2)            {_cut_pedrms.first=v1;   _cut_pedrms.second=v2;   };

  /// Setter for the range of event id to select pulses of interest.
  void set_range_event_id(PMT::word_t v1, PMT::word_t v2) {_cut_event_id.first=v1; _cut_event_id.second=v2; };

  /// Setter for the range of channel numbers to select pulses of interest.
  void set_range_channel(PMT::ch_number_t v1, PMT::ch_number_t v2) {_cut_channels.first=v1; _cut_channels.second=v2;};

  /// Setter for the range of event charge sum to select pulses of interest.
  void set_range_sum_charge(double v1, double v2)         {_cut_sum_charge.first=v1; _cut_sum_charge.second=v2;};

  /// Setter for the range of event peak sum to select pulses of interest.
  void set_range_sum_peak(double v1, double v2)           {_cut_sum_peak.first=v1; _cut_sum_peak.second=v2;};

  /// Setter for the range of event-wise number of pulses to select pulses of interest.
  void set_range_npulse(double v1, double v2)             {_cut_npulse.first=v1; _cut_npulse.second=v2;};

  //
  // Getters
  //

  /// Getter for the range of pusle start time to select pulses of interest
  const std::pair<double,double> range_pulse_start_time() const   {return _cut_tstart;};

  /// Getter for the range of pusle reconstructed start time to select pulses of interest
  const std::pair<double,double> range_pulse_start_time_reco() const   {return _cut_tstart_reco;};

  /// Getter for the range of pusle end time to select pulses of interest..
  const std::pair<double,double> range_pulse_end_time() const     {return _cut_tend;};

  /// Getter for the range of pusle amplitude to select pulses of interest..
  const std::pair<double,double> range_pulse_amp() const          {return _cut_amp;};

  /// Getter for the range of integrated charge to select pulses of interest..
  const std::pair<double,double> range_pulse_charge() const       {return _cut_charge;};

  /// Getter for the range of pedestal mean value to select pulses of interest..
  const std::pair<double,double> range_ped_mean() const           {return _cut_pedbase;};

  /// Getter for the range of pedestal rms to select pulses of interest..
  const std::pair<double,double> range_ped_rms() const            {return _cut_pedrms;};

  /// Getter for the range of event id to select pulses of interest.
  const std::pair<PMT::word_t,PMT::word_t> range_event_id() const {return _cut_event_id;};

  /// Getter for the range of channel numbers to select pulses of interest.
  const std::pair<PMT::word_t,PMT::word_t> range_channel() const  {return _cut_channels;};

  /// Getter for the range of event charge sum to select pulses of interest.
  const std::pair<double,double> range_sum_charge() const         {return _cut_sum_charge;};

  /// Getter for the range of event peak sum to select pulses of interest.
  const std::pair<double,double> range_sum_peak() const           {return _cut_sum_peak;};

  /// Getter for the range of event-wise number of pulses to select pulses of interest.
  const std::pair<UInt_t,UInt_t> range_npulse() const         {return _cut_npulse;};

protected:

  /// reset cut values
  void reset_cuts();

  std::map<PMT::ch_number_t,TH1D*> _hArray_PedMean, _hArray_PedRMS, _hArray_Charge, _hArray_Peak;

  /// pairs to define a range of pulse parameters for displaying pulses.
  std::pair<double,double> _cut_tstart, _cut_tstart_reco, _cut_tend, _cut_amp, _cut_charge, _cut_pedbase, _cut_pedrms;

  /// pairs to define a range of event parameters for displaying pulses.
  std::pair<double,double> _cut_sum_charge, _cut_sum_peak;

  /// pairs to define a range of event parameters for displaying pulses.
  std::pair<UInt_t,UInt_t> _cut_npulse;  

  /// a pair to define a range of event id for displaying pulses.
  std::pair<PMT::word_t,PMT::word_t> _cut_event_id;

  /// a pair to define a range of channel numbers for displaying pulses.
  std::pair<PMT::ch_number_t,PMT::ch_number_t> _cut_channels;

};

#endif

/** @} */ // end of doxygen group 
