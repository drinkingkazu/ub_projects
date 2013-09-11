#ifndef ALGO_THRESHOLD_HH
#define ALGO_THRESHOLD_HH

#include "preco_algo_base.hh"

class algo_threshold : public preco_algo_base {

public:

  algo_threshold();

  virtual ~algo_threshold(){};
 
  virtual bool reco(const std::vector<uint16_t> *wf);

  virtual void reset();

  void set_adc_threshold(double v) {_adc_thres = v;};

  void set_nsigma(double v) {_nsigma = v;};
  
protected:

  double _adc_thres;

  double _nsigma;

};

#endif
