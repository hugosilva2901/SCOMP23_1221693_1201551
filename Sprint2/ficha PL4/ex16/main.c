#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>


/*
//sincronizacao de processos e exclusao mutua

Pseudocodigo
    Processo Px
        Por cada valor no intervalo
            Espera por permissao para escrever
            Calcula a media movel de 10 valores
            Escreve o valor calculado no vetor final
        Se for o ultimo numero do intervalo
            Increment the count of finished processes in dados->processosAcabados    
        
        Se for o ultimo processoPc
            Avisa que atingiu a barreira
        Avisa que pode ler 
    

    Pmax
    Por cada valor no intervalo
    armazena o atual valor mais alto
    espera que um px avise que pode ler
    Econtrar o maior valor no vetor final
    Se houve mudanca no maior valor
        atualiza o valor mais alto
        avisa o user
    avisa o proximo px que pode ler

    Pai
    espera que a barreira seja atingida
    imprime o valor mais alto e os dados




    
    
    */

#define PROCESSES 5
#define QUANT_VALORES_VEC_INICIAL 10000
#define QUANT_VALORES_VEC_FINAL 1000
#define MOVING_AVERAGE_WINDOW 10

typedef struct {
    int vetorInicial[QUANT_VALORES_VEC_INICIAL];
    int vetorFinal[QUANT_VALORES_VEC_FINAL];
    int processosAcabados;
    int highest_value;
} shared_data;

int calcularMediaMovel10(const int* initial_vector, int start_index) {
    int sum = 0;
    for (int i = start_index; i < start_index + MOVING_AVERAGE_WINDOW; i++) {
        sum += initial_vector[i];
    }
    return sum / MOVING_AVERAGE_WINDOW;
}

