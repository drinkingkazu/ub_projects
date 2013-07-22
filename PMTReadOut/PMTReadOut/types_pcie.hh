#ifndef TYPES_PCIE_HH
#define TYPES_PCIE_HH

//#include <stdlib>
#include <stdint.h>

namespace PCIE{


  typedef struct mod_address{

    uint32_t trigger_id;
    uint32_t fem_id;
    uint32_t shaper_id;
    uint32_t xmit_id;
    
    mod_address() :
      trigger_id(0xffffffff),
      fem_id(0xffffffff),
      shaper_id(0xffffffff),
      xmit_id(0xffffffff)
    {};
  } mod_address_t;


  static const uint32_t BUFF_SEND_LENGTH=40000;
  typedef uint32_t buff_send_t[BUFF_SEND_LENGTH];

};

#endif
