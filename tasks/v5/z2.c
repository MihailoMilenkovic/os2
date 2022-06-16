#include<limits.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>
#include <dirent.h>

int main(int argc,char** argv){
  char* path;
  int sizeKB=100;
  if(argc==2){
     path=argv[1];
  }else if (argc==3){
    path=argv[1];
    sizeKB=atoi(argv[2]);
  }else{
    printf("usage: [path] [size]?\n");
    return -1;
  }
  DIR* d=opendir(path);
  if(d==NULL){
    printf("error:%d",errno);
    return -1;
  }
  struct dirent* ent;
  struct stat sb;
  char filepath[300];
  while((ent=readdir(d))!=NULL){
    strcpy(filepath,path);
    strcat(filepath,"/");
    strcat(filepath,ent->d_name);
    printf("entry: %s\n",filepath); 
    int res=stat(filepath,&sb);
    printf("size:%ld\n",sb.st_size);
  }

  return 0;
}
