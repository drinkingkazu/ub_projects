#ifndef XMIT_EVENT_SEARCH_HH
#define XMIT_EVENT_SEARCH_HH

#include <Base-TypeDef.hh>
#include <Algorithm-TypeDef.hh>
#include <FileIO-TypeDef.hh>

class xmit_event_search : public decoder_base {

public:
  xmit_event_search();
  ~xmit_event_search(){};

  void set_target_id(PMT::word_t id){_target_id=id;};

  void set_filename(std::string name){_fin.set_filename(name);};

  void set_continue_mode(bool mode){_continue_mode=mode;};

  void set_format(FORMAT::INPUT_FILE fmt){_fin.set_format(fmt);};
  
  void set_slow_readout(bool slow=true){_slow_readout=slow;};

  bool run();

  void print_word(std::vector<PMT::word_t> *in_array);

  inline bool new_event(PMT::word_t word) const{
    if(_slow_readout) return ( 0xffff == (word & 0xffff) );
    else return (0xffffffff == word); 
  };

  static const size_t XMIT_INDEX_EVENT_ID;
  static const size_t SLOW_INDEX_EVENT_ID;

private:
  
  bin_io_handler _fin;
  PMT::word_t _target_id;
  bool _continue_mode;
  bool _slow_readout;

  size_t _index_event_id;
  algo_base* _algo;

};
#endif

