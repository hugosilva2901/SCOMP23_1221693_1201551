#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSG_SIZE 13 // Hello World e a maior mensagem, 12 caracteres + \0

int main() {
    int p[2]; // p[0] = leitura, p[1] = escrita
    pid_t pid; 
    char mensagem1[] = "Hello World";
    char mensagem2[] = "Goodbye!";
    char buffer[MSG_SIZE]; // buffer para ler do pipe

    printf("Processo pai: pid = %d \n", getpid());

    // tentar criar um pipe
    if (pipe(p) < 0) {
        perror("Falha ao criar o pipe");
        return 1;
    }

    // tentar criar um processo filho
    pid = fork();
    if (pid < 0) { // fork falhou
        perror("Falha ao fazer fork");
    } 
    else if (pid == 0) { // processo filho
        close(p[1]); // fecha o lado de escrita do pipe

        // Enquanto houver mensagens para ler, ler
        while (read(p[0], buffer, MSG_SIZE) > 0) {
            printf("Processo filho recebeu: %s\n", buffer);
        }

        close(p[0]); // fecha o lado de leitura do pipe
        exit(14);
    } else if(pid > 0) { // processo pai
        close(p[0]); // fecha o lado de leitura do pipe

        // escrever mensagens no pipe
        write(p[1], mensagem1, MSG_SIZE);
        write(p[1], mensagem2, MSG_SIZE);

        close(p[1]); // fecha o lado de escrita do pipe

        // esperar que o processo filho termine
        int status;
        waitpid(pid, &status, 0);
        printf("Processo filho com pid %d terminou com status %d\n", pid, status);
    }

    return 0;
}
