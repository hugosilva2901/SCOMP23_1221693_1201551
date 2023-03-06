#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 1000
int main(void) {
    int status[5];
    //int status=0;
int status1=0;
int status2=0;
int status3=0;
int status4=0;
 int result=0;
int numbers[ARRAY_SIZE]; /* array to lookup */
 for (int i = 0; i < ARRAY_SIZE; i++){
    numbers[i] = rand() % 256;
 }
 int size=ARRAY_SIZE/5;
 // A)
pid_t p = fork();
if (p == 0) {
    
    for (int i = 0; i < size; i++)
    {
       if(result<numbers[i]){
              result=numbers[i];
       }
    }
    printf("1:%d\n",result);
    exit(result);
}
pid_t p1 = fork();
if (p1 == 0) {

    for (int i = size; i < size*2; i++)
    {
       if(result<numbers[i]){
              result=numbers[i];
       }
    }
     printf("2:%d\n",result);
    exit(result);
}
pid_t p2 = fork();
if (p2 == 0) {
    for (int i = size*2; i < size*3; i++)
    {
       if(result<numbers[i]){
              result=numbers[i];
       }
    }
     printf("3:%d\n",result);
    exit(result);
}
pid_t p3 = fork();

if (p3 == 0) {

    for (int i = size*3; i < size*4; i++)
    {
       if(result<numbers[i]){
              result=numbers[i];
       }
    }
     printf("4:%d\n",result);
    exit(result);
}
pid_t p4 = fork();
if (p4 == 0) {
   
    for (int i = size*4; i < size*5; i++)
    {
       if(result<numbers[i]){
              result=numbers[i];
       }
    }
     printf("5:%d\n",result);
    exit(result);
}
for (int i = 0; i < 5; i++)
{
    wait(&(status[i]));
}
//B)
for (int i = 0; i < 5; i++)
{
    if (result<WEXITSTATUS(status[i])){
        result=WEXITSTATUS(status[i]);
    }
}

int res[ARRAY_SIZE];
printf("%d\n",result);
for (int  i = 0; i < ARRAY_SIZE/2; i++)
{
	
    res[i]=((int)numbers[i]/result)*100;
	printf("%d\n",res[i]);
}
//C)

for (int i = ARRAY_SIZE/2; i < ARRAY_SIZE; i++)
{
    res[i]=( numbers[i]/result)*100;
    printf("%d\n",res[i]);

}


}
