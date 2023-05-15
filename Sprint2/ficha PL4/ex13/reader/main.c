#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>

#define MAX_READERS 5
#define SHM_KEY 1234

struct shared_memory {
    char string[100];
    int readers;
};

struct shared_memory *shared_mem;
sem_t *mutex;

void reader(int id) {
    sleep(1);
    sem_wait(mutex);
    printf("Reader %d: %s (Readers: %d)\n", id, shared_mem->string, shared_mem->readers);
    shared_mem->readers++;
    sem_post(mutex);
}

int main() {
    int segment_id = shmget(SHM_KEY, sizeof(struct shared_memory), S_IRUSR | S_IWUSR);
    shared_mem = (struct shared_memory *) shmat(segment_id, NULL, 0);

    mutex = sem_open("/mutex", O_CREAT, 0644, 1);

    for (int i = 0; i < MAX_READERS; i++) {
        if (fork() == 0) {
            reader(i);
            exit(0);
        }
    }

    for (int i = 0; i < MAX_READERS; i++) {
        if (wait(NULL) == -1) {
            perror("wait");
            exit(1);
        }
    }

    if (sem_close(mutex) == -1) {
        perror("sem_close");
        exit(1);
    }

    if (sem_unlink("/mutex") == -1) {
        perror("sem_unlink");
        exit(1);
    }
    
    return 0;
}