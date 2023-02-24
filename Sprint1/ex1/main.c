#include <stdio.h>
#include <stdint.h> 
#include <sys/types.h>
#include <unistd.h>
int main(void) {
int x = 1;
pid_t p = fork(); /*pid_t: sys/types.h; fork(): unistd.h*/
if (p == 0) {
 x = x+1;
 printf("1. x = %d\n", x);
 } else {
 x = x-1;
 printf("2. x = %d\n", x);
 }
 printf("3. %d; x = %d\n", p, x);
 } 

/*
1)A)O processo filho e o processo pai competem pelo cpu se o primeiro a ser executado for o processo filho o output vai ser 
2. x = 0  1-1=0
3. 1234; x = 0  1234 o PID do processo do filho 1234
1. x = 2  1+1=2
3. 0; x = 2  e o processo pai por isso nao tem o processo PID é 0
1)B) Sim se o pai ganhar a competição pelo cpu 
*/