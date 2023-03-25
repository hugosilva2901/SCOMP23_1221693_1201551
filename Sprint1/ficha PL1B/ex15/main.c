#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// estrutura proveniente do enunciado
typedef struct {
    char cmd[32];
    int time_cap;
} command_t;

void executar_commando(command_t cmd) {
    pid_t pid;
    int status;
    int return_execlp;
    pid = fork();
    if (pid < 0) {
        printf("Erro ao criar processo filho \n");
        exit(1);
    } else if (pid == 0) {
        printf("A executar o comando '%s'...\n", cmd.cmd);
        return_execlp = execlp(cmd.cmd, cmd.cmd, NULL);
        printf("Erro no execlp: %s\n", strerror(return_execlp)); //strerror retorna a string associada ao erro
        exit(1);
    } else {
        sleep(cmd.time_cap);
         // aguardar o processo filho terminar
        waitpid(pid, &status, 0);
        // se o processo filho terminou com sucesso e o execpl retornou 0
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Comando '%s' executado com sucesso.\n", cmd.cmd);
        } else {
            printf("Erro ao executar o comando '%s'.\n", cmd.cmd);
        }
    }
}


int main() {
    command_t commandos[] = { {"ls", 1}, {"ps", 1}, {"pwd", 1}, {"date", 1}, {"who", 1},{"echo", 1}};
    int num_commandos = sizeof(commandos) / sizeof(command_t);
    for (int i = 0; i < num_commandos; i++) {
        executar_commando(commandos[i]);
    }
    printf("Lista de comandos percorrida.\n");
    return 0;
}
