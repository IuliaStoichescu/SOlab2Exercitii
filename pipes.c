#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
  int pid,fd[2];
  
  if(pipe(fd)<0)
    {
      perror("Eroare la crearea pipe-ului\n");
      exit(1);
    }
  pid=fork();
  if(pid<0)
    {
      perror("Eroare!\n");
      exit(EXIT_FAILURE);
    }
  else if(pid==0)
    {
      char c;
      close(fd[0]);//inchidem capatul pt citire
      printf("Citeste un caracter: ");
      scanf("%c",&c);
      write(fd[1],&c,1);
      close(fd[1]);//inchidem capatul pt scriere
    }
  else
    {
      char d;
      close(fd[1]);//inchidem capatul pt scriere
      read(fd[0],&d,1);
      printf("Procesul parinte a luat caracterul %c\n",d);
      close(fd[0]);//inchidem capatul pt citire
    } 
}
