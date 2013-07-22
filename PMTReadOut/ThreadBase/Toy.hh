#ifndef TOY_HH
#define TOY_HH

#include "ThreadBase.hh"
#include "myutils.h"
#include "ThreadBase_utils.hh"
#include "ThreadBase_sem_tools.hh"

class Toy : public ThreadBase {

public:

  Toy();

protected:

  virtual bool Execute();

  virtual void Finalize() {std::cout<<"Ending "<<_threadptr<<std::endl;};

  void Sleep();

  void InitD();

  int sem_id;

};
#endif
