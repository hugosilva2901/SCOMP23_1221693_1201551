#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void timeout_handler(int sig) {
    printf("You were too slow and this program will end!\n");
    exit(0);
}

int main() {
    char str[100];
    signal(SIGALRM, timeout_handler);
    alarm(10);
    printf("Enter a string: ");
    fgets(str, 100, stdin);
    alarm(0);
    int a= strlen(str)-1;
    printf("The size of the string is: %d\n", a);
    return 0;
}