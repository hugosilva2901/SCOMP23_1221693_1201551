#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
 int a=0, b, c, d;
 b = (int) fork();
 c = (int) getpid(); /* getpid(), getppid(): unistd.h*/
 d = (int) getppid();
 a = a + 5;
 printf("\na=%d, b=%d, c=%d, d=%d\n",a,b,c,d);
} 

/*
a) Which of the variables a, b, c and d will have the same value in both processes?
    Answer: Apenas a variável a vai ter o mesmo valor nos dois processos.

b) Draw a process tree that describes the processes created.
    

    */
