#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main()
{
    int numeroSinal;
    pid_t pid_processo;
    char resposta;

    do {
        printf("Introduza o pid do processo: ");
        scanf("%d", &pid_processo);

        if(kill(pid_processo, 0) == -1){
            printf("O processo nao existe ou o utilizador nao tem permissao para enviar sinais.\n");
            continue;
        }else{
            printf("Introduza o numero do sinal: ");
            scanf("%d", &numeroSinal);
            if(numeroSinal < 1 || numeroSinal > 64){
                printf("O numero do sinal tem de estar entre 1 e 64.\n");
                continue;
            }else{
                if(kill(pid_processo, numeroSinal) == -1){
                    printf("O sinal nao foi enviado.\n");
                    continue;
                }else{
                    printf("O sinal foi enviado com sucesso.\n");
                }
            }
        }

        printf("Deseja continuar? (S/N) ");
        scanf(" %c", &resposta);
    } while (resposta == 'S' || resposta == 's');

    return 0;
}
