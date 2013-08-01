
#include<ThreadBase.hh>

void* thread_func(void* p){
  
  ThreadBase* t = reinterpret_cast<ThreadBase*>(p);
 
  if (t) t->Run();

  return 0;
}

//***************************************************************
void ThreadBase::Start() {
//***************************************************************
  
  _run=true;

  _executed=false;

  pthread_create(&_threadptr,NULL,thread_func,this); 
  
  sleep(0); }


//***************************************************************
void ThreadBase::Exit() {
//***************************************************************
  _run=false;
  pthread_exit(NULL);
  
}

//***************************************************************
void ThreadBase::Run(){ 
//***************************************************************
    _run=true;
    Initialize(); 
    while (_run && Execute()){}; 
    if(_run==false)
      std::cout<<"Hey process is already finished?!"<<std::endl;
    else
      std::cout<<"Hahaha I am finishing it."<<std::endl;

    _run = false;
    Finalize();
    _executed = true;
}
