
#include "Example.hh"
#include <TSystem.h>
int main(){
  gSystem->Load("libThreadBase");
  Example k;
  Example j;
  //usleep(3000000);
  k.Start();
  //usleep(2000000);
  k.Start();
  //j.Start();

  k.waitForExit();
  j.waitForExit();

  return 1;
}
