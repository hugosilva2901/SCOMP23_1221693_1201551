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
    int vec1[1000];
    int vec2[1000];
int i=0;
int status[5], temp, total_sum = 0;
for ( i = 0; i < 1000; i++)
{
    vec1[i] = random()%21;
    vec2[i] = random()%21;
}
    printf("%d\n",vec1[0]);
    printf("%d\n",vec2[0]);

    int pipe_fd[2];
    pid_t pid[5];
    if (pipe(pipe_fd) < 0) {
        perror("pipe");
        exit(0);
    }
    for (i = 0; i < 5; i++)
    {
        pid[i] = fork();
        if (pid[i] == -1) {
            perror("fork");
            exit(0);
        } else if (pid[i] == 0) {
            close(pipe_fd[0]);

             temp=0;
            for (int j = 0+(200*i); j < 200*(i+1); j++)
            {
                temp+=vec1[j]+vec2[j];
             
            }
          
             if (write(pipe_fd[1], &temp, sizeof(int)) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            close(pipe_fd[1]); 
            exit(EXIT_SUCCESS);
        }
    }
            close(pipe_fd[1]);
            for (int i = 0; i < 5; i++)
            {
               if (read(pipe_fd[0], &temp, sizeof(int)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);

        }
        total_sum += temp;
            }
            close(pipe_fd[0]); 
       
       for (i = 0; i < 5; i++) {
       waitpid(pid[i], &status[5], 0);
        }
       
        printf("\n%d\n", total_sum);
    

    return 0;
}