#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>

#define SHM_SIZE sizeof(Student) // Definimos o tamanho da memória partilhada como o tamanho da estrutura Student

typedef struct {
    int number; // Número do aluno
    char name[256]; // Nome do aluno
    char address[256]; // Morada do aluno
} Student;

int main() {
    int fd = shm_open("/ex01", O_CREAT | O_RDWR, 0666); // Temos um file descriptor que aponta para a memória partilhada, se não existir é criada, se existir é aberta para leitura e escrita
    /*  shm_open(const char *name, int oflag, mode_t mode);
        name: nome da memória partilhada
        oflag: O_CREAT para criar a memória partilhada se não existir, O_RDWR para abrir a memória partilhada para leitura e escrita
        mode: permissões da memória partilhada
    */
    
    ftruncate(fd, SHM_SIZE); // Definimos o tamanho da memória partilhada como o tamanho da estrutura Student
    /* ftruncate(int fd, off_t length);
        fd: file descriptor da memória partilhada
        length: tamanho da memória partilhada
    */

    Student *student = (Student *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);  // Mapeamos a memória partilhada para a estrutura Student
    //mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    /* addr: NULL para que o sistema escolha o endereço de memória
       length: tamanho da memória partilhada
       prot: PROT_READ para que possamos ler a memória partilhada, PROT_WRITE para que possamos escrever na memória partilhada
       flags: MAP_SHARED para que a memória partilhada seja partilhada entre processos
       fd: file descriptor da memória partilhada
       offset: 0 para que o mapeamento comece no início da memória partilhada
    */

    printf("Numero do Estudante: ");
    scanf("%d", &student->number); // Lemos o número do aluno
    printf("Nome do Estudante: ");
    scanf(" %[^\n]s", student->name); // Lemos o nome do aluno, %[^\n]s serve para ler strings com espaços
    printf("Morada do Estudante: ");
    scanf(" %[^\n]s", student->address); // Lemos a morada do aluno, %[^\n]s serve para ler strings com espaços

    munmap(student, SHM_SIZE);  // Desmapeamos a memória partilhada
    close(fd); // Fechamos o file descriptor

    return 0;
}
