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
#include <sys/types.h>
#include <sys/wait.h>

#define st_atime st_atim.tv_sec      
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec
struct timespec st_mtim;
struct dirent *dt,*dt2;
struct passwd *userInfo;
int status,cnt=0;

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

void writeForRegularBMPFile(char *dir,char *fis,char buffer[4096],struct stat st)
{
  int width,height;
  int f;
  char fisier[1000];
  sprintf(fisier,"%s/%s",dir,fis);
  if((f=open(fisier,O_RDONLY))==-1)
    {
      perror("Error reading BMP file components\n");
      exit(-1);
    }
  fstat(f,&st);
  sprintf(buffer,"numele fisierului: %s\n",fis);
  lseek(f,18,SEEK_SET);
  read(f,&width,sizeof(int));
  sprintf(buffer+strlen(buffer),"inaltime: %d\n",width);
  read(f,&height,sizeof(int));
  sprintf(buffer+strlen(buffer),"lungime: %d\n",height);
  sprintf(buffer+strlen(buffer),"dimensiunea fisierului: %ld\n",st.st_size);
  sprintf(buffer+strlen(buffer),"identificator: %d\n",st.st_uid);
  sprintf(buffer+strlen(buffer),"timpul ultimei modificari: %s\n",ctime(&st.st_mtime));
  sprintf(buffer+strlen(buffer),"contorul de legaturi: %ld\n",st.st_nlink);
 
  drept_acces(st.st_mode,buffer,"");
  sprintf(buffer+strlen(buffer),"\n");
}

void writeForRegularFile(char *dir,char *fis,char buffer[4096],struct stat st)
{
  int f;
  char fisier[1000];
  sprintf(fisier,"%s/%s",dir,fis);
  if((f=open(fisier,O_RDONLY))==-1)
    {
      perror("Error reading normal file components\n");
      exit(-1);
    }
  fstat(f,&st);
  sprintf(buffer,"numele fisierului: %s\n",fis);
  sprintf(buffer+strlen(buffer),"dimensiunea fisierului: %ld\n",st.st_size);
  sprintf(buffer+strlen(buffer),"identificator: %d\n",st.st_uid);
  sprintf(buffer+strlen(buffer),"timpul ultimei modificari: %s\n",ctime(&st.st_mtime));
  sprintf(buffer+strlen(buffer),"contorul de legaturi: %ld\n",st.st_nlink);
 
  drept_acces(st.st_mode,buffer,"");
  sprintf(buffer+strlen(buffer),"\n");
}

void writeForSymbolicLink(char *dir,char *fis,char buffer[4096],struct stat st)
{
  int f;
  char fisier[1000];
  sprintf(fisier,"%s/%s",dir,fis);
  if((f=open(fisier,O_RDONLY))==-1)
    {
      perror("Error reading simbolik link components\n");
      exit(-1);
    }
  fstat(f,&st);
  sprintf(buffer,"nume legatura: %s\n",dt->d_name);
  sprintf(buffer+strlen(buffer),"dimensiune legatura: %ld\n",st.st_size);
  drept_acces(st.st_mode,buffer,"legatura");
  sprintf(buffer+strlen(buffer),"\n");
}

void writeForDirectory(char*dir,char *fis,char buffer[4096],struct stat st)
{
  int f;
  userInfo = getpwuid(st.st_uid);
  char fisier[1000];
  sprintf(fisier,"%s/%s",dir,fis);
  if((f=open(fisier,O_RDONLY))==-1)
    {
      perror("Error reading directory components\n");
      exit(-1);
    }
  fstat(f,&st);
  sprintf(buffer,"nume director: %s\n",dt->d_name);
  sprintf(buffer+strlen(buffer),"identificator utilizator: %s\n",userInfo->pw_name);
  drept_acces(st.st_mode,buffer,"");
   sprintf(buffer+strlen(buffer),"\n");

}

void convertImageInGreyScale(char *dir,char *fis)
{
  int width,height,bit;
  unsigned char data[3];
  double grey;
  int f;
  char fisier[1000];
  sprintf(fisier,"%s/%s",dir,fis);
  if((f=open(fisier,O_RDWR))==-1)
    {
      perror("Error reading BMP file components\n");
      exit(-1);
    }
  lseek(f,18,SEEK_SET);//mutam cursorul pana ajungem la width
  read(f,&width,sizeof(int));
  lseek(f,22,SEEK_SET);//mutam cursorul pana ajungem la height
  read(f,&height,sizeof(int));
  lseek(f,28, SEEK_SET);//mutam cursorul pana la nr de biti
  read(f,&bit,sizeof(bit));//nr de biti per pixel
  lseek(f, 54, SEEK_SET);
  if(bit>8)//daca e o imagine colorata
    {
      for(int i=0;i<height;i++)
	{
	  for(int i=0;i<width;i++)
	    {	      
	      read(f,data, sizeof(data));
	      lseek(f, -3, SEEK_CUR);//fara padding
	      //P_gri = 0.299 * P_rosu + 0.587 * P_verde + 0.114 * P_albastru
	      grey=0.299*(double)data[0]+0.587*(double)data[1]+0.114*(double)data[2];
	      unsigned char grayPixel[3]={(unsigned char)grey,(unsigned char)grey,(unsigned char)grey};
	      write(f,grayPixel,sizeof(grayPixel));
	    }
	}
    }
}

