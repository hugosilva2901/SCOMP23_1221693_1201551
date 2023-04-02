#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> // islower, isupper, tolower, toupper

#define MAX_mensgaemn_SIZE 256 // tamanho maximo da mensagem sao 256 bytes

int main() {
    int up_pipe[2], down_pipe[2]; // pipe para cima (client -> servidor) e pipe down(servidor -> client)
    
    if (pipe(up_pipe) == -1 || pipe(down_pipe) == -1) { // Tentar criar os pipes
        perror("Erro ao criar os pipes");
        return 1;
    }

    // Tentar criar um processo filho
    pid_t pid = fork();
    if (pid == -1) {
        perror("Erro ao fazer fork");
        return 1;
    }

    if (pid == 0) { // proceresso filho (cliente)
        close(up_pipe[0]); // Fechar a leitura do pipe up
        close(down_pipe[1]); // fecha a escrita do pipe down

        printf("Mensagem a enviar pelo up pipe: ");
        char mensgaemn[MAX_mensgaemn_SIZE];
        fgets(mensgaemn, MAX_mensgaemn_SIZE, stdin); 
        int mensagem_tamanho = strlen(mensgaemn);

        // enviar mensagem para o servidor
        write(up_pipe[1], mensgaemn, mensagem_tamanho);

        // esperar pela resposta do servidor
        char resposta[MAX_mensgaemn_SIZE];
        int resposta_len = read(down_pipe[0], resposta, MAX_mensgaemn_SIZE);
        resposta[resposta_len] = '\0'; // adicionar o caracter de terminacao

        printf("Resposta do servidor: %s\n", resposta);

        close(up_pipe[1]); // fecha a escrita do pipe up
        close(down_pipe[0]); // fecha a leitura do pipe down
    } 
    else { // processo pai (servidor)
        close(up_pipe[1]); // fecha a escrita do pipe up
        close(down_pipe[0]); // fecha a leitura do pipe down

        // Ler mensagem do cliente
        char mensgaemn[MAX_mensgaemn_SIZE];
        int mensagem_tamanho = read(up_pipe[0], mensgaemn, MAX_mensgaemn_SIZE);
        mensgaemn[mensagem_tamanho] = '\0';

        // Converter a mensagem para maiusculas ou minusculas
        for (int i = 0; i < mensagem_tamanho; i++) {
            if (islower(mensgaemn[i])) {
                    mensgaemn[i] = toupper(mensgaemn[i]);
            } else if (isupper(mensgaemn[i])) {
                    mensgaemn[i] = tolower(mensgaemn[i]);
                }
        }

        // envia a mensagem convertida para o cliente
        write(down_pipe[1], mensgaemn, mensagem_tamanho);
        close(up_pipe[0]); // fecha a leitura do pipe up
        close(down_pipe[1]); // fecha a escrita do pipe down
    }

    return 0;
}
