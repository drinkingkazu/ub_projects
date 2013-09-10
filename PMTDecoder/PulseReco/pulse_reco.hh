#ifndef PULSE_RECO_HH
#define PULSE_RECO_HH

#include "ana_base.hh"
#include "preco_algo_base.hh"
#include "ped_estimator.hh"

class pulse_reco : public ana_base {

public:

  enum PED_METHOD{
    kHEAD = 0, ///< Use first N samples
    kTAIL,     ///< Use last N samples
    kBOTH      ///< Calculate both and use the one with smaller RMS
  };

public:

  pulse_reco() : ana_base(), _ped_algo() { _reco_algo = 0; };

  ~pulse_reco(){};

  virtual bool initialize();

  virtual bool analyze(storage_manager* storage);

  virtual bool finalize();

  void set_reco_algo(preco_algo_base* algo){_reco_algo = algo;};

private:

  preco_algo_base *_reco_algo;

  ped_estimator _ped_algo;

  PED_METHOD _ped_method;
  
};

#endif
