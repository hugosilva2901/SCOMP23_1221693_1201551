#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


// Change the program so that each time a SIGQUIT signal is received the “I won’t let the process
// end by pressing CTRL-\!” message is displayed.
// Recompile and run the program and test by pressing several times CTRL-\.


void sigint_handler(int sig) {
    printf("I won't let the process end with CTRL- \\ !\n");
}

int main() {

    //Printing the process ID
    printf("Process ID: %d \n", getpid());

    

    // if the signal received is SIGQUIT, call the sigint_handler function, which prints the message.
    if (signal(SIGQUIT, sigint_handler) == SIG_ERR) {
        perror("signal"); 
        exit(1);
    }

    for (;;) {
        printf("I like signals\n");
        sleep(1);
    }

    return 0;
}