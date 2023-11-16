#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void procesFiu()
{
  execl("./6.1","6.1",6,NULL);
  //execl("./bin/ls""ls","-l", NULL)
  exit(3);
}

void procesTata()
{
  int status;
  char c;
  while(1)
    { 
      scanf("%c",&c);
      sleep(500);
      if(c=='a')
	{
	  break;
	}
    }
  int wpid=wait(&status);
  if(WIFEXITED(status))
    {
      printf("Procesul %d s-a terminat cu statusul %d\n",wpid,WEXITSTATUS(status));   
    }
}

int main()
{
  int pid;
  if((pid=fork())<0)
    {
      perror("Eroare la PID\n");
      exit(-1);
    }
  if(pid==0)
    {
      procesFiu();
    }
  else
    {
      procesTata();
    }

  return 0;
}
