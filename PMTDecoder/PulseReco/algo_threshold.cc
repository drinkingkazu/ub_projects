#ifndef ALGO_THRESHOLD_CC
#define ALGO_THRESHOLD_CC

#include "algo_threshold.hh"

void algo_threshold::reset(){

  preco_algo_base::reset();

}

bool algo_threshold::reco(const std::vector<uint16_t> *wf) {

  for(auto value : *wf){
    
    std::cout<<value<<std::endl;

  }

  return true;

}

#endif
