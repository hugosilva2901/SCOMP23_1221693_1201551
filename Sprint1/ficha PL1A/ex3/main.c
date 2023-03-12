#include <stdio.h>
#include <stdint.h> 
#include <sys/types.h>
#include <unistd.h>
int main(void) {
 fork(); fork(); fork();
 printf("SCOMP!\n");
 } 
/*
3)A) 8 processos
3)B) foto na mesma pasta 
3)c) 8 vezes 
*/