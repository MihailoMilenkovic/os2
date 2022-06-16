#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

int main(){
  printf("sadrzaj trenutnog direktorijuma:\n");
  int ok=system("ls -la");
  if(ok!=-1){
    printf("kraj ispisa\n");
    return 0;
  }else{
    printf("greska (errno:%d)\n",errno);
  }
}
