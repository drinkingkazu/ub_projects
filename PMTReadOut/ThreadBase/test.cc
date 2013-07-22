
#include "Toy.hh"

int main(){

  Toy k;
  //Toy j;
  usleep(3000000);
  k.Start();
  //usleep(2000000);
  k.Start();
  //j.Start();

  k.waitForExit();
  //j.waitForExit();

  return 1;
}
