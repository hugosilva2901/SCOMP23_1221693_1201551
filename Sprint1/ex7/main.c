#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 1000

 int main ()
 { 
    int status=0;
int numbers[ARRAY_SIZE]; /* array to lookup */
 int n; /* the number to find */
 time_t t; /* needed to init. the random number generator (RNG)*/
 int i;
 /* intializes RNG (srand():stdlib.h; time(): time.h) */
 srand ((unsigned) time (&t));

 /* initialize array with random numbers (rand(): stdlib.h) */
 for (i = 0; i < ARRAY_SIZE; i++)
    numbers[i] = rand () % 10000;

 /* initialize n */
 n = rand () % 10000;
  int sizes=ARRAY_SIZE /2 ;
 pid_t p = fork();
    if (p == 0) {
        int ns=0;
       
        for (i = 0; i <sizes ; i++){
        if (numbers[i] == n) {
            ns++;
             
        }
        printf("%d %d\n ",numbers[i],i);
        printf("%d\n ",n);
        }
        exit(ns);
        }
        int ns1=0;
        for (int j = sizes; j <ARRAY_SIZE; j++){
        if (numbers[j] == n) {
            ns1++;
        }
        printf("%d %d\n ",numbers[i],i);
        printf("%d\n ",n);
        }
        wait( &status);
        printf("%d\n",WEXITSTATUS(status));
       int a =ns1+ WEXITSTATUS(status);
        printf("%d",a);
        }
 
