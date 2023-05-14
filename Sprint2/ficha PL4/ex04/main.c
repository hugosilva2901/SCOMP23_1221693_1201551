#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

int main() {
    sem_t *sem1, *sem2;
    pid_t pid1, pid2;
    int f;
    sem1 = sem_open("/sem1", O_CREAT, 0644, 0);
    sem2 = sem_open("/sem2", O_CREAT, 0644, 0); 
    pid1 = fork();
    if (pid1 == 0) { // filho 1
        printf("1st child\n"); // imprime "1st child"
        sem_post(sem1); // liberta o semaforo 1
        exit(0); // termina o processo
    } else {
        pid2 = fork(); // cria o filho 2
        if (pid2 == 0) {
            sem_wait(sem2); // espera pelo semaforo 2
            printf("2nd child\n"); // imprime "2nd child"
            exit(0);
        } else { // pai
            sem_wait(sem1); // Esperar que o semaforo 1 seja libertado
            printf("Father\n"); // Quando o semaforo 1 for libertado, imprime "Father"
            sem_post(sem2); // Depois de imprimir "Father", liberta o semaforo 2 para que o filho 2 possa imprimir "2nd child"
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
            f = sem_unlink("/sem1");
            if(f == -1) perror("sem_unlink sem1");
            f = sem_unlink("/sem2");
            if(f == -1) perror("sem_unlink sem2");
            exit(0);
        }
    }
    return 0;
}
