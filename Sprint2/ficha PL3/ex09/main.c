#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


#define TAM_BUFFER 10
#define NVALORES 30

typedef struct {
    int buffer[TAM_BUFFER];
    int in; /* Próxima posição vazia */
    int out; /* Próxima posição a ser consumida */
    int count; /* Número de elementos no buffer */
} buffer_circular;

int main() {
    int i, j;
    pid_t pid;
    buffer_circular *bc;
    int fd;

    /* Cria o objeto de memória compartilhada */
    fd = shm_open("/bc", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    /* Define o tamanho do objeto de memória compartilhada */
    if (ftruncate(fd, sizeof(buffer_circular)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    /* Mapeia o objeto de memória compartilhada no espaço de endereço do processo */
    bc = mmap(NULL, sizeof(buffer_circular), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (bc == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    /* Inicializa o buffer circular */
    bc->in = 0;
    bc->out = 0;
    bc->count = 0;

    /* Cria processo filho */
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    /* Processo produtor */
    if (pid == 0) {
        for (i = 1; i <= NVALORES; i++) {
            // bc -> count serve para saber se o buffer está cheio
            // se estiver cheio, o produtor espera
            while (bc->count == TAM_BUFFER) {
                /* Espera que seja consumido */
            }

            bc->buffer[bc->in] = i; // insere o valor no buffer
            bc->in = (bc->in + 1) % TAM_BUFFER; // incrementa o in, para que o próximo valor seja inserido no próximo espaço do buffer
            bc->count++; // incrementa o count, para que o consumidor saiba que o buffer não está vazio
        }

        /* Desanexa o segmento de memória compartilhada */
        if (munmap(bc, sizeof(buffer_circular)) == -1) {
            perror("munmap");
            exit(1);
        }

        exit(0);
    }

    /* Processo consumidor */
    if (pid > 0) {
        for (j = 1; j <= NVALORES; j++) {
            // se o buffer está vazio
            // o consumidor espera
            while (bc->count == 0) {
                /* Espera que o produtor meta produtos */
            }

            printf("Valor consumido: %d\n", bc->buffer[bc->out]);
            bc->out = (bc->out + 1) % TAM_BUFFER;
            bc->count--;

        }

        /* Aguarda o processo filho terminar */
        wait(NULL);

        /* Desanexa o segmento de memória compartilhada */
        if (munmap(bc, sizeof(buffer_circular)) == -1) {
            perror("munmap");
            exit(1);
        }

        /* Remove o objeto de memória compartilhada */
        if (shm_unlink("/bc") == -1) {
            perror("shm_unlink");
            exit(1);
        }

        exit(0);
    }
    return 0;
}
