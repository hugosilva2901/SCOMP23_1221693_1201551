#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CAPACITY 10

int main() {
    sem_t *sem = sem_open("/club_sem", O_CREAT, 0644, MAX_CAPACITY);

    for (int i = 0; i <MAX_CAPACITY*2 ; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            sem_wait(sem);
            printf("Client entered.\n");
            sleep(5);
            printf("Client left.\n");
            sem_post(sem);
            exit(0);
        }
    }

    for (int i = 0; i < MAX_CAPACITY*2; i++) {
        wait(NULL);
    }

    sem_close(sem);
    sem_unlink("/club_sem");
    return 0;
}
