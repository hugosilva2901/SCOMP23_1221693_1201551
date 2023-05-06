#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>


int main() {
    sem_t *sem_child1, *sem_child2, *sem_child3;

    sem_child1 = sem_open("sem_child1", O_CREAT, 0666, 0);
    sem_child2 = sem_open("sem_child2", O_CREAT, 0666, 0);
    sem_child3 = sem_open("sem_child3", O_CREAT, 0666, 0);

    pid_t pid1, pid2, pid3;
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(1);
    } else if (pid1 == 0) {
  // Child 1
        printf("Sistemas ");
        fflush(stdout);

        sem_post(sem_child2);

        sem_wait(sem_child1);
        printf("a ");
        fflush(stdout);
        sem_post(sem_child2);
        exit(0);
    } else {
        pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            exit(1);
        } else if (pid2 == 0) {
            // Child 2

            sem_wait(sem_child2);
            printf("de ");
            fflush(stdout);
            sem_post(sem_child3);

            sem_wait(sem_child2);

            printf("melhor ");
            fflush(stdout);
            sem_post(sem_child3);
            exit(0);
        } else {
            pid3 = fork();
            if (pid3 == -1) {
                perror("fork");
                exit(1);
            } else if (pid3 == 0) {
                // Child 3

                sem_wait(sem_child3);
                printf("Computadores- ");
                fflush(stdout);
                sem_post(sem_child1);

                sem_wait(sem_child3);
                
                printf("disciplina! ");
                fflush(stdout);
                sem_post(sem_child1);
                exit(0);
            } else {
                for (int i = 0; i < 3; i++)
                {
                    wait(NULL);
                }
                
                // Clean up semaphores
                sem_unlink("sem_child1");
                sem_unlink("sem_child2");
                sem_unlink("sem_child3");
                sem_close(sem_child1);
                sem_close(sem_child2);
                sem_close(sem_child3);
                exit(0);
            }
        }
    }
}