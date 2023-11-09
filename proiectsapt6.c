#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char **argv)
{
  struct stat st;
  int f=0;
  int output=0;
  if(argc!=2  || strstr(argv[1],".bmp")==NULL)
    {
      printf("Usage %s %s\n",argv[0],argv[1]);
      exit(EXIT_FAILURE);
    }
  else printf("ok\n");
  //printf("argc= %d",argc);
  
  f=open(argv[1],O_RDONLY);
  stat(argv[1],&st);
  printf("numele fisierului: %s\n",argv[1]);
  printf("dimensiunea fisierului: %ld\n",st.st_size);
  printf("identificator: %d\n",st.st_uid);
  printf("timpul ultimei modificari: %ld\n",st.st_size);
  
  output=open("statistica.txt",O_WRONLY);
  
  return 0;
}
