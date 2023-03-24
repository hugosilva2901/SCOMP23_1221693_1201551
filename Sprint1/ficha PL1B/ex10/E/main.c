#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <time.h>
volatile atomic_int USR1_counter = 0;

void handle_USR1(int signum) {
    sleep(1); // Sleep for 1 second
    USR1_counter++;
    printf("USR1 counter = %d\n", USR1_counter);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_USR1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
    for(; ;){
        printf("I’m working!\n");
        sleep(1);
    }
    return 0;
}





