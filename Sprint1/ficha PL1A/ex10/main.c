#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

const int TAMANHO_ARRAY = 2000;
const int NUM_PROCES_FILHOS = 10;

int main() {
	
    int array_aleatorios [TAMANHO_ARRAY];
    int n;
    time_t t;
    srand((unsigned) time(&t));

    int i;
    for (i = 0; i < TAMANHO_ARRAY; i++) {
        array_aleatorios[i] = rand() % 100;
    }

    printf("N (0-99): ");
    scanf("%d", &n);

    int existe = 0;
    for (i = 0; i < NUM_PROCES_FILHOS; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            printf("Falha ao criar processo filho\n");
            exit(1);
        }
        else if (pid == 0) {
            // intervalo de procura para cada processo filho
            int inicio = i * 100;
            int fim = (i + 1) * 100;
            int j;
            for (j = inicio; j < fim; j++) {
                if (array_aleatorios[j] == n) {
                    printf("O número %d foi encontrado na posição %d\n", n, j);
                    exit(0);
                }
            }
            exit(255);
        }
    }

    // espera pelos processos filhos e verifica seus resultados
    for (i = 0; i < NUM_PROCES_FILHOS; i++) {
        int status;
        pid_t pid = wait(&status);
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 255) {
                existe = 1;
            }
        }
    }
    if (!existe) {
        printf("O número %d não foi encontrado no array\n", n);
    }

    return 0;
}
