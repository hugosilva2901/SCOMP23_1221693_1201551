#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define PROCESSOS_FILHO 5
#define QUANTIDADE_PIPES (PROCESSOS_FILHO + 1) // pai -> filho1 -> filho2 -> filho3 -> filho4 -> filho5 -> pai

int main(void)
{
    int fd[QUANTIDADE_PIPES][2];
    int numeroAleatorio, maiorNumero = 0;

    // Criamos os pipes
    for (int i = 0; i < QUANTIDADE_PIPES; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("Erro ao criar pupe");
            return 1;
        }
    }

    // criar os processos filhos
    pid_t pid;
    for (int i = 0; i < PROCESSOS_FILHO; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("Erro ao criar oricesso filho");
            return 1;
        }
        if (pid == 0) // processo filho
        {
            srand((unsigned)time(NULL) * getpid()); // para obtermos uma seed diferente para cada processo filho
            
            // ler o numero do pipe
            read(fd[i][0], &numeroAleatorio, sizeof(int));
            printf("pid: %d, numero recebido: %d\n", getpid(), numeroAleatorio);

            // gerar um novo numero aleatorio entre 1 e 500 e comparar com o numero recebido atraves do pipe
            int numeroGerado = rand() % 500 + 1;
            printf("pid: %d, numero gerado: %d\n", getpid(), numeroGerado);
            if (numeroGerado > numeroAleatorio)
            {
                numeroAleatorio = numeroGerado;
            }

            // enviar o maior numero (gerado ou recebido) para o proximo processo
            write(fd[i+1][1], &numeroAleatorio, sizeof(int));
            exit(EXIT_SUCCESS);
        }
    }

    // processo pai
    srand((unsigned)time(NULL) * getpid());  // seed aleatoria
    numeroAleatorio = rand() % 500 + 1;
    printf("pid: %d, numero: %d\n", getpid(), numeroAleatorio);

    // enviar o numero gerado para o primeiro processo filho
    write(fd[0][1], &numeroAleatorio, sizeof(int));

    // Esperar que o ultimo filho envie o maior numero para o pai
    read(fd[QUANTIDADE_PIPES-1][0], &maiorNumero, sizeof(int));
    printf("Maior numero é: %d\n", maiorNumero);

    return 0;
}
