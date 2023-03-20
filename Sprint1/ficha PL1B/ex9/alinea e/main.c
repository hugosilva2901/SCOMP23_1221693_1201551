#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


// Change the program so that each time a SIGQUIT signal is received the “I won’t let the process
// end by pressing CTRL-\!” message is displayed.

void handle_SIGQUIT(int sig) {
    printf("I won't let the process end with CTRL- \\ !\n");
}

int main() {
struct sigaction action;
    printf("Pid do processo: %d \n ", getpid());

    /* Clear the act variable. */
    memset(&action, 0, sizeof(struct sigaction));
    sigemptyset(&action.sa_mask);/*No signals blocked*/
    action.sa_handler = handle_SIGQUIT;
    sigaction(SIGQUIT, &action, NULL);

    for (;;) {
        printf("I like signals\n");
        sleep(1);
    }

    return 0;
}