int main() {
    const char* semafProcessos[PROCESSES] = { "/P1", "/P2", "/P3", "/P4", "/P5" };
    int fd, size = sizeof(shared_data), posicao;
    shared_data* dados;
    pid_t pid;
    srand(time(NULL));
    fd = shm_open("/ex16", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(fd, size) < 0) {
        perror("ftruncate");
        return 1;
    }
    dados = (shared_data*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

   sem_t* semaforosEscrita[PROCESSES]; // Uma lista de semaforos para controlar qual o Px que pode escrever
    sem_t* semaforoLeitura; // semaforo que avisa o pmax que pode ler determinado intervalo no vetor final
    sem_t* barreira; // barreira que avisa o pai que todos os processos acabaram

//inicialziar os semaforos
    for (int i = 0; i < PROCESSES; i++) {
        int option = 0;
        if(i == 0){ // Se for o P1, o semaforo deve ser inicializado a 1 para que seja o primeiro a escrever
            option = 1;
        }else{
            option = 0; // Os restantes devem ser inicializados a 0 para que esperem que o anterior escreva
        }
        semaforosEscrita[i] = sem_open(semafProcessos[i], O_CREAT | O_EXCL, 0644, option);
        if (semaforosEscrita[i] == SEM_FAILED) {
            perror("Error in sem_open()");
            return 1;
        }
    }
    semaforoLeitura = sem_open("/semaforoLeitura", O_CREAT | O_EXCL, 0644, 0);
    if (semaforoLeitura == SEM_FAILED) {
        perror("Error in sem_open()");
        return 1;
    }
    barreira = sem_open("/barreira", O_CREAT | O_EXCL, 0644, 0);
    if (barreira == SEM_FAILED) {
        perror("Error in sem_open()");
        return 1;
    }

//carregar o vetor inicial com os 10 000 valores
    for (int i = 0; i < QUANT_VALORES_VEC_INICIAL; i++) {
        dados->vetorInicial[i] = rand() % 10001 + 1; // numeros entre 1 e 10000
    }

//criar os processos Px
    for (int i = 0; i < PROCESSES; i++) {
        pid = fork();
        if (pid == 0) {
            int media;
            posicao = i; // cada processo comeca a escrever no seu indice
            for (int j = 0; j < QUANT_VALORES_VEC_FINAL / 5; j++) {
                sem_wait(semaforosEscrita[i]); // Espera que o semaforo do processo seja 1
                media = calcularMediaMovel10(dados->vetorInicial, posicao * MOVING_AVERAGE_WINDOW); // Calcula a media movel
                dados->vetorFinal[posicao] = media; // Escreve no vetor final
                if (j == (QUANT_VALORES_VEC_FINAL / 5) - 1) { // Se for o ultimo numero do intervalo
                    dados->processosAcabados++; // Incrementa o count de processos acabados
                    if (dados->processosAcabados == PROCESSES) { // Se for o ultimo processo a acabar
                        sem_post(barreira); // Avisa o pai que todos os processos acabaram
                    }
                }
                sem_post(semaforoLeitura); // Avisa o pmax que pode ler o intervalo no vetor final
                posicao += PROCESSES; // Incrementa a posicao para o proximo numero
            }
            munmap(dados, size);
            close(fd);
            return 0;
        }
    }

// Processo Pmax
    pid = fork();
    if (pid == 0) {
        int auxMaior = 0, aux2, aux = 1; // auxMaior guarda o maior numero encontrado ate ao momento, aux2 guarda o valor anterior de auxMaior, aux e um contador para saber qual o processo que pode escrever
        for (int i = 0; i < QUANT_VALORES_VEC_FINAL; i++) { // O pmax vai ler todos os valores do vetor final
            aux2 = auxMaior; // Guarda o valor anterior de auxMaior
            sem_wait(semaforoLeitura); // espera por permissao para ler
            for (int j = 0; j < QUANT_VALORES_VEC_FINAL; j++) { // Percorre o vetor final
                if (auxMaior < dados->vetorFinal[j]) { // Se o valor atual for maior que o auxMaior
                    auxMaior = dados->vetorFinal[j]; // Guarda o valor atual
                }
            }
            if (auxMaior != aux2) { // se for um valor novo
                dados->highest_value = auxMaior; // Escreve no shared memory
                printf("Maior numero encontrado pelo processo pMAX!\n"); // Imprime no ecra
            }
            if (aux == PROCESSES) { 
                aux = 0; 
            }
            sem_post(semaforosEscrita[aux]); // Avisa o processo Px que pode escrever
            aux++; // Incrementa o contador
        }
        munmap(dados, size);
        close(fd);
        return 0;
    }

    if (dados->processosAcabados == PROCESSES) { // Se todos os processos acabaram
        sem_post(barreira);  // Avisa o pai que todos os processos acabaram
    }

    sem_wait(barreira); // Espera que todos os processos acabem

    //imprime o maior valor e o vetor final

    printf("[\n");
    for (int i = 0; i < QUANT_VALORES_VEC_FINAL; i++) {
        printf("%d, ",  dados->vetorFinal[i]);
    }
    printf("]\n");

    printf("maior número: %d\n", dados->highest_value);

    int checkAux;

    for (int i = 0; i < PROCESSES; i++) {
        checkAux = sem_close(semaforosEscrita[i]);
        if (checkAux == -1) {
            perror("Error in sem_close()");
            return 1;
        }
        checkAux = sem_unlink(semafProcessos[i]);
        if (checkAux == -1) {
            perror("Error in sem_unlink()");
            return 1;
        }
    }

    checkAux = sem_close(semaforoLeitura);
    if (checkAux == -1) {
        perror("Error in sem_close()");
        return 1;
    }
    checkAux = sem_unlink("/semaforoLeitura");
    if (checkAux == -1) {
        perror("Error in sem_unlink()");
        return 1;
    }
    checkAux = sem_close(barreira);
    if (checkAux == -1) {
        perror("Error in sem_close()");
        return 1;
    }
    checkAux = sem_unlink("/barreira");
    if (checkAux == -1) {
        perror("Error in sem_unlink()");
        return 1;
    }

    checkAux = munmap(dados, size);
    if (checkAux == -1) {
        perror("Error in munmap()");
        return 1;
    }
    checkAux = close(fd);
    if (checkAux == -1) {
        perror("Error in close()");
        return 1;
    }
    shm_unlink("/ex16");
    return 0;
}
