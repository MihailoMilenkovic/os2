#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void handleInterrupt(int signum){
  //idk?
}

int main(){
  struct sigaction sa;
  sa.sa_handler=handleInterrupt;
  sigaction(SIGINT,&sa,NULL);
  while(1){
    printf("Vrtimo petlju\n");
    sleep(2);
  }
  return 0;
}
