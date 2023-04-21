#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define SHM_SIZE sizeof(Student)

typedef struct {
    int number;
    char name[256];
    char address[256];
} Student;

int main() {
    int fd = shm_open("/ex01", O_RDONLY, 0666); 
    /* shm_open(const char *name, int oflag, mode_t mode);
        name: nome da memória partilhada
        oflag: O_RDONLY para abrir a memória partilhada para leitura
        mode: permissões da memória partilhada
    */
    ftruncate(fd, SHM_SIZE); 
    /* ftruncate(int fd, off_t length);
        fd: file descriptor da memória partilhada
        length: tamanho da memória partilhada
    */

    Student *student = (Student *) mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0); 

    /*  Este loop serve para que o processo leitor espere que o processo escritor escreva na memória partilhada
        O processo leitor vai verificar se o número do aluno, o nome e a morada estão vazios, se estiverem vazios o processo leitor vai dormir durante 1 segundo e depois volta a verificar se o número do aluno, o nome e a morada estão vazios
        Quando o processo escritor escrever na memória partilhada o processo leitor vai sair do loop e vai imprimir o número do aluno, o nome e a morada 
    */
    while (student->number == 0 && strcmp(student->name, "") == 0 && strcmp(student->address, "") == 0) {
        sleep(1);
    }

    printf("Numero estudnate: %d\n", student->number);
    printf("Nome Estudante: %s\n", student->name);
    printf("Morada do Estudante: %s\n", student->address);

    munmap(student, SHM_SIZE); 
    close(fd); 

    return 0;
}
