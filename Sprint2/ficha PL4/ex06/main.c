#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


/*Write a program that creates a new process. The father and the child should indefinitely write the
letters ’S’ and ’C’, respectively:
Father process Child process
while TRUE do: while TRUE do:
print ”S” print ”C”
Implement a policy based on semaphores such that at any moment the number of ’S’ or ’C’ differs by
at most two. The solution should output strings such as: “SCCSSSCCSCSCCC”.
Note: Use fflush(stdout) after every printf() so the text is not buffered and printed immediately.*/


/*Pattern: Temos um semaforo inicializado a 1, para acesso exclusivo.
            Temos um semaforo inicializado a 0, para sincronizacao.
            
Pseudocode:
        Codigo para o pai
        while true
            espera que o filho escreva
            se a diferenca entre Ss e Cs for menor ou igual a 2
                escreve S
            senao
                espera que o filho escreva

        Codigo para o filho
        while true
            espera que o pai escreva
            se a diferenca entre Ss e Cs for menor ou igual a 2
                escreve C
            senao
                espera que o pai escreva



 */

int main() {
    sem_t *sem_parent, *sem_child;
    int s_count = 0, c_count = 0;
    sem_parent = sem_open("/sem_parent", O_CREAT, 0644, 1);
    sem_child = sem_open("/sem_child", O_CREAT, 0644, 0);
    pid_t pid = fork();
    if (pid == 0) { 
        while (1) {
            sem_wait(sem_child); // espera que o filho escreva
            if (s_count - c_count <= 2) { // se a diferenca entre Ss e Cs for menor ou igual a 2
                printf("C");
                c_count++;
            } // caso a diferenca for maior que 2. significa que o pai escreveu 2 Ss seguidos e portanto esperamos que o filho escreva 
            fflush(stdout);
            sem_post(sem_parent); // avisa o pai que pode escrever

        }
    } else {
        while (1) {
            sem_wait(sem_parent); // espera que o pai escreva
            if (c_count - s_count <= 2) { // se a diferenca entre Ss e Cs for menor ou igual a 2
                printf("S");
                s_count++;
            }
            fflush(stdout);
            sem_post(sem_child); // avisa o filho que pode escrever
        }
    }

    int r;
    r = sem_close(sem_parent);
    if ( r == -1) {
        perror("sem_close");
        return 1;
    }
     r = sem_close(sem_child);
    if ( r == -1) {
        perror("sem_close");
        return 1;
    }
    
    r =        sem_unlink("/sem_parent");
    if ( r == -1) {
        perror("sem_unlink");
        return 1;
    }
    r + sem_unlink("/sem_child");
    if ( r == -1) {
        perror("sem_unlink");
        return 1;
    }


    return 0;
}
