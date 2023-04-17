#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>


int main() {
    const int NUM_ITERATIONS = 1000000;
    const int SIZE = 2 * sizeof(int);

    int *shared_array;
    int fd;
    fd = shm_open("/main", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, SIZE);
    shared_array = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_array == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    
    shared_array[0] = 10000;
    shared_array[1] = 500;

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {  
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            shared_array[0]++;
            shared_array[1]--;
        }
        close(fd);
        exit(0);
    } else { 
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            shared_array[0]--;
            shared_array[1]++;
        }
        wait(NULL);
    }
    printf("first value :%d\n",shared_array[0]);
     printf("second value :%d",shared_array[1]);
    // cleanup shared memory
    munmap(shared_array, SIZE);
    close(fd);
    shm_unlink("/shmtest");

    return 0;
}