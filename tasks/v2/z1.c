#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
struct options{
  int recursive;
  int hidden;
};

int list(char* dirname,struct options o){
  printf("LISTAMO: %s\n",dirname);
  DIR* dir=opendir(dirname);
  if(dir==NULL){
    printf("Greska: %s\n",strerror(errno));
    return -1;
  }
  // printf("NIJE GRESKA\n");
  struct dirent* dirEntry;
  while(dirEntry=readdir(dir)){
    // printf("ok ovde\n");
    char* contentName=dirEntry->d_name;
    // printf("CITAMO %s\n",contentName);
    if(contentName[0]!='.'||(o.hidden==1&&contentName[0]=='.')){
        printf("%s\n",dirEntry->d_name);
        if(dirEntry->d_type==DT_REG){
          printf("Normalan fajl\n");
        }
        if(dirEntry->d_type==DT_DIR){
          printf("Direktorijum\n");
          //idemo rekurzivno ovde
          char* newDirName;
          sprintf(newDirName,"%s/%s",dirname,contentName);
          printf("OK\n");
          printf("Direktorijum\n");
          if(o.hidden){
            if(strcmp(newDirName,".")!=0&&strcmp(newDirName,"..")!=0){
              list(newDirName,o);
            }
          }
        } 
      }
    }
  printf("Zatvaramo\n");
  closedir(dir);
  return 0;
}
int main(int argc,char** argv){

  struct options o;
  for(int i=1;i<argc;i++){
    if(strcmp(argv[i],"-R")==0){
      o.recursive=1;
    }
    else if(strcmp(argv[i],"-A")==0){
      o.hidden=1;
    }
  }
  for(int i=1;i<argc;i++){
    if(strcmp(argv[i],"-R")!=0&&strcmp(argv[i],"-A")!=0){
      int res=list(argv[i],o);
      if(res==-1){
        return -1;
      }
    }
  }

  return 0;
}