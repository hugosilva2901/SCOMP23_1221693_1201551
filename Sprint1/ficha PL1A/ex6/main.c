#include <stdio.h>
#include <unistd.h>	
#include <stdlib.h>
#include <sys/wait.h>

int main () 
{
    int i;
    int status;

    for (i = 0; i < 4; i++)
    {
        if (fork () == 0) {
            sleep (1); /*sleep(): unistd.h */
        }
    }
    printf ("This is the end.\n");
}

/*
a)How many processes will be created by this code? Justify by drawing a process tree that describes the
processes created.
    Resposta: Vão ser criados 16 processos.

b)What change, if any, would you do to this code so that exactly 4 child processes are created?
    Resposta: Para garantir que apenas 4 processos filhos sao criados, devemos adicionar um exit(0) dentro do if
                Isto garante que cada processo filho, apos a funcao "dormir" por 1 segundo , faca exit  */