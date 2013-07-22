#ifndef TOY_CC
#define TOY_CC

#include "Toy.hh"

Toy::Toy(){
  
  InitD();

}

void Toy::InitD(){

  pid_t pid = getpid();

  sem_id = create_semaphore('A' + pid);

  init_semaphore(sem_id,1);

}

void Toy::Sleep(){

  decrease_semaphore(sem_id);  

  mydream(_threadptr);
  
  increase_semaphore(sem_id);
  
}

bool Toy::Execute(){

  Sleep();

  return false;

};

#endif
