#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd[2]; //fd[0] = leitura, fd[1] = escrita
    pid_t pid;

    pipe(fd); // criar o pipe
    pid = fork(); // fazer fork, criar processo filho

    if (pid == 0) { // processo filho
        close(fd[1]); // fecha o lado de escrita do pipe

        pid_t pidPai;
        read(fd[0], &pidPai, sizeof(pid_t)); // ler o PID do processo pai
        printf("Processo filho: pid do pai = %d\n", pidPai);

        close(fd[0]); // fechar o lado de leitura do pipe
    } else if (pid > 0) { // processo pai
        close(fd[0]); // fechar o lado de leitura do pipe

        pid_t pidPai = getpid(); 
        write(fd[1], &pidPai, sizeof(pid_t)); // escrever o PID do processo pai
        printf("Processo Pai: pid = %d\n", pidPai);

        close(fd[1]); // close write end of pipe
        wait(NULL); // wait for child to terminate
    } else { 
        printf("Erro no fork\n");
    }
}
