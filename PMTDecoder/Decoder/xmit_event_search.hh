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

  void run();

  void print_word(std::vector<PMT::word_t> *in_array);

private:
  
  bin_io_handler _fin;
  PMT::word_t _target_id;
  bool _continue_mode;


};
#endif

