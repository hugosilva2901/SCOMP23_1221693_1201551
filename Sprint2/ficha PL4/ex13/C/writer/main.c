#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define MAX_WRITERS 5
#define SHM_KEY 1234

struct shared_memory
{
    char string[100];
    int readers;
    int writers;
};

struct shared_memory *shared_mem;
sem_t *mutex;
sem_t *write_mutex;

void signal_handler(int signal)
{
    sem_close(mutex);
    sem_unlink("/mutex");
    sem_close(write_mutex);
    sem_unlink("/write_mutex");
    exit(0);
}

void writer(int id)
{
    sem_wait(write_mutex);
    sem_wait(mutex);
    sprintf(shared_mem->string, "Writer %d: PID: %d Time: ", id, getpid());
    shared_mem->writers++;
    printf("Writer %d: (Writers: %d Readers: %d)\n", id, shared_mem->writers, shared_mem->readers);
    sem_post(mutex);
    sem_post(write_mutex);
}

int main()
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    int fd;
    if (shm_open("/shared_mem", O_CREAT | O_EXCL, 0644) == -1)
    {
        fd = shm_open("/shared_mem", O_CREAT | O_RDWR, 0644);
        if (fd == -1)
        {
            perror("shm_open");
            exit(1);
        }
    }
    else
    {
        fd = shm_open("/shared_mem", O_CREAT | O_RDWR, 0644);
        if (fd == -1)
        {
            perror("shm_open");
            exit(1);
        }
        if (ftruncate(fd, sizeof(struct shared_memory)) == -1)
        {
            perror("ftruncate");
            exit(1);
        }
    }

    shared_mem = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_mem == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    mutex = sem_open("/mutex", O_CREAT, 0644, 1);
    if (mutex == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    write_mutex = sem_open("/write_mutex", O_CREAT, 0644, 1);
    if (write_mutex == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    shared_mem->readers = 0;
    shared_mem->writers = 0;
    for (int i = 0; i < MAX_WRITERS; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            writer(i);
            exit(0);
        }
        else if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
    }

    for (int i = 0; i < MAX_WRITERS; i++)
    {
        if (wait(NULL) == -1)
        {
            perror("wait");
            exit(1);
        }
    }

    if (shared_mem->readers == 5)
    {
        if (sem_unlink("/mutex") == -1)
        {
            perror("sem_unlink");
            exit(1);
        }
        if (sem_unlink("/write_mutex") == -1)
        {
            perror("sem_unlink");
            exit(1);
        }
        if (shm_unlink("/shared_mem") == -1)
        {
            perror("shm_unlink");
            exit(1);
        }
    }
    return 0;
}
