#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main()
{
 pid_t p;
 
 if (fork() == 0) {
    printf("PID = %d\n", getpid()); exit(0);
 }
 
 if ((p=fork()) == 0) {
    printf("PID = %d\n", getpid()); exit(0);
 } 

 printf("Parent PID = %d\n", getpid());
 
 printf("Waiting... (for PID=%d)\n",p); 
 waitpid(p, NULL, 0);
 
 printf("Enter Loop...\n"); 
 while (1); /* Infinite loop */
}

// In the shell, list the running processes with ps (look for the PIDs printed). Are all parent and child processes
// listed? Why?
//     Resposta: Nao, so os processos que ainda nao foram tratados e que estao a correr sao listados.
//      Aparece um processo filho como "defunct" (zombie).
//      O segundo processo filho nao aparece, mesmo apesar de ter sido feito exit(0), porque temos o waitpid() no pai.

// Is there anything particular about the child process? Explain.
//    Resposta:  Sim, aparece como defunt ou seja processo zombie.
// Um processo zombie acontece quando um processo filho termina mas o processo pai nao esta a espera dele.
// Isto faz com que o SO mantenha os recursos, pid e outros dados do processo filho mesmo depois de este ter terminado.

// Kill the parent process. 

 // kill -KILL xxxx

