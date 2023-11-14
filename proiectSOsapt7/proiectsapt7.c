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
#include <dirent.h>
#include <pwd.h>
#define st_atime st_atim.tv_sec      
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec
struct timespec st_mtim;
struct dirent *dt;
struct passwd *userInfo;

void drept_acces(mode_t mode,char buffer[4096],const char optional[100])
{
  sprintf(buffer+strlen(buffer),"drepturi de acces user %s: ",optional);
  if(mode & S_IRUSR )
    {
      sprintf(buffer+strlen(buffer),"R");
    }
  else {sprintf(buffer+strlen(buffer),"-");}
  
  if(mode & S_IWUSR )
    {
      sprintf(buffer+strlen(buffer),"W");
    }
  else {sprintf(buffer+strlen(buffer),"-");}
  
  if(mode & S_IXUSR )
    {
    sprintf(buffer+strlen(buffer),"X");
    }
  else {sprintf(buffer+strlen(buffer),"-");}
  
  sprintf(buffer+strlen(buffer),"\ndrepturi de acces grup %s: ",optional);
   if(mode & S_IRGRP )
     sprintf(buffer+strlen(buffer),"R");
   else {sprintf(buffer+strlen(buffer),"-");}
   
    if(mode & S_IWGRP )
      sprintf(buffer+strlen(buffer),"W");
    else {sprintf(buffer+strlen(buffer),"-");}
    
    if(mode & S_IXGRP )
      sprintf(buffer+strlen(buffer),"X");
    else {sprintf(buffer+strlen(buffer),"-");}
    
    sprintf(buffer+strlen(buffer),"\ndrepturi de acces altii %s: ",optional);
   if(mode & S_IROTH )
     sprintf(buffer+strlen(buffer),"R");
   else {sprintf(buffer+strlen(buffer),"-");}
   
    if(mode & S_IWOTH )
      sprintf(buffer+strlen(buffer),"W");
    else {sprintf(buffer+strlen(buffer),"-");}
    
    if(mode & S_IXOTH )
      sprintf(buffer+strlen(buffer),"X");
    else sprintf(buffer+strlen(buffer),"-");
}

void writeForRegularBMPFile(char *dir,char *fis,char buffer[4096],int fisier_iesire,struct stat st)
{
  int width,height;
  int f;
  char fisier[1000];
  sprintf(fisier,"%s/%s",dir,fis);
  f=open(fisier,O_RDONLY);
  fstat(f,&st);
  sprintf(buffer,"numele fisierului: %s\n",fis);
  lseek(f,18,SEEK_SET);
  read(f,&width,sizeof(int));
  sprintf(buffer+strlen(buffer),"inaltime: %d\n",width);
  read(f,&height,sizeof(int));
  sprintf(buffer+strlen(buffer),"lungime: %d\n",height);
  sprintf(buffer+strlen(buffer),"dimensiunea fisierului: %ld\n",st.st_size);
  sprintf(buffer+strlen(buffer),"identificator: %d\n",st.st_uid);
  sprintf(buffer+strlen(buffer),"timpul ultimei modificari: %s",ctime(&st.st_mtime));
  sprintf(buffer+strlen(buffer),"contorul de legaturi: %ld\n",st.st_nlink);
 
  drept_acces(st.st_mode,buffer,"");
  sprintf(buffer+strlen(buffer),"\n");
  printf("%s",buffer);
  write(fisier_iesire,buffer,strlen(buffer));
}

void writeForRegularFile(char *dir,char *fis,char buffer[4096],int fisier_iesire,struct stat st)
{
  int f;
  char fisier[1000];
  sprintf(fisier,"%s/%s",dir,fis);
  f=open(fisier,O_RDONLY);
  fstat(f,&st);
  sprintf(buffer,"numele fisierului: %s\n",fis);
  sprintf(buffer+strlen(buffer),"dimensiunea fisierului: %ld\n",st.st_size);
  sprintf(buffer+strlen(buffer),"identificator: %d\n",st.st_uid);
  sprintf(buffer+strlen(buffer),"timpul ultimei modificari: %s",ctime(&st.st_mtime));
  sprintf(buffer+strlen(buffer),"contorul de legaturi: %ld\n",st.st_nlink);
 
  drept_acces(st.st_mode,buffer,"");
  sprintf(buffer+strlen(buffer),"\n");
  printf("%s",buffer);
  write(fisier_iesire,buffer,strlen(buffer));
}

