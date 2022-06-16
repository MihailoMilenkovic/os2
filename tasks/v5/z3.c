#include<limits.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>
#include <dirent.h>

int main(int argc,char** argv){
  char* startdir; char *enddir;
  int sizeKB=100;
  if(argc<3){
    printf("usage: [startpath] [endpath] [...files]\n");
    return -1;
  }
  startdir=argv[1]; 
  enddir=argv[2]; 
  DIR* d=opendir(enddir);
  mode_t dirmode=S_IRUSR|S_IXUSR|S_IWUSR|S_IWGRP;
  if(d==NULL){
    //we need to create the new directory
    int res=mkdir(enddir,dirmode); 
    if(res==-1){
      printf("error while making directory (errno:%d)\n",errno)
    }
  }
  mode_t filemode=S_IRUSR|S_IWGRP;
  struct dirent* ent;
  struct stat sb;
  char filepath[300];
  char newfilepath[300];
  for(int i=3;i<argc;i++){
    char* currItem=argv[i];
    strcpy(filepath,startdir);
    strcat(filepath,"/");
    strcat(filepath,currItem);
    strcpy(newfilepath,enddir);
    strcat(newfilepath,"/");
    strcat(newfilepath,currItem);
    printf("moving %s to %s\n",filepath,newfilepath);
    int res=rename(filepath,newfilepath);
    printf("res:%d\n",res);
    printf("errno:%d\n",errno);
    chmod(newfilepath,filemode);
  }

  return 0;
}
