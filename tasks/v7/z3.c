#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int pid=0;
bool original=true;
int fact(int n){
  //we create a new child only if this process is the child
  //we call this function recursively in the child
  int result=1;
  if(n>0&&pid==0){
    int fd[2];
    if(pipe(fd)==-1){
      perror("pipe");
      exit(EXIT_FAILURE);
    }
    pid=fork();
    if(pid==-1){
      perror("fork");
      exit(EXIT_FAILURE);
    }
    if(pid!=0){
      //we are the parent
      //we pass the current value to the child in the pipe
      write(fd[1],&n,sizeof(n));
      close(fd[1]);
      
      int return_status;
      waitpid(pid,&return_status,0);
      int passed_value;
      //we read its result
      read(fd[0],&passed_value,sizeof(passed_value));
      close(fd[0]);
      //printf("passed:%d\n",passed_value);
      //the final result is the child result multiplied by the current value
      result=n*passed_value;
      return result;
    }else{
      //we are the child
      original=false;
      //we read the valu of n passed by the parent
      int passed_n;
      read(fd[0],&passed_n,sizeof(passed_n));
      close(fd[0]);
      //we call this function recursively for n-1 if n>1
      int recursive_res=1;
      if(n>1){
        recursive_res=fact(n-1);
      } 
      result=recursive_res;
      //we pipe the result back to the parent
      write(fd[1],&result,sizeof(result));
      close(fd[1]);
      exit(0);
     // return result;
    }
  }
}
int main(int argc, char *argv[]){
  int rez=fact(4);
  printf("res:%d\n",rez);
  return 0;
}
