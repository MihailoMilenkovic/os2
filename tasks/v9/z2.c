#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int kraj=0;
void handleInterrupt(int signum){
  printf("Uhvacen signal, izlazimo\n");
  kraj=1;
}

int main(){
  struct sigaction sa;
  sa.sa_handler=handleInterrupt;
  sigaction(SIGINT,&sa,NULL);
  while(!kraj){
    printf("Vrtimo petlju\n");
    sleep(2);
  }
  return 0;
}
