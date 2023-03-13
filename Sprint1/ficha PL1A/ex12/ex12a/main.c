#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int spawn_childs(int n) {
    int i, indice;
    pid_t pid;
    int status, filho;

    // criar n processos filhos
    for (i = 1; i <= n; i++) {
        pid = fork();
        if (pid < 0) { 
            exit(1);
        } else if (pid == 0) { // sucesso ao criar processo filho
            indice = i; // guardar o indice do processo filho
            exit(indice);
        }
    }

    if (pid != 0) {
        //por cada processo filho
        for (i = 1; i <= n; i++) {
            filho = wait(&status); // esperar que termine
            printf("filho %d: %d\n", filho, WEXITSTATUS(status)); 
        }
        return 0; 
    }
}



int main(){
    int n = 5;
    int indice = spawn_childs(n);
    printf("To the parent process: %d \n", indice);
    return 0;
    
}