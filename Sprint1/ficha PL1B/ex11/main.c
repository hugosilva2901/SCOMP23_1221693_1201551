#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sinais_bloquados() {
    sigset_t blocked_signals;
    sigfillset(&blocked_signals); // set all signals to be blocked initially
    sigprocmask(SIG_BLOCK, NULL, &blocked_signals); // block all signals
    printf("Sinais bloqueados quando recebe SIGUSR1:\n");
    for (int i = 1; i <= NSIG; i++) {
        if (sigismember(&blocked_signals, i)) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void handler(int sig) {
    printf("Recebido o SIGUSR1\n");
    sinais_bloquados();
}

int main() {
    pid_t pid = getpid();
    printf("PID do processo: %d \n", pid);
    signal(SIGUSR1, handler); // registar o handler para o sinal SIGUSR1
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1); // remover o sinal SIGUSR1 da máscara
    sigprocmask(SIG_SETMASK, &mask, NULL); // definir a máscara
    printf("Enviar SIGUSR1...\n");
    while(1) {
        sleep(1);
    }
    return 0;
}


