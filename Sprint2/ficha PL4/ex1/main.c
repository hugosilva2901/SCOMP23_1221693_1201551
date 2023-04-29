#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main(){
    pid_t pid[8];
    int readcount = 0;
    int writecount = 0;
    int mutex1 = 1;
    int mutex2 = 1;
    int mutex3=1;
    int w=1;
    int r=1;
    int *i1; /* shared mem */
    int *i2; /* shared mem */
    sem_t *sem;

    FILE* fp_in = fopen("Numbers.txt", "r");
    if (fp_in == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }
    setbuf("Numbers.txt",NULL);
    FILE* fp_out = fopen("Output.txt", "w");
    if (fp_out == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    return 0;
}