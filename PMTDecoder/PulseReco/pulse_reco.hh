#ifndef PULSE_RECO_HH
#define PULSE_RECO_HH

#include "ana_base.hh"
#include "preco_algo_base.hh"
#include "ped_estimator.hh"

#include <TH1D.h>
#include <TH2D.h>

class pulse_reco : public ana_base {

public:

  enum PED_METHOD{
    kHEAD = 0, ///< Use first N samples
    kTAIL,     ///< Use last N samples
    kBOTH      ///< Calculate both and use the one with smaller RMS
  };

public:

  pulse_reco() 
    : ana_base(), 
      _ped_algo() 

  { _reco_algo = 0; 
    _ped_method = kHEAD;
    _name="pulse_reco";
  };

  ~pulse_reco(){};

  virtual bool initialize();

  virtual bool analyze(storage_manager* storage);

  virtual bool finalize();

  void set_reco_algo        (preco_algo_base* algo){_reco_algo = algo;};

  void set_ped_algo         (PED_METHOD type){_ped_method = type;};

  void set_ped_nsample_head (size_t n){ _ped_algo.set_nsample_head(n); };

  void set_ped_nsample_tail (size_t n){ _ped_algo.set_nsample_tail(n); };

private:

  preco_algo_base *_reco_algo;

  ped_estimator _ped_algo;

  PED_METHOD _ped_method;

  std::map<PMT::ch_number_t,double> _channel_Q_m, _channel_A_m;

private:

  TH2D *_ped_Mean, *_ped_RMS;

  TH2D *_channel_Q, *_channel_A;

  TH1D *_event_Q, *_event_A;
  
};

#endif
