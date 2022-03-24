#include<stdio.h>
#include<sys/stat.h>
#include <errno.h>

int main(int argc, char** argv){
  mode_t mode;
  int rez=mkdir("./os2",mode);
  if(rez==0){
    printf("Napravio folder os2 \n");
  }else{
    printf("Greska pri pravljenju foldera (error:%d)\n",errno);
  }
  return 0;
}