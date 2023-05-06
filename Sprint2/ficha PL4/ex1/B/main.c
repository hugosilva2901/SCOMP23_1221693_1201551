#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define N_CHILDREN 8
#define MAX 200 

int main() {
    int num[MAX];
    sem_t *sem_numbers,*sem_out;
    sem_numbers = sem_open("sem", O_CREAT , 0666, 1);
    if(sem_numbers == SEM_FAILED){
        perror("Error opening semaphore");
        exit(1);
    }

    sem_out = sem_open("sem_out", O_CREAT , 0666, 1);

    int out = open("Output.txt", O_CREAT , 0666);
    if(out == -1) {
        perror("Error opening file");
        exit(1);
    }

    pid_t pid;
    for(int i = 0; i < N_CHILDREN; i++) {
        pid = fork();
        if(pid == -1){
            perror("Error creating child process");
            exit(1);
        }
        if(pid == 0){
            close(out); 
            FILE *in = fopen("Numbers.txt", "r");
            if(in == NULL){
                perror("Error opening file");
                exit(1);
            }
            int j = 0, numeros;
            while(fscanf(in, "%d", &numeros) == 1 && j < MAX) { 
                sem_wait(sem_numbers);
                num[j] = numeros; 
                j++;
                sem_post(sem_numbers);
            }
            fclose(in);
            sem_wait(sem_out);
            int fd_child_output = open("Output.txt", O_WRONLY | O_APPEND);
            if(fd_child_output == -1) {
                perror("Error opening file");
                exit(1);
            }
            for(int k = 0; k < j; k++){
                dprintf(fd_child_output, "[%d] %d\n", getpid(), num[k]);
            }
            close(fd_child_output);
            sem_post(sem_out);
            exit(0);
        }
    }

    for(int i = 0; i < N_CHILDREN; i++){
        wait(NULL);
    }

    struct stat st;     
    if(fstat(out, &st) == -1){
        perror("fstat");
        exit(1);
    }

    char* file = (char*)mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, out, 0);
    if(file == MAP_FAILED){
        perror("mmap");
        exit(1);
    }

    printf("%s", file);
    if(sem_unlink("sem") == -1) {
        perror("Error unlinking semaphore");
        exit(1);
    }
    if(sem_unlink("sem_out") == -1) {
        perror("Error unlinking semaphore");
        exit(1);
    }
    close(out);
    return 0;
}