#include<stdio.h>
#include<sys/stat.h>
#include <errno.h>
#include<unistd.h>
#include <string.h>

int main(int argc, char** argv){
  if(argc!=3){
    printf("Pogresan broj arg\n");
    return 0;
  }
  char* prvi=argv[1];
  char* drugi=argv[2];
  printf("%s\n",prvi);
  printf("%s\n",drugi);
  int rez=chdir(prvi);
  if(rez!=0){
    printf("Greska (error:%d)\n",errno);
    return 0;
  }
  mode_t mode;
  rez=mkdir(strcat("./",drugi),mode);
  if(rez!=0){
    printf("Greska pri pravljenju foldera (error:%d)\n",errno);
    return 0;
  }
  printf("Napravio novi folder\n");
}