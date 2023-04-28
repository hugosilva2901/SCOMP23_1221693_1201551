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
   
    return 0;
}
