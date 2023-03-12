#include <stdio.h>
#include <unistd.h>	
#include <stdlib.h>
#include <sys/wait.h>

int main () 
{
    int i;
    int status;
    pid_t pid;

    for (i = 0; i < 4; i++)
    {
        if (fork () == 0) {
            sleep (1); /*sleep(): unistd.h */
            exit(0); // para criar apenas 4 processos filhos, from b)
        }
    }

    for(i = 0; i < 4; i++) {
       pid = waitpid(-1, NULL, 0); // espera que o processo filho termine, igual a wait(NULL)
       if (pid % 2 == 0) {
              printf("Processo com PID %d terminou.\n", pid);
       }
       
    }
    printf ("This is the end.\n");
}

// Assuming the changes in b), change the code so that the parent process waits for child processes with an
// even PID.