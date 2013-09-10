#ifndef ALGO_FIXED_WINDOW_CC
#define ALGO_FIXED_WINDOW_CC

#include "algo_fixed_window.hh"

algo_fixed_window::algo_fixed_window() : preco_algo_base() {
  
  reset();

  _index_start = 0;

  _index_end = 0;

}

void algo_fixed_window::reset(){

  preco_algo_base::reset();

}

bool algo_fixed_window::reco(const std::vector<uint16_t> *wf) {

  this->reset();

  _pulse.t_start = (double)(_index_start);

  if(!_index_end)

    _pulse.t_end = (double)(wf->size() - 1);

  else

    _pulse.t_end = (double)_index_end;

  _pulse.t_max = preco_algo_base::max(wf, _pulse.peak, _index_start, _index_end);

  preco_algo_base::integral(wf, _pulse.area, _index_start, _index_end);

  _pulse.area = _pulse.area - ( _pulse.t_end - _pulse.t_start + 1) * _ped_mean;

  return true;

}

#endif
