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
    int array[1000];
    int i;
    pid_t pid;
    const int SIZE = 10 * sizeof(int);
    for ( i = 0; i < 1000; i++)
    {
        array[i] = rand()%1001;
    }
    int *shared_array;
    int fd;
    fd = shm_open("/main", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, 10*sizeof(int));
    shared_array = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
     if (shared_array == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    for (int i = 0; i < 10; i++)
    {
       pid = fork();
         if (pid == -1) {
          perror("fork");
          exit(1);
        } else if (pid == 0) {  
            int temp=0;
            for(int j=i*100;j<(i+1)*100;j++){
                if(array[j]>temp){
                    shared_array[i] = array[j];
                    temp = array[j];
                }
            }
            close(fd);
            exit(0);
        }
    }
    for (int i = 0; i < 10; i++)
    {
        wait(NULL);
    }
    int temp=0;
    int GlobalMax;
    for (int i = 0; i < 10; i++)
    {
        if(shared_array[i]>temp){
            GlobalMax = shared_array[i];
            temp = shared_array[i];
        }
    }
    printf("GlobalMax :%d",GlobalMax);
    // cleanup shared memory
    munmap(shared_array, SIZE);
    close(fd);
    shm_unlink("/main");


    
    return 0;
}