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
    Answer: The variable a will have the same value in both processes
            The variable b will have the same value on the parent process as the variable c on the child process
            The variable c on the parent process does not have the same value as the variable d on the child process 

b) Draw a process tree that describes the processes created.
    

    */
