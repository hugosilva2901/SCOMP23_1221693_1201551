#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
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
    sem_chips = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_beer = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_eat = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sem_init(sem_chips, 1, 0);
    sem_init(sem_beer, 1, 0);
    sem_init(sem_eat, 1, 0);

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

    munmap(sem_chips, sizeof(sem_t));
    munmap(sem_beer, sizeof(sem_t));
    munmap(sem_eat, sizeof(sem_t));

    return 0;
}