void writeForSymbolicLink(char *dir,char *fis,char buffer[4096],int fisier_iesire,struct stat st)
{
  int f;
  char fisier[1000];
  sprintf(fisier,"%s/%s",dir,fis);
  f=open(fisier,O_RDONLY);
  fstat(f,&st);
  sprintf(buffer,"nume legatura: %s\n",dt->d_name);
  sprintf(buffer+strlen(buffer),"dimensiune legatura: %ld\n",st.st_size);
  drept_acces(st.st_mode,buffer,"legatura");
  sprintf(buffer+strlen(buffer),"\n");
  printf("%s",buffer);
  write(fisier_iesire,buffer,strlen(buffer));
}

void writeForDirectory(char*dir,char *fis,char buffer[4096],int fisier_iesire,struct stat st)
{
  int f;
  userInfo = getpwuid(st.st_uid);
  char fisier[1000];
  sprintf(fisier,"%s/%s",dir,fis);
  f=open(fisier,O_RDONLY);
  fstat(f,&st);
  sprintf(buffer,"nume director: %s\n",dt->d_name);
  sprintf(buffer+strlen(buffer),"identificator utilizator: %s\n",userInfo->pw_name);
  drept_acces(st.st_mode,buffer,"");
   sprintf(buffer+strlen(buffer),"\n");
  printf("%s",buffer);
  write(fisier_iesire,buffer,strlen(buffer));
}

void verifyTypeOfFile(char *file)
{
  char buffer[4096];
  char filePath[4096];
  char linkbuff[256];
  int output=0;

  struct stat st;
  output=open("statistica.txt",O_WRONLY|O_APPEND);
  if(output==-1)
    {
      perror("There was an error opening the file \n");
      exit(-1);
    }
  sprintf(filePath,"%s/%s",file,dt->d_name);
  if(lstat(filePath,&st)==-1)
    {
      perror("Error reading file components\n");
      exit(-1);
    }
  if(S_ISREG(st.st_mode))
    {
      if(strstr(dt->d_name,".bmp"))
	{
          writeForRegularBMPFile(file,dt->d_name,buffer,output,st);
	}
      else
	{
           writeForRegularFile(file,dt->d_name,buffer,output,st);
	}
    }
  else if(S_ISLNK(st.st_mode))
    {
      if(readlink(filePath,linkbuff,sizeof(linkbuff)))
	{
	  writeForSymbolicLink(file,dt->d_name,buffer,output,st);
	}
    }
  else if(S_ISDIR(st.st_mode))
    {
      
      writeForDirectory(file,dt->d_name,buffer,output,st);
    }
  else
    {
      sprintf(buffer," ");
      write(output,buffer,strlen(buffer));
    }
  if(close(output)==-1)
    {
      perror("The file couldn't close normally\n");
      exit(-1);
    }
}


int main(int argc,char **argv)
{
  DIR *dire;
  if(argc!=2)
    {
      perror("Not enough arguments! We will not write anything to the file");
      exit(EXIT_FAILURE);
    }
  else printf("ok\n");

  dire=opendir(argv[1]);
  if(dire==NULL)
    {
      perror("Error opening directory\n");
      exit(-1);
    }
  dt=readdir(dire);
  
  while(dt!=NULL)
    {
      if(strcmp(dt->d_name,".")!=0 || strcmp(dt->d_name,"..")!=0)
	{
          verifyTypeOfFile(argv[1]);
	}
      dt = readdir(dire);
    }
  //printf("\n");
  closedir(dire);
  return 0;
}
