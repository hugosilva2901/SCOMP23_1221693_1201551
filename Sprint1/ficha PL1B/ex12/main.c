#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdatomic.h>
void child_handler(int sig) {
    exit(0);
}

int main(){
    int n = 5;
    pid_t pid[n];
    atomic_int count = 5;
    int i,status[5];
    
     for (i = 1; i <= n; i++) {
        pid[i]= fork();
        if (pid[i] < 0) { 
            exit(1);
        } else if (pid[i] == 0) { // sucesso ao criar processo filho
            signal(SIGUSR1, child_handler);
            printf("Child %d: ", i);
            for (int j = i * 200; j < (i + 5) * 200; j++) {
                printf(" %d ", j);
            }
            printf("\n");
            kill(getppid(), SIGUSR1);
            exit(0);
        }
    }    
    while (count > 0) {
        pause();
    }
    for (i = 0; i < 5; i++) {
        waitpid(pid[i], &status[i], 0);
    }
    printf("All children have completed their tasks.\n");

    return 0;
}