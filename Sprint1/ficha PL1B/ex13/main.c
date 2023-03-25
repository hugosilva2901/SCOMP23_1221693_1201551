#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void sig_handler(int signl) {
    if (signl == SIGUSR1) {
        printf("Task A: done!\n");
    }
}

int main() {
    pid_t pid;
    int status;

    signal(SIGUSR1, sig_handler);

    pid = fork();

    if (pid == -1) {
        perror("erro no fork");
        exit(1);
    } else if (pid == 0) {
        // processo filho
        srand(time(NULL));
        int sleep_time = rand() % 5 + 1; // segundos entre 1 e 5

        printf("Filho: inicio taskb\n");
        sleep(sleep_time);
        printf("Task B: done!\n");

        printf("Filho: esperar pelo SIGUSR1\n");
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1); // adicionar o sinal SIGUSR1 à máscara
        int sig;
        sigwait(&mask, &sig); // esperar pelo sinal SIGUSR1

        printf("Filho: Inicio da task c\n");
        sleep(1);
        printf("Task C: done!\n");

        exit(0);
    } else {
        printf("Pai: Inicio task A\n");
        sleep(3);
        kill(pid, SIGUSR1); // enviar sinal SIGUSR1 ao processo filho 

        waitpid(pid, &status, 0); // esperar pelo processo filho terminar
        printf("Job is complete!\n");
    }

    return 0;
}
