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
#define MAX_READRS 5
struct shared_memory
{
    char string[100];
    int readers;
    int writers;
};

struct shared_memory *shared_mem;
sem_t *mutex;

void reader(int id)
{
    sleep(1);
    printf("teste1\n");
    if (sem_wait(mutex) == -1)
    {
        perror("sem wait");
        exit(1);
    }
    printf("teste1\n");
    printf("Reader %d: %s (Readers: %d)\n", id, shared_mem->string, shared_mem->readers);
    shared_mem->readers++;
    if (sem_post(mutex) == -1)
    {
        perror("sem post");
        exit(1);
    }
}

int main()
{
    int fd;
    if (shm_open("/shared_mem", O_EXCL, 0644) == -1)
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
    shared_mem->readers = 0;
    for (int i = 0; i < MAX_READRS; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            printf("1\n");
            reader(i);
            printf("Terminating\n");
            exit(0);
        }
        else if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
    }

    for (int i = 0; i < MAX_READRS; i++)
    {
        wait(NULL);
        printf("terminated\n");
    }

    if (sem_close(mutex) == -1)
    {
        perror("sem_close");
        exit(1);
    }

    if (shared_mem->writers == 5)
    {
        if (sem_unlink("/mutex") == -1)
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
