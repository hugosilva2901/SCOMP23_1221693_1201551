#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <time.h>
typedef struct 
{
    int integer;
    char buffer[1024];
}date;

int main(void){
    int pipe_fd[2];
    pid_t pid;
    date data;
    if (pipe(pipe_fd) < 0) {
        perror("pipe");
        exit(0);
    }
    pid = fork();
    if(pid<0){
        perror("fork");
        exit(0);
    }else if(pid>0){
        close(pipe_fd[0]);
        printf("Enter an integer: ");
        scanf("%d", &data.integer);
        printf("Enter a string: ");
        scanf("%s", data.buffer);
        write(pipe_fd[1], &data, sizeof(data));
        close(pipe_fd[1]);
    }
    else{
        close(pipe_fd[1]);
        read(pipe_fd[0], &data, sizeof(data));
        printf("Received integer: %d, string: %s\n",data.integer,data.buffer);
        close(pipe_fd[0]);
    }
    return 0;
}