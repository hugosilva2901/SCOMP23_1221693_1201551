#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define SHM_SIZE 10 * sizeof(int) 

int main() {
    pid_t pid;
    int fd, *data;
    int sum = 0;
    int r;
    int i, j;

    fd = shm_open("/ex04", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SHM_SIZE);

    data = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pid = fork(); 

    if (pid < 0) {
        perror("error fork\n");
        return 1;
    }

    if (pid == 0) {

        for (j = 0; j < 5; j++) {
            while (data[0] == 0);

            printf("Numbers received:\n");
            for (i = 0; i < 10; i++) {
                printf("%d\n", data[i]);
                sum += data[i];
            }
            float avg = (float) sum / 10;
            printf("Average: %.2f\n", avg);

            sum = 0;

            
            data[0] = 0;
        }

        r = munmap(data, SHM_SIZE);
        if (r < 0) {
            perror("error munmap\n");
            exit(1);
        }
        
        r = close(fd);
        if (r < 0) {
            perror("error close\n");
            exit(1);
        }

        exit(EXIT_SUCCESS);
    } else {
        srand(time(NULL));
        for (j = 0; j < 5; j++) {
            
            for (i = 0; i < 10; i++) {
                data[i] = rand() % 20 + 1;
               
            }

            data[0] = 1;

            while (data[0] == 1);
        }

        r = munmap(data, SHM_SIZE);
        if (r < 0) {
            perror("error munmap\n");
            exit(1);
        }

        r = close(fd);
        if (r < 0) {
            perror("error close\n");
            exit(1);
        }

        r = shm_unlink("/ex04");
        if (r < 0) {
            perror("error shm_unlink\n");
            exit(1);
        }

        exit(EXIT_SUCCESS);
    }
}