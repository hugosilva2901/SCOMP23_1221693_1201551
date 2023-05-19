#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <string.h>

#define CAPACIDADE_BUFFER_CIRCULAR 10
#define NUMERO_VALORES 30
#define PRODUCER_SLEEP 1
#define CONSUMER_SLEEP 2
 /*Implement a program that creates two new processes. The child processes will have the role of
producer, while the father will act as a consumer.
In a shared memory area, there is a circular buffer with a capacity for 10 integers.
The producer writes 30 increasing values into the buffer, with a interval of 1s and prints them on the screen.
The consumer read values from the buffer every 2 seconds and prints them on screen.
Write a program that performs as described, following this set of functional restrictions:
(a) a producer blocks if the buffer is full;
(b) a producer only writes after guaranteeing mutual exclusion;
(c) a consumer blocks while there is no new data available*/

/*Pattern:
    Temos um semaforo sem_buffer_cheio que vai ser inicializado a 0
    Temos um semaforo sem_buffer_vazio que vai ser inicializado a 10
    Temos um semaforo sem_aceder_buffer que vai ser inicializado a 1

    Temos mutual exclusion porque o semaforo sem_aceder_buffer vai bloquear o acesso ao buffer
    Temos também controlling access to resources with a limited capacity porque os semaforos sem_buffer_cheio e sem_buffer_vazio vão controlar o acesso ao buffer



    Pseudo-Code:

    Para o pai (consumidor)
        enquanto o pai não tiver lido 30 valores
            esperar  pelo semaforo sem_buffer_cheio
            esperar  pelo semaforo sem_aceder_buffer

            ler valor do buffer
            imprimir valor

            libertar o semaforo sem_aceder_buffer
            libertar o semaforo sem_buffer_vazio

            dormir 2 segundos


     Para o filho(produtor)
        enquanto o filho não tiver escrito 30 valores
            esperar  pelo semaforo sem_buffer_vazio
            esperar  pelo semaforo sem_aceder_buffer

            escrever valor no buffer
            imprimir valor

            libertar o semaforo sem_aceder_buffer
            libertar o semaforo sem_buffer_cheio

            dormir 1 segundo

*/
 
typedef struct{
        int buffer[CAPACIDADE_BUFFER_CIRCULAR];
      int head;
    int tail;

}buffer_circular;


 int main(void) {
    int fd, i, valor;
    pid_t pid;
    buffer_circular *b;
    sem_t *sem_buffer_cheio, *sem_buffer_vazio, *sem_aceder_buffer;
    int aux;
    fd = shm_open("/ex12", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1){
        perror("shm_open\n");
        return 1;
    }
    aux = ftruncate(fd, sizeof(buffer_circular));
    if(aux == -1){
        perror("ftruncate\n");
        return 1;
    }
    b = (buffer_circular *) mmap(NULL, sizeof(buffer_circular), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(b == NULL){
        perror("mmap\n");
        return 1;
    }
    sem_unlink("/sem_buffer_cheio"); 
    sem_unlink("/sem_buffer_vazio");
    sem_unlink("/sem_aceder_buffer");

    sem_buffer_cheio = sem_open("/sem_buffer_cheio", O_CREAT | O_EXCL, 0644, 0);
    if(sem_buffer_cheio == SEM_FAILED){
        perror("Erro no sem_open\n");
        return 1;
    }
    sem_buffer_vazio = sem_open("/sem_buffer_vazio", O_CREAT | O_EXCL, 0644, CAPACIDADE_BUFFER_CIRCULAR);
    if(sem_buffer_vazio == SEM_FAILED){
        perror("Erro no sem_open\n");
        return 1;
    }

    sem_aceder_buffer = sem_open("/sem_aceder_buffer", O_CREAT | O_EXCL, 0644, 1);
    if(sem_aceder_buffer == SEM_FAILED){
        perror("Erro no sem_open\n");
        return 1;
    }

    pid = fork();
    if(pid == -1){
        perror("Erro no fork\n");
        return 1;
    }
    if(pid == 0){ 
        // filho
        for(i = 0; i < NUMERO_VALORES; i++){ // 30 valores
            sem_wait(sem_buffer_vazio); // decrementa o semaforo do buffer vazio
            sem_wait(sem_aceder_buffer); // decrementa o semaforo do acesso ao buffer
            printf("A escrever no buffer: %d\n", i);
            b->buffer[b->head] = i; // escreve no buffer
            b->head = (b->head + 1) % CAPACIDADE_BUFFER_CIRCULAR; // incrementa o head
            sem_post(sem_aceder_buffer); // incrementa o semaforo do acesso ao buffer
            sem_post(sem_buffer_cheio); // incrementa o semaforo do buffer cheio
            sleep(PRODUCER_SLEEP); // dorme 1 segundo
        }
    
        // espera que o pai termine
        wait(NULL);
        sem_unlink("/sem_buffer_cheio"); // remove o semaforo
        sem_unlink("/sem_buffer_vazio"); 
        sem_unlink("/sem_aceder_buffer");
        int t = munmap(b, sizeof(buffer_circular)); // remove o mapeamento
        if(t == -1){
            perror("Erro no munmap\n");
            return 1;
        }
        
        t = close(fd); // fecha o descritor
        if(t == -1){
            perror("Erro no close\n");
            return 1;
        }
        shm_unlink("/ex12"); // remove a memoria partilhada

    }else{
        // pai
        for(i = 0; i < NUMERO_VALORES; i++){ // 30 valores
            sem_wait(sem_buffer_cheio); // decrementa o semaforo do buffer cheio
            sem_wait(sem_aceder_buffer); // decrementa o semaforo do acesso ao buffer
            valor = b->buffer[b->tail]; // le do buffer
            b->tail = (b->tail + 1) % CAPACIDADE_BUFFER_CIRCULAR; // incrementa o tail
            sem_post(sem_aceder_buffer); // incrementa o semaforo do acesso ao buffer
            sem_post(sem_buffer_vazio); // incrementa o semaforo do buffer vazio
            printf("A ler do buffer: %d\n", valor); // imprime o valor
            sleep(CONSUMER_SLEEP); // dorme 2 segundos
        }
    }

    return 0;



}