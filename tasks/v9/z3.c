#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int kraj=0;
void handleInterrupt(int signum){
  printf("Uhvacen signal\n");
  kraj++;
}

int main(){
  struct sigaction sa;
  sa.sa_handler=handleInterrupt;
  sigaction(SIGINT,&sa,NULL);
  while(kraj<4){
    printf("Vrtimo petlju, zasad je dobijeno %d signala\n",kraj);
    sleep(2);
  }
  return 0;
}
