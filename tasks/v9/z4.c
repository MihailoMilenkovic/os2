#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>


int radi=0;
void handleSigUsr1(int signum){
  radi=1;
}
void handleSigUsr2(int signum){
  radi=0;
}

void handleSigChild(int signum){
}
int main(){
  pid_t pid=fork();
  if(pid==0){
    struct sigaction sa1;
    sa1.sa_handler=handleSigUsr1;
    sigaction(SIGUSR1,&sa1,NULL);
    struct sigaction sa2;
    sa2.sa_handler=handleSigUsr2;
    sigaction(SIGUSR2,&sa2,NULL);
    while(1){
      if(radi){
        printf("radi\n");
      }else{
        printf("spava\n");
      }
      sleep(1);
    }
    return 0;
  }else{
    struct sigaction sac;
    sac.sa_handler=handleSigChild;
    sigaction(SIGUSR2,&sac,NULL);
    sleep(1);
    printf("sending start signal\n");
    kill(pid,SIGUSR1);
    sleep(3);
    printf("sending end signal\n");
    kill(pid,SIGUSR2);
    sleep(2);
    printf("sending kill signal\n");
    kill(pid,SIGINT);
    printf("kraj\n");
  }
}
