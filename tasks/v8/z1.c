#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int pid=0;
int fib(int n){
  //we create a new child only if this process is the child
  //we call this function recursively in the child
  int result=0;
  int pidLeft=0;
  int pidRight=0;
  if(n<=1){
    return 1;
  }
  if(n>1&&pid==0){
    int fdLeft[2];
    if(pipe(fdLeft)==-1){
      perror("pipe");
      exit(EXIT_FAILURE);
    }
    int fdRight[2];
    if(pipe(fdRight)==-1){
      perror("pipe");
      exit(EXIT_FAILURE);
    }
    pid=fork();
    if(pid!=0){
      pidLeft=pid;
    }
    if(pid==-1){
      perror("fork");
      exit(EXIT_FAILURE);
    }
    if(pid==0){
      //we are the first child
      close(fdRight[0]);
      close(fdRight[1]);
      //we read the value of n passed by the parent
      int passed_n;
      read(fdLeft[0],&passed_n,sizeof(passed_n));
      close(fdLeft[0]);
      //we call this function recursively for n-1 if n>1
      int recursive_res=1;
      if(n>1){
        recursive_res=fib(n-1);
      } 
      result=recursive_res;
      //we pipe the result back to the parent
      write(fdLeft[1],&result,sizeof(result));
      close(fdLeft[1]);
      exit(0);
    }
    pid=fork();
    if(pid!=0){
      pidRight=pid;
    }
    if(pid==-1){
      perror("fork");
      exit(EXIT_FAILURE);
    }
    if(pid==0){
      //we are the second child
      close(fdLeft[0]);
      close(fdLeft[1]);
      //we read the value of n passed by the parent
      int passed_n;
      read(fdRight[0],&passed_n,sizeof(passed_n));
      close(fdRight[0]);
      //we call this function recursively for n-1 if n>1
      int recursive_res=1;
      if(n>2){
        recursive_res=fib(n-2);
      } 
      result=recursive_res;
      //we pipe the result back to the parent
      write(fdRight[1],&result,sizeof(result));
      close(fdRight[1]);
      exit(0);
    }
    if(pid!=0){
      //we are the parent
      //we pass the current value to the left child in the pipe
      write(fdLeft[1],&n,sizeof(n));
      close(fdLeft[1]);
      
      int return_status;
      waitpid(pidLeft,&return_status,0);
      int passed_value;
      //we read its result
      read(fdLeft[0],&passed_value,sizeof(passed_value));
      close(fdLeft[0]);
      //printf("passed:%d\n",passed_value);
      //the final result is the child result multiplied by the current value
      result=passed_value;
    
      //we pass the current value to the right child in the pipe
      write(fdRight[1],&n,sizeof(n));
      close(fdRight[1]);
      
      waitpid(pidRight,&return_status,0);
      //we read its result
      read(fdRight[0],&passed_value,sizeof(passed_value));
      close(fdRight[0]);
      
      result=result+passed_value;

      return result;
    }
  }
  exit(0);
}
int main(int argc, char *argv[]){
  int rez=fib(10);
  printf("res:%d\n",rez);
  return 0;
}
