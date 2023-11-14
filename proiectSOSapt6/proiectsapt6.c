#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#define st_atime st_atim.tv_sec      
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec
struct timespec st_mtim;

void drept_acces(struct stat st,char buffer[4096])
{
  if( st.st_mode & S_IRUSR )
    {
      sprintf(buffer+strlen(buffer),"R");
    }
  else {sprintf(buffer+strlen(buffer),"-");}
  
  if( st.st_mode & S_IWUSR )
    {
      sprintf(buffer+strlen(buffer),"W");
    }
  else {sprintf(buffer+strlen(buffer),"-");}
  
  if( st.st_mode & S_IXUSR )
    {
    sprintf(buffer+strlen(buffer),"X");
    }
  else {sprintf(buffer+strlen(buffer),"-");}
  
  sprintf(buffer+strlen(buffer),"\ndrepturi de acces grup: ");
   if( st.st_mode & S_IRGRP )
     sprintf(buffer+strlen(buffer),"R");
   else {sprintf(buffer+strlen(buffer),"-");}
   
    if( st.st_mode & S_IWGRP )
      sprintf(buffer+strlen(buffer),"W");
    else {sprintf(buffer+strlen(buffer),"-");}
    
    if( st.st_mode & S_IXGRP )
      sprintf(buffer+strlen(buffer),"X");
    else {sprintf(buffer+strlen(buffer),"-");}
    
    sprintf(buffer+strlen(buffer),"\ndrepturi de acces altii: ");
   if( st.st_mode & S_IROTH )
     sprintf(buffer+strlen(buffer),"R");
   else {sprintf(buffer+strlen(buffer),"-");}
   
    if( st.st_mode & S_IWOTH )
      sprintf(buffer+strlen(buffer),"W");
    else {sprintf(buffer+strlen(buffer),"-");}
    
    if( st.st_mode & S_IXOTH )
      sprintf(buffer+strlen(buffer),"X");
    else sprintf(buffer+strlen(buffer),"-");
}

int main(int argc,char **argv)
{
  struct stat st;
  int width,height;
  char buffer[4096];
  int f=0;
  int output=0;
  
  if(argc!=2  || strstr(argv[1],".bmp")==NULL)
    {
      printf("Usage %s %s\n",argv[0],argv[1]);
      exit(EXIT_FAILURE);
    }
  else printf("ok\n");
  
  f=open(argv[1],O_RDONLY);
  if(f==-1)
    {
      perror("There was an error opening the file \n");
      exit(-1);
    }

  if(fstat(f,&st)==-1)
    {
      perror("There was an error getting the file info\n");
      exit(-1);
    }
  
  output=open("statistica.txt",O_WRONLY);
  if(output==-1)
    {
      perror("There was an error opening the file \n");
      exit(-1);
    }
  
  
  sprintf(buffer,"numele fisierului: %s\n",argv[1]);
  lseek(f,18,SEEK_SET);
  read(f,&width,sizeof(int));
  sprintf(buffer+strlen(buffer),"inaltime: %d\n",width);
  read(f,&height,sizeof(int));
  sprintf(buffer+strlen(buffer),"lungime: %d\n",height);
  sprintf(buffer+strlen(buffer),"dimensiunea fisierului: %ld\n",st.st_size);
  sprintf(buffer+strlen(buffer),"identificator: %d\n",st.st_uid);
  sprintf(buffer+strlen(buffer),"timpul ultimei modificari: %s",ctime(&st.st_mtime));
  sprintf(buffer+strlen(buffer),"contorul de legaturi: %ld\n",st.st_nlink);
  sprintf(buffer+strlen(buffer),"drepturi de acces user: ");
 
  drept_acces(st,buffer);
  sprintf(buffer+strlen(buffer),"\n");
  
  printf("%s",buffer);
  write(output,buffer,strlen(buffer));

  if(close(f)==-1)
    {
      perror("The file couldn't close normally\n");
      exit(-1);
    }

  if(close(output)==-1)
    {
      perror("The file couldn't close normally\n");
      exit(-1);
    }
   
  return 0;
}
