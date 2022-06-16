#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdbool.h>

int fact(int n){
  if(n<=0){
    return 1;
  }
  int pipefd[2];
  int ok=pipe(pipefd);
  if(ok==-1){
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  pid_t pid=fork();
  if(pid==0){
    //child code here
    close(pipefd[0]);
    int res=fact(n-1);
    write(pipefd[1],&res,sizeof(res));
    close(pipefd[1]);
    _exit(EXIT_SUCCESS);
  }else{
    //parent code here
    close(pipefd[1]);
    int prev;
    int wstatus;
    pid_t w_pid=waitpid(pid,&wstatus,0);
    if(WIFEXITED(wstatus)){
      read(pipefd[0],&prev,sizeof(prev));
      close(pipefd[0]);
      int ans=n*prev;
      return ans;
    }else{
      printf("error in child\n");
      close(pipefd[0]);
      exit(0);
    }
  }

}

int main(int argc, char** argv){
  if(argc!=2){
    printf("Enter the number n to calculate the factorial of\n");
    return -1;
  }
  int n=atoi(argv[1]);
  int ans=fact(n);
  printf("%d\n",ans);
  return 0;
}
