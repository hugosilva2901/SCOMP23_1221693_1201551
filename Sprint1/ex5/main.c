#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
int main(void) {
int status1=0;
int status=0;
  pid_t p = fork();
    if (p == 0) {
        sleep(1);
        exit(1);
    } 
    pid_t p1 = fork();
   
        if (p1 == 0) {
        sleep(2);
            exit(2);
        }
    wait( &status1);
    wait( &status);  
    printf("%d\n",WEXITSTATUS(status));
    printf("%d",WEXITSTATUS(status1));
    
   
 }
