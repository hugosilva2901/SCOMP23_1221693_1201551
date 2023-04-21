#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define SHM_SIZE sizeof(int) * 10 // 10 inteiros

int main() {
    int r;
    int fd = shm_open("/ex02", O_RDWR, 0666);

       
    int *shared_numbers = (int *) mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);

    // Serve para esperar que o writer escreva na shared memory
    while (shared_numbers[0] == 0) {
        sleep(1);
    }

    // media
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += shared_numbers[i];
    }
    double average = (double) sum / 10;

    // Imprimir os números recebidos e a média calculada
    printf("Números recebidos: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", shared_numbers[i]);
    }
    printf("\n");
    printf("Média: %.2f\n", average);

    r = munmap(shared_numbers, SHM_SIZE);
    if (r < 0) {
        perror("erro munmap\n");
        exit(1);
    }

    r = close(fd);
    if (r < 0) {
        perror("erro close\n");
        exit(1);
    }

    return 0;
}
