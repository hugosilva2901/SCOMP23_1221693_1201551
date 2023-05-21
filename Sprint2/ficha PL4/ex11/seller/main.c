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

#define SEM_NAME_1 "/sem_ex11_1"
#define SEM_NAME_2 "/sem_ex11_2"
#define SEM_NAME_3 "/sem_ex11_3"
#define SHM_NAME "/shm_ex11"

#define NR_CHILDREN 5
sem_t *ticket_to_costumer;
sem_t *wait_for_tikect ;
typedef struct
{
    int ticketNumber;
} Ticket;

int main()
{

    wait_for_tikect = sem_open("/sem1", O_CREAT , 0644, 0);
    if (wait_for_tikect == SEM_FAILED)
    {
        perror("Error opening semaphore ");
        exit(EXIT_FAILURE);
    }

    ticket_to_costumer = sem_open("/sem2", O_CREAT , 0644, 0);
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
    ticket = 0;
    for (int i = 0; i < NR_CHILDREN; i++)
    {
        ticket++;
        printf("Ticket %d reeady\n",ticket);
        //sleep(2);
        sem_post(wait_for_tikect);
        sem_wait(ticket_to_costumer);
  
  }
    if (sem_close(wait_for_tikect) == -1)
    {
        perror("Cant close semaphore");
        exit(EXIT_FAILURE);
    }
    if (sem_close(ticket_to_costumer) == -1)
    {
        perror("Cant close semaphore");
        exit(EXIT_FAILURE);
    }

    return 0;
}