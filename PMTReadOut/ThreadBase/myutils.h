#ifndef MYUTILS_H
#define MYUTILS_H

#include <iostream>

static void mydream(pthread_t ptr){

  for(int i=0; i<10; i++){
    std::cout<<"sleeping: "<<i<<" ... "<<ptr<<std::endl;
    usleep(1000000);
  }  
};

#endif
