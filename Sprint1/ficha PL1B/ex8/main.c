#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
 void handle_USR1 (int signo, siginfo_t *sinfo, void *context) {
 char msg [80];
 sprintf ( msg , " Here is a number : %d\n", signo) ;
 write ( STDOUT_FILENO , msg , strlen ( msg ) ) ;
 }

int main(void){
    printf("%d",getpid());
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_sigaction = handle_USR1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    slep(1000);
    return 0;
}