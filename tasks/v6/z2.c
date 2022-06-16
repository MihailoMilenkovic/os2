#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
  printf("sadrzaj trenutnog direktorijuma:\n");
  pid_t pid=fork();
  if(pid==0){
    int ok=execlp("ls","ls","-la",NULL);
    if(ok!=-1){
      printf("greska pri ispisu\n");
      return -1;
    } 
    return 0;
  }else{
    int wstatus;
    waitpid(pid,&wstatus,0);
    //moguce da ovo nije korektno za status? nisam siguran
    if(wstatus==EXIT_SUCCESS){
      printf("kraj ispisa %d\n",wstatus);
      return 0; 
    }else{
      printf("error in child process (exit code %d)",wstatus);
      return -1;
    }
  }
}
