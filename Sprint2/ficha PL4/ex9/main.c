#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s n time\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    int time = atoi(argv[2]);

    sem_t *sem = sem_open("/testpar_sem", O_CREAT, 0644, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            sem_wait(sem);
            usleep(time);
            sem_post(sem);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < n; i++) {
        sem_post(sem);
    }

    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    gettimeofday(&end, NULL);
    long elapsed = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
    printf("Elapsed time: %ld us\n", elapsed);

    sem_close(sem);
    sem_unlink("/testpar_sem");

    return 0;
}