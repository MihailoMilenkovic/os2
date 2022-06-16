#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
  pid_t pid=fork();
  if(pid==0){
    printf("OVO JE DETE\n");
    return 0;
  }else{
    int wstatus;
    waitpid(pid,&wstatus,0);
    if(wstatus==EXIT_SUCCESS){
      printf("OVO JE RODITELJ\n");
      return 0; 
    }else{
      printf("error in child process (exit code %d)",wstatus);
      return -1;
    }
  }
}
