#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <time.h>
int main(void){
 int pipe_fd[2];
    pid_t pid;
    char buffer[1024];
    if (pipe(pipe_fd) < 0) {
        perror("pipe");
        exit(0);
    }
    pid = fork();
    
    if (pid < 0) {
        perror("fork");
        exit(0);
    } else if (pid > 0) {
        close(pipe_fd[0]); 
        int integer;
        printf("Enter an integer: ");
        scanf("%d", &integer);
        printf("Enter a string: ");
        scanf("%s", buffer);
        write(pipe_fd[1], &integer, sizeof(integer));
        write(pipe_fd[1], buffer, 1024);
        close(pipe_fd[1]); 
    } else {
        close(pipe_fd[1]); 
        int integer;
        read(pipe_fd[0], &integer, sizeof(integer));
        read(pipe_fd[0], buffer, 1024);
        printf("Received integer: %d, string: %s\n", integer, buffer);
        close(pipe_fd[0]); 
    }
    return 0;
}