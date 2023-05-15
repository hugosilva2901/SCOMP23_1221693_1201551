#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_WRITERS 5
#define SHM_KEY 1234

struct shared_memory {
    char string[100];
    int readers;
    int writers;
};

struct shared_memory *shared_mem;
sem_t *mutex;
sem_t *write_mutex;

void signal_handler(int signal) {
    sem_close(mutex);
    sem_unlink("/mutex");
    sem_close(write_mutex);
    sem_unlink("/write_mutex");
    exit(0);
}

void writer(int id) {
    sem_wait(write_mutex);
    sem_wait(mutex);
    time_t t = time(NULL);
    sprintf(shared_mem->string, "Writer %d: PID: %d Time: %s", id, getpid(), ctime(&t));
    shared_mem->writers++;
    printf("Writer %d: (Writers: %d Readers: %d)\n", id, shared_mem->writers, shared_mem->readers);
    sem_post(mutex);
    sem_post(write_mutex);
}

int main() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    int segment_id = shmget(SHM_KEY, sizeof(struct shared_memory), S_IRUSR | S_IWUSR | IPC_CREAT);
    if (segment_id == -1) {
        perror("shmget");
        exit(1);
    }

    shared_mem = (struct shared_memory *) shmat(segment_id, NULL, 0);
    if (shared_mem == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    mutex = sem_open("/mutex", O_CREAT, 0644, 1);
    if (mutex == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    write_mutex = sem_open("/write_mutex", O_CREAT, 0644, 1);
    if (write_mutex == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    shared_mem->readers = 0;
    shared_mem->writers = 0;
   for (int i = 0; i < MAX_WRITERS; i++) {
       pid_t pid = fork();
       if (pid == 0) {
           writer(i);
           exit(0);
       } else if (pid == -1) {
           perror("fork");
           exit(1); 
       }
   }

   for (int i = 0; i < MAX_WRITERS; i++) {
       if (wait(NULL) == -1) {
           perror("wait");
           exit(1); 
       }
   }

   return 0;
}
