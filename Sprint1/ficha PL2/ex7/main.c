#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TAMANHO_ARRAY 1000 // cada array tem 1000 elementos
#define TAMANHO_INTERVALO 200 // cada filho/pipe e responsavel por 200 elementos

int main() {
    int vec1[TAMANHO_ARRAY];
    int vec2[TAMANHO_ARRAY];
    int resultado[TAMANHO_ARRAY];
    int child_pipes[5][2]; // 5 pipes, child_pipes[i][0] = leitura, child_pipes[i][1] = escrita
    int i, j;
    int jaImpresso = 0;

    // preencher os arrays com valores aleatorios, com valores entre 0 e 10
    for (i = 0; i < TAMANHO_ARRAY; i++) {
        vec1[i] = rand() % 10;
        vec2[i] = rand() % 10;
    }

    // criar os 5 pipes
    for (i = 0; i < 5; i++) {
        if (pipe(child_pipes[i]) < 0) {
            perror("Erro ao cirar os pipes");
            return 1;
        }
    }

    // tentar criar 5 processos filhos
    for (i = 0; i < 5; i++) {
        pid_t pid = fork(); 
        if (pid < 0) {
            perror("Erro ao fazer fork");
            return 1;
        } 
        else if (pid == 0) { // processo filho
            int start = i * TAMANHO_INTERVALO; // indice inicial do array 
            int end = start + TAMANHO_INTERVALO; // indice final do array

            close(child_pipes[i][0]); // fechar o lado de leitura do pipe

            // somar os elementos dos arrays
            for (j = start; j < end; j++) {
                int sum = vec1[j] + vec2[j];
                write(child_pipes[i][1], &sum, sizeof(int)); // escrever o resultadoado no pipe
            }

            close(child_pipes[i][1]); // fecha o lado de escrita do pipe
            exit(0);
        }
    }

    // Processo pai

    for (i = 0; i < 5; i++) { // fechar o lado de escrita dos pipes
        close(child_pipes[i][1]); 
    }

    int somaFilho;
    // ler os resultadoados dos pipes
    for (i = 0; i < TAMANHO_ARRAY; i++) {
        int indexFilho = i / TAMANHO_INTERVALO; // indice do pipe
        read(child_pipes[indexFilho][0], &somaFilho, sizeof(int)); // ler o resultadoado do pipe
        resultado[i] = somaFilho;
    }

    // fechar o lado de leitura dos pipes
    for (i = 0; i < 5; i++) {
        close(child_pipes[i][0]); // close read end of pipe
    }

    // imprimir os 10 primeiros e os 10 ultimos elementos do array resultado
    printf("resultado:\n");
    for (i = 0; i < 10; i++) {
        printf("%d ", resultado[i]);
    }
    printf("... ");
    for (i = TAMANHO_ARRAY - 10; i < TAMANHO_ARRAY; i++) {
        printf("%d ", resultado[i]);
    }
    printf("\n");

    return 0;
}
