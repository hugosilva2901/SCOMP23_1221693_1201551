#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    int pipefd[2];
    int pipefdChild[2];
    pid_t pid;
    int credit = 20;
    int status;
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefdChild) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
        close(pipefd[1]);
        close(pipefdChild[0]);
        int bet;
        srand(bet);
        while (1) {
            
            int credit;
            int signal;
            read(pipefd[0], &signal, sizeof(signal));
            if (signal == 0) {
                printf("Child process terminating\n");
                exit(EXIT_SUCCESS);
            }

            bet = (rand() % 5) + 1;
            printf("Child process betting %d\n", bet);

            write(pipefdChild[1], &bet, sizeof(bet));

            read(pipefd[0], &credit, sizeof(credit));
            printf("Child process new credit: %d euros\n", credit);
        }
/*
        close(pipefd[0]);
        close(pipefd[1]);
        exit(EXIT_SUCCESS);
        */
    }
    else {
        close(pipefd[0]);
        close(pipefdChild[1]);
        
        while (credit > 0) {
            int number;
            int bet;
            srand(bet);
            number = (rand() % 5) + 1;
            printf("\nParent process generated %d\n", number);

            int signal = 1;
            write(pipefd[1], &signal, sizeof(signal));

            
            read(pipefdChild[0], &bet, sizeof(bet));
            printf("Parent process read bet %d\n", bet);
            if (number == bet) {
                credit += 10;
            }
            else {
                credit -= 5;
            }
            write(pipefd[1], &credit, sizeof(credit));
            sleep(1);
        }

        int signal = 0;
        write(pipefd[1], &signal, sizeof(signal));

        wait(&status);

        printf("Parent process terminating\n");
        close(pipefd[0]);
        close(pipefd[1]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}