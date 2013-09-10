#ifndef PED_ESTIMATOR_HH
#define PED_ESTIMATOR_HH

#include "Base-TypeDef.hh"
#include "DataFormat-TypeDef.hh"
#include <cmath>

class ped_estimator : public decoder_base {

public:

  ped_estimator();

  ~ped_estimator(){};

  void set_nsample_head(size_t n){ _nsample_head = n;};

  void set_nsample_tail(size_t n){ _nsample_tail = n;};

  void analyze_head(const std::vector<uint16_t>*wf);

  void analyze_tail(const std::vector<uint16_t>*wf);

  double mean_head() const { return _mean_head;};

  double rms_head() const { return _rms_head;};

  double mean_tail() const { return _mean_tail;};

  double rms_tail() const { return _rms_tail;};


protected:

  size_t _nsample_head;
  size_t _nsample_tail;

  double _mean_head, _rms_head;
  double _mean_tail, _rms_tail;

};

#endif

