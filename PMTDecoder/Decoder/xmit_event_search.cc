#ifndef XMIT_EVENT_SEARCH_CC
#define XMIT_EVENT_SEARCH_CC

#include "xmit_event_search.hh"

xmit_event_search::xmit_event_search() : 
  decoder_base() 
{
  _continue_mode=false;
  _target_id=PMT::INVALID_WORD;
  _fin.set_format(FORMAT::BINARY);
  _fin.set_mode(bin_io_handler::READ);

}

void xmit_event_search::run() {

  if(_target_id==PMT::INVALID_WORD){
    Message::send(MSG::ERROR,__FUNCTION__,"Event ID not specified!");
    exit(1);
  }
  
  _target_id = (0xf0000000 + ((_target_id & 0xfff) << 16) + 0x0000f000 + ((_target_id >> 12) & 0xfff));
  
  const PMT::word_t key_header=0xffffffff;

  std::vector<PMT::word_t> word_array(4,0);
  PMT::word_t word=0x0;
  bool fire=false;
  _fin.open();
  
  if(!_fin.is_open()) exit(1);
  
  while(1){

    word = _fin.read_word();

    if(_fin.eof()) break;
    if(fire && word==key_header) {

      word_array.push_back(word);

      if(!_continue_mode) break;

      print_word(&word_array);

      fire=false;
      word_array.clear();
      word_array.reserve(4);
      word_array.push_back(0);
      word_array.push_back(0);
      word_array.push_back(0);
      word_array.push_back(0);
    }

    if(!fire){
      word_array[0]=word_array[1];
      word_array[1]=word_array[2];
      word_array[2]=word_array[3];
      word_array[3]=word;
      
      if(word_array[0]==key_header && word_array[3]==_target_id){
	fire=true;
      }
    }else
      word_array.push_back(word);
  }

  if(!_continue_mode){
    print_word(&word_array);
  }
  
  _fin.close();
}

void xmit_event_search::print_word(std::vector<PMT::word_t> *in_array){


  algo_xmit_decoder algo;  
  size_t ctr=0;
  std::string msg("");

  std::vector<PMT::word_t> word_array;
  word_array.reserve(in_array->size() * 2);
  for(std::vector<PMT::word_t>::const_iterator iter(in_array->begin());
      iter!=in_array->end();
      ++iter){
    word_array.push_back( (*iter) & 0xffff);
    word_array.push_back( (*iter) >> 16 );
  }

  for(std::vector<PMT::word_t>::const_iterator iter(word_array.begin());
      iter!=word_array.end();
      ++iter){

    PMT::PMT_WORD word_type=algo.get_word_class((*iter));
    
    switch(word_type){
    case PMT::UNDEFINED_WORD:
    case PMT::CHANNEL_WORD:
      msg+=Form(" %04x ",(*iter));
      break;
    case PMT::EVENT_FIRST_HEADER:
    case PMT::EVENT_HEADER:
    case PMT::EVENT_LAST_WORD:
      msg+=" \033[93m";
      msg+=Form("%04x ",(*iter));
      msg+="\033[0m";
	break;
    case PMT::FIRST_WORD:
    case PMT::FEM_LAST_WORD:
      msg+=" \033[91m";
      msg+=Form("%04x ",(*iter));
      msg+="\033[0m";
      break;
    case PMT::CHANNEL_HEADER:
    case PMT::CHANNEL_LAST_WORD:
      msg+=" \033[95m";
      msg+=Form("%04x ",(*iter));
      msg+="\033[0m";
      break;
    }
    ctr+=1;
    if(ctr && ctr%8==0){
      std::cout<<msg.c_str()<<std::endl;
      msg="";
    }
  }
  if(msg.size()>0) std::cout<<msg.c_str()<<std::endl;
  std::cout<<" ... continue to next event ... " <<std::endl;
}




#endif
