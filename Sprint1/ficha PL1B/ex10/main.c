#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


int main(){
    for(; ;){
        printf("I’m working!\n");
        sleep(1);
    }
}