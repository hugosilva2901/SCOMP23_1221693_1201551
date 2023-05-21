#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

#define SEM_NAME_1 "/sem_ex11_1"
#define SEM_NAME_2 "/sem_ex11_2"
#define SEM_NAME_3 "/sem_ex11_3"
#define SHM_NAME "/shm_ex11"
sem_t *ticket_to_costumer;
sem_t *wait_for_tikect ;
typedef struct
{
    int ticketNumber;
} Ticket;

int main()
{
  wait_for_tikect = sem_open("/sem1", O_CREAT , 0644, 1);
    if (wait_for_tikect == SEM_FAILED)
    {
        perror("Error opening semaphore ");
        exit(EXIT_FAILURE);
    }

    ticket_to_costumer = sem_open("/sem2", O_CREAT , 0644, 1);
    if (ticket_to_costumer == SEM_FAILED)
    {
        perror("Error opening semaphore WARNING");
        exit(EXIT_FAILURE);
    }
    int *ticket;

    int fd;
        fd = shm_open("/shared_mem", O_CREAT | O_RDWR,0664);
    if (fd == -1)
    {
        perror("Error creating shared memory");
        exit(EXIT_FAILURE);
    }
    ticket = mmap(NULL, sizeof(int), PROT_READ, MAP_SHARED, fd, 0);
    
    if (ticket== MAP_FAILED){
        perror("Error on mmap");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 5; i++)
    {
        printf("test1\n");
        pid_t pid = fork();
        if (pid == 0)
        {
            printf("test2\n");
            sem_wait(wait_for_tikect);
            printf("test3\n");
            int ticket_number = *ticket;
            printf("Child %d got ticket number %d\n", getpid(), ticket_number);
            sem_post(ticket_to_costumer);
            exit(EXIT_SUCCESS);
        }
    }
    sem_close(wait_for_tikect);
    sem_close(ticket_to_costumer);
    return 0;
}