void verifyTypeOfFile(char *file,char *file2)
{
  char linkbuff[256];
  char filePath[1024];
  int output1,output2,output3,output4;
  int pid0,pid1,pid2,pid3,pid4;
  char file_name1[256],buffer1[4096],path1[1024];
  char file_name2[256],buffer2[4096],path2[1024];
  char file_name3[256],buffer3[4096],path3[1024];
  char file_name4[256],buffer4[4096],path4[1024];
  struct stat st;
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
	  cnt++;
	  pid0=fork();
	  if(pid0==0)
	  {
	     
	      strcpy(file_name1,dt->d_name);
	      strcat(file_name1,"_statistica.txt");
	      writeForRegularBMPFile(file,dt->d_name,buffer1,st);
	      sprintf(path1,"%s/%s",file2,file_name1);
	      output1=open(path1,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP);
	      write(output1,buffer1,strlen(buffer1));
	      if( close(output1)==-1)
		{
		  perror("The BMP file couldn't close normally\n");
		  exit(-1);
		}
	      exit(10);
	  }
	 cnt++;
	 pid1=fork();
          if(pid1==0)
	    {
	    
	      convertImageInGreyScale(file,dt->d_name);
	      exit(34);
	    }
	}
      else
	{
	  cnt++;
	  pid2=fork();
	  if(pid2==0)
	    {
	      
	      strcpy(file_name2,dt->d_name);
	      strcat(file_name2,"_statistica.txt");
	      writeForRegularFile(file,dt->d_name,buffer2,st);
	      sprintf(path2,"%s/%s",file2,file_name2);
	      output2=open(path2,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP);
	      write(output2,buffer2,strlen(buffer2));
	      if( close(output2)==-1)
		{
		  perror("The normal file couldn't close normally\n");
		  exit(-1);
		}
	      exit(8);
	    }
	}
    }
   else if(S_ISLNK(st.st_mode))
    {
      if(readlink(filePath,linkbuff,sizeof(linkbuff)))
	{
	cnt++;
	  pid3=fork();
	  if(pid3==0)
	    {
	     
	      strcpy(file_name3,dt->d_name);
	      strcat(file_name3,"_statistica.txt");
	      writeForSymbolicLink(file,dt->d_name,buffer3,st);
	      sprintf(path3,"%s/%s",file2,file_name3);
	      output3=open(path3,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP);
	      write(output3,buffer3,strlen(buffer3));
	      if( close(output3)==-1)
		{
		  perror("The the symbolik link file couldn't close normally\n");
		  exit(-1);
		}
	      exit(5);
	    }
	}
	}
  else if(S_ISDIR(st.st_mode))
    {
      cnt++;
      pid4=fork();
      if(pid4==0)
	{
	  
	  strcpy(file_name4,dt->d_name);
	  strcat(file_name4,"_statistica.txt");
	  writeForDirectory(file,dt->d_name,buffer4,st);
	  sprintf(path4,"%s/%s",file2,file_name4);
	  output4=open(path4,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP);
	  write(output4,buffer4,strlen(buffer4));
	   if( close(output4)==-1)
		{
		  perror("The directory file couldn't close normally\n");
		  exit(-1);
		}
	  exit(5);
	}
    }
}


int main(int argc,char **argv)
{
  DIR *dire;
  DIR *dir1;
  int wpid;
  if(argc!=3)
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

    dir1=opendir(argv[2]);
  if(dir1==NULL)
    {
      perror("Error opening directory\n");
      exit(-1);
    }
  dt2=readdir(dir1);
  
  while(dt!=NULL)
    {
      if(strcmp(dt->d_name,".")!=0 && strcmp(dt->d_name,"..")!=0)
	{
          verifyTypeOfFile(argv[1],argv[2]);
	}
      dt = readdir(dire);
    }

  for(int i=1;i<=cnt;i++)
    {
      wpid=wait(&status);
      if(WIFEXITED(status))
	{
	  printf("Procesul %d s-a terminat cu statusul %d\n",wpid,WEXITSTATUS(status));   
	}
    }
 if(closedir(dire)==-1)
   {
     perror("Error closing directory 1 \n");
     exit(-1);
   }
 if(closedir(dir1)==-1)
   {
     perror("Error closing directory 2 \n");
     exit(-1);
   }
  return 0;
}
