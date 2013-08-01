#ifndef EXAMPLE_HH
#define EXAMPLE_HH

#include "ThreadBase.hh"
#include "ThreadBase_utils.hh"
#include "ThreadBase_sem_tools.hh"

class Example : public ThreadBase {

public:

  Example();

protected:

  virtual bool Execute();

  virtual void Finalize() {std::cout<<"Ending "<<_threadptr<<std::endl;};

  void Sleep();

  void InitD();

  inline void Dream(pthread_t ptr){
    for(int i=0; i<10; i++){
      std::cout<<"sleeping: "<<i<<" ... "<<ptr<<std::endl;
      usleep(1000000);
    } 
  };

  int sem_id;

};
#endif
