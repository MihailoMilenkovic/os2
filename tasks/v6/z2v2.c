#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
  pid_t pid=fork();
  if(pid<0){
    perror("fork");
    exit(EXIT_FAILURE);
  }else if(pid==0){
    int ok=execlp("ls","ls","-la",NULL);
    if(ok==-1){
      printf("Error in child process: %s\n",strerror(errno));
      _exit(EXIT_FAILURE);
    }
  }else{
    int ret_status;
    waitpid(pid,&ret_status,0);
    if(WIFEXITED(wstatus)){
      printf("Kraj ispisa\n");
    }else{
      printf("Error in child process\n");
    }
  }
  return 0;
}
