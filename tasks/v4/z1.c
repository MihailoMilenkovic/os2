#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<errno.h>
#include<stdbool.h>
#include<string.h>

typedef struct options{
  char* allowedChars;
  bool sorted;
  bool hasStartChars;
  bool hasStartYear;
  int startYear;
  bool isMarch;
}opt;

struct options o;
int startPos=1;
char currDirname[300];
void parseArgs(int argc,char**argv){
  for(int i=1;i<argc;i++){
    char* currOption=argv[i];
    if(strlen(currOption)>2 && currOption[0]=='-' && currOption[1]=='-'){
      if(currOption[2]=='S'){
        char okChars[100];
        sprintf(okChars,"%s",currOption+3);
        o.allowedChars=okChars;
      }
      if(currOption[2]=='p'){
        char currYear[100];
        printf("okej\n");
        sprintf(currYear,"%s",currOption+3);
        printf("okej\n");
        o.hasStartYear=true;
        o.startYear=atoi(currYear)+1;
      }if(strcmp(currOption,"--mart")==0){
        o.isMarch=true;
      }if(strcmp(currOption,"--sort")==0){
        o.sorted=true;
      }
      startPos++;
    }
    else{
      break;
    }
  }
}
int startCharFilter(const struct dirent *dent){
  for(int i=0;i<strlen(o.allowedChars);i++){
    if(dent->d_name[0]==o.allowedChars[i]){
      return 1;
    }
  }
  return 0;
}
int filterByStartYear(const struct dirent *dent){
    char filename[300];
    struct stat sb;
    strcpy(filename,currDirname); 
    strcat(filename,"/");
    strcat(filename,dent->d_name); 
    int ok=stat(filename,&sb);
    if(ok==-1){
      return 0;
    }
    if(localtime(&sb.st_atime)->tm_year>=o.startYear){
      return 1;
    }
    return 0;
}
int marchFilter(const struct dirent *dent){
    char filename[300];
    struct stat sb;
    strcpy(filename,currDirname); 
    strcat(filename,"/");
    strcat(filename,dent->d_name); 
    int ok=stat(filename,&sb);
    if(ok==-1){
      return 0;
    }
    if(localtime(&sb.st_atime)->tm_mon==2){
      return 1;
    }
    return 0;
}

void printCharOrDash(bool ok,char ch){
  if(ok){
    printf("%c",ch);
  }else{
    printf("-");
  }
}

void list(char* dirname){
  printf("listing %s:\n",dirname);
  strcpy(currDirname,dirname);
  DIR* d=opendir(dirname);
  if(d==NULL){
    printf("error:%d",errno);
    return;
  }
  struct dirent* dent;
  struct stat sb;
  char filename[300];
  struct dirent** entList;
  int (*filter)(const struct dirent *) =NULL;
  int (*compar)(const struct dirent **, const struct dirent **) =NULL;
  if(o.sorted){
    compar=alphasort; 
  }
  if(o.hasStartYear){
    filter=filterByStartYear;
  }
  if(o.isMarch){
    filter=marchFilter;
  }
  if(o.hasStartChars){
    filter=startCharFilter;
  }
  int entries=scandir(dirname,&entList,filter,compar);
  if(entries==-1){
    printf("error scanning dir:%d",errno);
    return;
  }
  for(int i=0;i<entries;i++){
    dent=entList[i];
    if(strcmp(dent->d_name,".")==0||strcmp(dent->d_name,"..")==0){
      continue;
    }
    strcpy(filename,dirname); 
    strcat(filename,"/");
    strcat(filename,dent->d_name); 
    int ok=stat(filename,&sb);
    if(ok==-1){
      printf("error for current file,%d\n",errno);
      continue;
    }
    printCharOrDash(dent->d_type&DT_DIR,'d');
    printCharOrDash(sb.st_mode&S_IRUSR,'r');
    printCharOrDash(sb.st_mode&S_IWUSR,'w');
    printCharOrDash(sb.st_mode&S_IXUSR,'x');
    printCharOrDash(sb.st_mode&S_IRGRP,'r');
    printCharOrDash(sb.st_mode&S_IWGRP,'w');
    printCharOrDash(sb.st_mode&S_IXGRP,'x');
    printCharOrDash(sb.st_mode&S_IROTH,'r');
    printCharOrDash(sb.st_mode&S_IWOTH,'w');
    printCharOrDash(sb.st_mode&S_IXOTH,'x');
    printf(" ");
    printf("%ld ",sb.st_nlink);
    printf("%ld ",sb.st_blksize);
    struct tm* locTime=localtime(&sb.st_atime);
    printf("%04d-%02d-%02d ",locTime->tm_year+1900,locTime->tm_mon+1,locTime->tm_mday);
    printf("%02d:%02d ",locTime->tm_hour,locTime->tm_min);
    printf("%s\n",dent->d_name);
    
  }

}

int main(int argc,char** argv){
  
  parseArgs(argc,argv);
  printf("startYear:%d",o.startYear);
  printf("allowedChars:%s",o.allowedChars);
  for(int i=1;i<argc;i++){
    list(argv[i]);
  }
}
