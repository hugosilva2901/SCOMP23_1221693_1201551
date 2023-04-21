#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SHM_SIZE sizeof(int) * 10 // Para os 10 inteiros

int main() {
    // Gerar 10 números aleatórios entre 1 e 20
    int numbers[10];
    int r;
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        numbers[i] = rand() % 20 + 1;
    }

    // Criar a área de memória partilhada
    int fd = shm_open("/ex02", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SHM_SIZE);

    // Mapear a área de memória partilhada
    int *shared_numbers = (int *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Escrever os números na área de memória partilhada
    for (int i = 0; i < 10; i++) {
        shared_numbers[i] = numbers[i];
    }

    // Desmapear a área de memória partilhada
   r =  munmap(shared_numbers, SHM_SIZE);
    if (r < 0) {
        perror("erro munmap\n");
        exit(1);
    }
    // Fechar o file descriptor
    r = close(fd);
    if (r < 0) {
        perror("erro close\n");
        exit(1);
    }
    
    // Imprimir os números gerados
    printf("Números gerados: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    return 0;
}
