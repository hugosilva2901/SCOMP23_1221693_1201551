#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
int main(void) {
int status=0;
int status1=0;
int status2=0;
int status3=0;
int status4=0;
int status5=0;
int status6=0;
int status7=0;
int status8=0;
int status9=0;
int i=0;
int n=0;
  pid_t p = fork();
  if (p == 0) 
  {
    n=0;
   for ( i = 0; i < 100; i++)
   {
   printf("%d\n",n);
    n++;
   }
   exit(0);
  }
    pid_t p1 = fork();
  if (p1 == 0) 
  {
    n=100;
   for ( i = 0; i < 100; i++)
   {
   printf("%d\n",n);
    n++;
   }
   exit(0);
  }
    pid_t p2 = fork();
  if (p2 == 0) 
  {
    n=200;
   for ( i = 0; i < 100; i++)
   {
   printf("%d\n",n);
    n++;
   }
   exit(0);
  }
    pid_t p3 = fork();
  if (p3 == 0) 
  {
    n=300;
   for ( i = 0; i < 100; i++)
   {
   printf("%d\n",n);
    n++;
   }
   exit(0);
  }
    pid_t p4 = fork();
  if (p4 == 0) 
  {
    n=400;
   for ( i = 0; i < 100; i++)
   {
   printf("%d\n",n);
    n++;
   }
   exit(0);
  }
    pid_t p5 = fork();
  if (p5 == 0) 
  {
    n=500;
   for ( i = 0; i < 100; i++)
   {
   printf("%d\n",n);
    n++;
   }
   exit(0);
  }
    pid_t p6 = fork();
  if (p6 == 0) 
  {
    n=600;
   for ( i = 0; i < 100; i++)
   {
   printf("%d\n",n);
    n++;
   }
   exit(0);
  }
    pid_t p7 = fork();
  if (p7 == 0) 
  {
    n=700;
   for ( i = 0; i < 100; i++)
   {
   printf("%d\n",n);
    n++;
   }
   exit(0);
  }
    pid_t p8 = fork();
  if (p8 == 0) 
  {
    n=800;
   for ( i = 0; i < 100; i++)
   {
   printf("%d\n",n);
    n++;
   }
   exit(0);
  }
    pid_t p9 = fork();
   if (p9 == 0) 
  {
    n=900;
   for ( i = 0; i < 100; i++)
   {
   printf("%d\n",n);
    n++;
   }
   exit(0);
  }
  wait(&status);
    wait(&status1);
    wait(&status2);
    wait(&status3);
    wait(&status4);
    wait(&status5);
    wait(&status6);
    wait(&status7);
wait(&status8);
wait(&status9);
    
  
}

/*9)A) não pois os processos filhos competem pelo cpu e podem ser executado por ordem diferente 
*/