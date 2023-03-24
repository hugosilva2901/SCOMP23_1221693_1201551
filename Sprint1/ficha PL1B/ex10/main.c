#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <time.h>
volatile atomic_int USR1_counter = 0;

void handle_USR1(int signum) {
   sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL); 
    sleep(1);
    USR1_counter++;
    printf("SIGUSR1 signal captured: USR1 counter = %d\n", USR1_counter);
    sigemptyset(&mask); 
    sigprocmask(SIG_SETMASK, &mask, NULL);
}

int main(){
    pid_t pid = fork();
    if (pid <0) {
        exit(1);
    } else if (pid == 0) {
        for (int i = 0; i < 12; i++) {
            kill(getppid(), SIGUSR1);
            struct timespec req={0,10000000};
            nanosleep(&req,NULL); 
        }
        kill(getppid(), SIGINT); // Send a SIGINT signal to the parent process
        exit(0);
    } else {
    
        signal(SIGUSR1, handle_USR1);
        for(; ;){
        printf("I’m working!\n");
        sleep(1);
    }
        return 0;
    }
    
}