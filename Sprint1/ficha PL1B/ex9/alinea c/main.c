#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


// Modify the program to print the “I won’t let the process end with CTRL-C!” message each time
// a SIGINT signal is received.

void sigint_handler(int sig) {
    printf("I won't let the process end with CTRL-C!\n");
}

int main() {

    // If the signal received is SIGINT, call the sigint_handler function, which prints the message.
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("signal"); 
        exit(1);
    }

    for (;;) {
        printf("I like signals\n");
        sleep(1);
    }

    return 0;
}