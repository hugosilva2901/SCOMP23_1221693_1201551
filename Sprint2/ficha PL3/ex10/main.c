#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define TAM_BUFFER 30
#define NVALORES 30

typedef struct {
    int buffer[TAM_BUFFER];
    int head; /* Próxima posição vazia */
    int tail; /* Próxima posição a ser consumida */
    int count; /* Número de elementos no buffer */
} buffer_circular;

int main() {
    int i, j;
    pid_t pid;
    buffer_circular *bc;
    int fd;

    int prod_pipe[2];
    int cons_pipe[2];

    if (pipe(prod_pipe) == -1 || pipe(cons_pipe) == -1) {
        perror("pipe");
        exit(1);
    }

   fd = shm_open("/shm_ex10", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, sizeof(buffer_circular));
    bc = (buffer_circular*)mmap(NULL, sizeof(buffer_circular), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    bc->head = 0;
    bc->tail = 0;
    bc->count = 0;
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }


    if (pid == 0) {
        int i;
        close(prod_pipe[1]);
        close(cons_pipe[0]);
         
        for (i = 1; i <= NVALORES; i++) {

            int value;
            read(prod_pipe[0], &value, sizeof(value)); 
            while (bc -> count == NVALORES); // do nothing - no free buffers
            
            bc -> buffer[bc -> head] = i; 
            bc -> head = (bc -> head + 1) % NVALORES; 
            bc -> count++; // increment count

            write(cons_pipe[1], &value, sizeof(value)); 

            printf("Producer sent: %d\n", i); 
        }
        close(prod_pipe[0]);
        close(cons_pipe[1]);
           exit(0);
    }else{
        
        pid = fork();
        close(prod_pipe[0]);
        close(cons_pipe[1]);
        if (pid < 0) {
            perror("fork error");
            exit(1);

        } else if (pid == 0) {

            int i, value;
            for (i = 1; i <= NVALORES; i++) {

                write(prod_pipe[1], &value, sizeof(value)); 
                while (bc -> count == 0); 
                
                value = bc -> buffer[bc -> tail]; 
                bc -> tail = (bc->tail + 1) % NVALORES; 
                bc -> count--; 
                
                printf("Consumer received: %d\n", value); 
            }
        close(prod_pipe[1]);
        close(cons_pipe[0]);
            exit(0);
        }
    }
              
      wait(NULL);

    munmap(bc, sizeof(buffer_circular));
    close(fd);
    shm_unlink("/shm_ex10");
    return 0;
}
