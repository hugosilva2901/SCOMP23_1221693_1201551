#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
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
        int fd = open("file.txt", O_RDONLY);
        if (fd < 0) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        size_t read_size;
        while ((read_size = read(fd, buffer, 1024)) > 0) {
            write(pipe_fd[1], buffer, read_size);
        }
        close(fd); 
        close(pipe_fd[1]); 
        wait(NULL);
    } else {
       close(pipe_fd[1]);
        size_t read_size;
        while ((read_size = read(pipe_fd[0], buffer, 1024)) > 0) {
            write(STDOUT_FILENO, buffer, read_size);
        }
        close(pipe_fd[0]); 
    }
    return 0;
}