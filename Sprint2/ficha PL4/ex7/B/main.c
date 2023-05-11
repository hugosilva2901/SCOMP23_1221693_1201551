#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#define NUM_PROCESSES 6

sem_t *sem_chips;
sem_t *sem_beer;
sem_t *sem_eat;
int *chips_count;
int *beer_count;

void buy_chips(int id) {
    printf("Process %d: Buying chips\n", id);
    (*chips_count)++;
    if ((*chips_count + *beer_count) == NUM_PROCESSES ) {
        for (int i = 0; i < NUM_PROCESSES; i++) {
            sem_post(sem_eat);
        }
    }
}

void buy_beer(int id) {
    printf("Process %d: Buying beer\n", id);
    (*beer_count)++;
    if ((*chips_count + *beer_count) == NUM_PROCESSES ) {
        for (int i = 0; i < NUM_PROCESSES; i++) {
            sem_post(sem_eat);
        }
    }
}


void eat_and_drink(int id) {
    printf("Process %d: Eating and drinking\n", id);
}

int main() {
    sem_chips = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_beer = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_eat = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    chips_count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    beer_count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sem_init(sem_chips, 1, 0);
    sem_init(sem_beer, 1, 0);
    sem_init(sem_eat, 1, 0);
    *chips_count = 0;
    *beer_count = 0;

    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            int id = i + 1;
            sleep(1);
            if ((rand()) % 2 == 0) {
                buy_chips(id);
            } else {
                buy_beer(id);
            }
            sem_wait(sem_eat);
            eat_and_drink(id);
            exit(0);
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }

    sem_destroy(sem_chips);
    sem_destroy(sem_beer);
    sem_destroy(sem_eat);

    munmap(sem_chips, sizeof(sem_t));
    munmap(sem_beer, sizeof(sem_t));
    munmap(sem_eat, sizeof(sem_t));
    munmap(chips_count, sizeof(int));
    munmap(beer_count, sizeof(int));

    return 0;
}