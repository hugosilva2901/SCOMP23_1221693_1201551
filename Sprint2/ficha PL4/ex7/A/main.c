#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>

sem_t *sem_chips;
sem_t *sem_beer;
sem_t *sem_eat;

void buy_chips() {
    printf("Process 1: Buying chips\n");
    sem_post(sem_chips);
}

void buy_beer() {
    printf("Process 2: Buying beer\n");
    sem_post(sem_beer);
}

void eat_and_drink() {
    printf("Eating and drinking\n");
}

int main() {
    sem_chips = sem_open("/sem_chips", O_CREAT, 0644, 0);
    sem_beer = sem_open("/sem_beer", O_CREAT, 0644, 0);
    sem_eat = sem_open("/sem_eat", O_CREAT, 0644, 0);

    pid_t pid = fork();

    if (pid == 0) {
        sleep(rand() % 5);
        buy_chips();
        sem_wait(sem_eat);
        eat_and_drink();
        exit(0);
    } else {
        sleep(rand() % 5);
        buy_beer();
        sem_wait(sem_chips);
        sem_wait(sem_beer);
        sem_post(sem_eat);
        eat_and_drink();
        wait(NULL);
    }

    sem_destroy(sem_chips);
    sem_destroy(sem_beer);
    sem_destroy(sem_eat);


    return 0;
}