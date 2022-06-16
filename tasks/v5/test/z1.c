#include<limits.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc,char** argv){
  char* path=".";
  if(argc==2){
     path=argv[1];
  }else if (argc>2){
    printf("Wrong number of arguments");
    return -1;
  }
  char* real=realpath(path,NULL);
  printf("real path:%s\n",real);
  int dep=-1;
  for(int idx=0;idx<strlen(real);idx++){
    if(real[idx]=='/'){
      dep++;
    }
  }
  printf("depth:%d\n",dep);
  return 0;
}
