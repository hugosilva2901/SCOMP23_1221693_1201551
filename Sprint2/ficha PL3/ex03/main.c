#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define SHM_SIZE 10 * sizeof(int) // Para 10 inteiros

int main() {
    pid_t pid;
    int fd, *data;
    int sum = 0;
    int r;

    // Temos um file descriptor para a shared memory
    fd = shm_open("/ex02", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SHM_SIZE);

    // Temos um apontador *data, que vai ser usado para aceder à shared memory
    data = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pid = fork(); // para criar um processo filho

    if (pid < 0) {
        perror("erro fork\n");
        return 1;
    }

    if (pid == 0) {
        // processo filho, age como reader

        // aguarda que o writer escreva na shared memory, pode
        while (data[0] == 0);

        // Vamos ler os dados da shared memory
        printf("Numeros recebidos:\n");
        for (int i = 0; i < 10; i++) {
            printf("%d\n", data[i]);
            sum += data[i];
        }

        // Calculo da media
        float avg = (float) sum / 10;
        printf("Media: %.2f\n", avg);

        // Unmap shared memory object
        r = munmap(data, SHM_SIZE);
        if (r < 0) {
            perror("erro munmap\n");
            exit(1);
        }
        
        r = close(fd);
        if (r < 0) {
            perror("erro close\n");
            exit(1);
        }

        exit(0);
    } else {
        // processo parent, age como writer

        // Gerar 10 numeros aleatorios e escrever na shared memory
        printf("Inteiros gerados:\n");
        srand(time(NULL));
        for (int i = 0; i < 10; i++) {
            data[i] = rand() % 20 + 1;
            printf("%d\n", data[i]);
        }

        // Aguradar que o reader leia os dados da shared memory
        wait(NULL);

        // Desmapear a shared memory
        r = munmap(data, SHM_SIZE);
        if (r < 0) {
            perror("erro munmap\n");
            exit(1);
        }

        r = close(fd);
        if (r < 0) {
            perror("erro close\n");
            exit(1);
        }

        r = shm_unlink("/ex02");
        if (r < 0) {
            perror("erro shm_unlink\n");
            exit(1);
        }

        exit(0);
    }
}
