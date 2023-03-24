#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_handler(int sig) {
    exit(0);
}

void parent_handler(int sig) {
    printf("You were too slow and this program will end!\n");
    kill(0, SIGTERM);
}

int main() {
    char str[100];
    pid_t pid;
    signal(SIGUSR1, child_handler);
    signal(SIGALRM, parent_handler);
    pid = fork();
    if (pid == 0) {
        alarm(10);
        pause();
        exit(0);
    } else {
        printf("Enter a string: ");
        fgets(str, 100, stdin);
        kill(pid, SIGUSR1);
        wait(NULL);
        int a= strlen(str)-1;
        printf("The size of the string is: %d\n",a);
        return 0;
    }
}