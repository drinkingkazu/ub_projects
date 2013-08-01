#ifndef EXAMPLE_CC
#define EXAMPLE_CC

#include "Example.hh"

Example::Example(){
  
  InitD();

}

void Example::InitD(){

  pid_t pid = getpid();

  sem_id = create_semaphore('A' + pid);

  init_semaphore(sem_id,1);

}

void Example::Sleep(){

  decrease_semaphore(sem_id);  

  Dream(_threadptr);
  
  increase_semaphore(sem_id);
  
}

bool Example::Execute(){

  Sleep();

  return false;

};

#endif
