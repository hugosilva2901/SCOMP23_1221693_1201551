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
    for ( i = 0; i < 1000; i++)
    {
        array[i] = rand()%1001;
    }
    
    /*
  // Create an array of 1000 integers with random values between 0 and 1000
    int arr[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = rand() % 1000;
    }

    // Create a shared memory area to store an array of 10 integers, each containing the local maximum of 100 values
    key_t key = IPC_PRIVATE;
    int shmid = shmget(key, sizeof(int) * 10, IPC_CREAT | 0666);
    int* local_maxes = (int*) shmat(shmid, NULL, 0);

    // Create 10 new processes
    for (int i = 0; i < 10; i++) {
        int pid = fork();
        if (pid == 0) {  // child process
            // Determine the local maximum
            int local_max = arr[i * BLOCK_SIZE];
            for (int j = i * BLOCK_SIZE + 1; j < (i + 1) * BLOCK_SIZE; j++) {
                if (arr[j] > local_max) {
                    local_max = arr[j];
                }
            }

            // Store the local maximum in shared memory
            local_maxes[i] = local_max;
            exit(0);
        }
    }

    // Wait until the 10 child processes finish
    for (int i = 0; i < 10; i++) {
        wait(NULL);
    }

    // Determine the global maximum
    int global_max = local_maxes[0];
    for (int i = 1; i < 10; i++) {
        if (local_maxes[i] > global_max) {
            global_max = local_maxes[i];
        }
    }

    printf("The biggest element in the array is %d\n", global_max);

    // Eliminate the shared memory area
    shmdt(local_maxes);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;

    */
    return 0;
}