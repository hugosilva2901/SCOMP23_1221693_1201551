#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


// Modify the program to print the “I won’t let the process end with CTRL-C!” message each time
// a SIGINT signal is received.
void handle_SIGINT(int sig) {
    printf("I won't let the process end with CTRL-C! \n");
}


int main() {
    struct sigaction action;
    printf("Pid do processo: %d \n ", getpid());

    /* Clear the act variable. */
    memset(&action, 0, sizeof(struct sigaction));
    sigemptyset(&action.sa_mask);/*No signals blocked*/
    action.sa_handler = handle_SIGINT;
    sigaction(SIGINT, &action, NULL);

    for (;;) {
        printf("I like signals\n");
        sleep(1);
    }

    return 0;
}