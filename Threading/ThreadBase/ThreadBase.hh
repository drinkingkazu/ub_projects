#ifndef THREADBASE_HH
#define THREADBASE_HH

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>

class ThreadBase {

protected:

  //! initialize: set parameters
  virtual void Initialize(){}
  
  //! execute
  //virtual bool Execute()=0;
  //virtual bool Execute(){return true;};
  virtual bool Execute(){ return false;};
  
  //! finalize
  virtual void Finalize(){};

  //! run thread functions
  virtual void Run();

 
public:

  //! destructor
  virtual ~ThreadBase(){waitForExit();}

  //! create thread
  virtual void Start();
  
  //! stop thread: execute finalize function
  virtual void Stop(){_run=false;}

  //! wait for exit
  void waitForExit(){pthread_join(_threadptr,NULL);};
  
  //! kill thread
  virtual void Exit();
  
  //! true if running
  bool isRunning()const {return _run;}

  //! true if already executed
  bool executed() const {return _executed;}

  //! thread function: can't be method of the class
  friend void* thread_func(void*);
  
protected:
  
  //pthread_t  _threadptr;
  //_opaque_pthread_t _threadptr;
  
  pthread_t _threadptr;
  
  bool _run;

  bool _executed;

};



#endif



