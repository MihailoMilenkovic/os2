# include <stdio.h>
# include <sys/types.h>
# include <dirent.h>
# include <errno.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <time.h>

// ispis sadrzaja datih direktorijuma; ispisuje se nekoliko podatak vezano za njih

void ispisiAkoVazi(int uslov,char * tekst){
  if(uslov!=0){
    printf("%s",tekst);
  }else{
    printf("-");
  }
}
char* padded(int num){
  char* rez= malloc (sizeof (char) * 2);;
  if(num<10){
    sprintf(rez,"0%d",num); 
  }else{
    sprintf(rez,"%d",num);
  }
  return rez;
}

void list(char *name) {
    DIR *d = opendir(name);
    if (d == NULL) {
        int err = errno;
        printf("error while reading %s: %d %s\n", name,err,strerror(err));
        return;
    }
    struct dirent *ent;
    // set errno for later checks
    errno = 0;
    while ((ent = readdir(d)) != NULL) {
        if(ent->d_name[0]=='.')
          continue; 
        if (ent->d_type == DT_DIR)
            printf("d");
        else
            printf("-");
        //printf("%d \t %s", ent->d_type, ent->d_name);
        //printf("\n");
        //printf("OKEJ %s %s\n",name,ent->d_name);
        char path[80];
        sprintf(path,"%s%s",name,ent->d_name);
        //printf("OKEJ222 %s %s\n",name,ent->d_name);
        struct stat stats;
        int res=stat(path,&stats);
        ispisiAkoVazi(stats.st_mode & S_IRUSR,"r");
        ispisiAkoVazi(stats.st_mode & S_IWUSR,"w");
        ispisiAkoVazi(stats.st_mode & S_IXUSR,"x");
        ispisiAkoVazi(stats.st_mode & S_IRGRP,"r");
        ispisiAkoVazi(stats.st_mode & S_IWGRP,"w");
        ispisiAkoVazi(stats.st_mode & S_IXGRP,"x");
        ispisiAkoVazi(stats.st_mode & S_IROTH,"r");
        ispisiAkoVazi(stats.st_mode & S_IWOTH,"w");
        ispisiAkoVazi(stats.st_mode & S_IXOTH,"x");

        printf("\t");
        printf("%d\t",stats.st_nlink);
        struct tm* currtime=localtime(&stats.st_ctime);
        char* mon=padded(currtime->tm_mon);
        char* day=padded(currtime->tm_mday);
        printf("%d-%s-%s %d:%d\t",currtime->tm_year+1900,mon,day,currtime->tm_hour,currtime->tm_min);

        printf("%s\n",ent->d_name);
    }
    if (errno != 0) {
        printf("error while reading dir: %d %s\n", errno,strerror(errno));       
    }
    int ret = closedir(d);
    if (ret == -1) {
        printf("error closing dir stream: %d %s\n", errno,strerror(errno));           
    }

}

int main(int argn, char **args) {
    if (argn == 1) {
        printf("No arguments given; exiting;\n");
        exit(0);
    }
    for (int i=1; i<argn; i++){
        printf(args[i]);
        printf("\n");
        list(args[i]);
    }
}