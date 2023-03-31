#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

struct message {
    char text[10];
    int round;
};

int main() {
    int pipe_fd[2];
    pid_t pid;

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= 10; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            close(pipe_fd[1]); 
           struct message msg;
        while (1) {
        
        ssize_t n = read(pipe_fd[0], &msg, sizeof(msg));
        if (n == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (n == 0) {
            printf("Child %d exiting...\n", i);
            exit(EXIT_SUCCESS);
        }
        if (strcmp(msg.text, "Win") == 0 && msg.round == i) {
            printf("Child %d won in round %d\n", i, msg.round);
            exit(msg.round);
        }
    }
        }
    }

    close(pipe_fd[0]); 
    struct message msg;
    for (int i = 1; i <= 10; i++) {
        sprintf(msg.text, "Win");
        msg.round = i;
        write(pipe_fd[1], &msg, sizeof(msg));
        printf("Parent wrote message %s %d\n", msg.text, msg.round);
        sleep(2);
    }
    printf("Parent waiting for child processes to terminate...\n");
     int status[10];
    for (int i = 0; i < 10; i++) {
       
        if (WIFEXITED(status[i])) {
            pid_t pid = wait(&status[i]);
            int round = WEXITSTATUS(status[i]);
            printf("Child %d won in round %d\n", pid, round);
        }
    }
    printf("Parent exiting...\n");  
    close(pipe_fd[1]);
    return 0;
}