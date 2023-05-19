#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CAPACITY 10

typedef enum { VIP, SPECIAL, NORMAL } client_type;

typedef struct {
    client_type type;
    int duration;
} client;

sem_t *sem_vip;
sem_t *sem_special;
sem_t *sem_normal;

void enter_club(client c) {
    switch (c.type) {
        case VIP:
            sem_wait(sem_vip);
            printf("VIP client entered.\n");
            break;
        case SPECIAL:
            sem_wait(sem_special);
            sem_wait(sem_vip);
            printf("Special client entered.\n");
            break;
        case NORMAL:
            sem_wait(sem_normal);
            sem_wait(sem_special);
            sem_wait(sem_vip);
            printf("Normal client entered.\n");
            break;
    }
}

void leave_club(client c) {
    switch (c.type) {
        case VIP:
            printf("VIP client left.\n");
            sem_post(sem_vip);
            break;
        case SPECIAL:
            printf("Special client left.\n");
            sem_post(sem_vip);
            sem_post(sem_special);
            break;
        case NORMAL:
            printf("Normal client left.\n");
            sem_post(sem_vip);
            sem_post(sem_special);
            sem_post(sem_normal);
            break;
    }
}

int main() {
    sem_vip = sem_open("/club_sem_vip", O_CREAT, 0644, MAX_CAPACITY);
    sem_special = sem_open("/club_sem_special", O_CREAT, 0644, 1);
    sem_normal = sem_open("/club_sem_normal", O_CREAT, 0644, 1);

    client clients[] = {
        { VIP, 2 },
        { SPECIAL, 1 },
        { NORMAL, 3 },
        { VIP, 1 },
        { SPECIAL, 2 },
        { NORMAL, 1 },
        { VIP, 3 },
        { SPECIAL, 1 },
        { NORMAL, 2 },
        { VIP, 1 },
        { SPECIAL, 3 },
        { VIP, 2 },
        { SPECIAL, 1 },
        { NORMAL, 3 },
        { VIP, 1 },
        { SPECIAL, 2 },
        { NORMAL, 1 },
        { VIP, 3 },
        { SPECIAL, 1 },
        { NORMAL, 2 },
        { VIP, 1 },
        { SPECIAL, 3 }

    };
    for (int i = 0; i < MAX_CAPACITY *2; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            enter_club(clients[i]);
            sleep(clients[i].duration);
            leave_club(clients[i]);
            exit(0);
        }
    }

    for (int i = 0; i < MAX_CAPACITY *2; i++) {
        wait(NULL);
    }

    sem_close(sem_vip);
    sem_unlink("/club_sem_vip");
    sem_close(sem_special);
    sem_unlink("/club_sem_special");
    sem_close(sem_normal);
    sem_unlink("/club_sem_normal");
    return 0;
}
