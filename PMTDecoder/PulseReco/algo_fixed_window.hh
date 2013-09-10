#ifndef ALGO_FIXED_WINDOW_HH
#define ALGO_FIXED_WINDOW_HH

#include "preco_algo_base.hh"

class algo_fixed_window : public preco_algo_base {

public:

  algo_fixed_window();

  virtual ~algo_fixed_window(){};

  virtual bool reco(const std::vector<uint16_t> *wf);
  
  virtual void reset();

  void set_start_index(size_t index) {_index_start = index;};

  void set_end_index  (size_t index) {_index_end   = index;};
  
protected:

  size_t _index_start, _index_end;

};

#endif
