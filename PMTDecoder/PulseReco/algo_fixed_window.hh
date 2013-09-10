#ifndef ALGO_FIXED_WINDOW_HH
#define ALGO_FIXED_WINDOW_HH

#include "preco_algo_base.hh"

class algo_fixed_window : public preco_algo_base {

public:

  algo_fixed_window();

  virtual ~algo_fixed_window(){};

  virtual bool reco(const std::vector<uint16_t> *wf);
  
  virtual void reset();

protected:

  size_t _index_start, _index_end;

};

#endif
