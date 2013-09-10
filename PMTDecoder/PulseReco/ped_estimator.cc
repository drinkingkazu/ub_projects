#ifndef PED_ESTIMATOR_CC
#define PED_ESTIMATOR_CC

#include "ped_estimator.hh"

ped_estimator::ped_estimator(){

  _nsample_head = _nsample_tail = 0;

  _mean_head = _rms_head = -1;

  _mean_tail = _rms_tail = -1;

}

void ped_estimator::analyze_head(const std::vector<uint16_t> *wf){

  _mean_head = 0;
  _rms_head  = 0;

  for(size_t index=0; index < _nsample_head; ++index)

    _mean_head += wf->at(index);

  _mean_head = _mean_head / (double)(_nsample_head);

  for(size_t index=0; index < _nsample_head; ++index)

    _rms_head += pow( (wf->at(index) - _mean_head), 2 );

  _rms_head = sqrt(_rms_head/((double)(_nsample_head)));

}

void ped_estimator::analyze_tail(const std::vector<uint16_t> *wf){

  _mean_tail = 0;
  _rms_tail  = 0;

  for(size_t index=wf->size()-1; index > (wf->size()-1-_nsample_tail); --index)

    _mean_tail += wf->at(index);

  _mean_tail = _mean_tail / (double)(_nsample_tail);

  for(size_t index=wf->size()-1; index > ( (wf->size()) - 1 - _nsample_tail); --index)

    _rms_tail += pow( (wf->at(index) - _mean_tail), 2 );

  _rms_tail = sqrt(_rms_tail/((double)(_nsample_tail)));

}


#endif
