#ifndef DECODER_BASE_CC
#define DECODER_BASE_CC

#include "decoder_base.hh"

decoder_base::decoder_base() : _name("decoder_base")
{
  set_verbosity(MSG::NORMAL);
}

void decoder_base::set_verbosity(MSG::Level level){

  _verbosity_level=level;

  for(size_t i=(size_t)(MSG::DEBUG); i<(size_t)(MSG::MSG_TYPE_MAX); ++i)
    _verbosity[i]=false;

  switch(level){
  case MSG::DEBUG:
    _verbosity[MSG::DEBUG]=true;
  case MSG::INFO:
    _verbosity[MSG::INFO]=true;
  case MSG::NORMAL:
    _verbosity[MSG::NORMAL]=true;
  case MSG::WARNING:
    _verbosity[MSG::WARNING]=true;
  case MSG::ERROR:
    _verbosity[MSG::ERROR]=true;
  case MSG::MSG_TYPE_MAX:
    break;
  }

}

#endif
