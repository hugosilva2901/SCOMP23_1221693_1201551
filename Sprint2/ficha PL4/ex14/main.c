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
    sincronizacao de processos / exclusao mutua


    pseudocodigo

    Para um processo Ax
        esperoa pelo semaforo relativo aos processos A
        espera pelo semaforo relativo a escrita
        escreve no vetor final
        incrementa o semaforo relativo aos processos A
        incrementa o semaforo relativo a escrita
        incrementa o semaforo dos processos B, para que se nenhum A estiver a escrever, um B possa escrever


    Para um processo Bx
        esperoa pelo semaforo relativo aos processos B
        espera pelo semaforo relativo a escrita
        escreve no vetor final
        incrementa o semaforo relativo aos processos B
        incrementa o semaforo relativo a escrita
        incrementa o semaforo dos processos A, para que se nenhum B estiver a escrever, um A possa escrever    


// Cada processo nao produz dados, tem valores predefinidos


*/

#define QUANTIDADE 10
#define PROCESSO_AX 3
#define PROCESSO_BX 2


typedef struct {
  int numeros[QUANTIDADE];
} MemoriaCompartilhada;

int main(){
	int numeros_processo_a[PROCESSO_AX][QUANTIDADE] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}};
	int numeros_processo_b[PROCESSO_BX][QUANTIDADE] = {{3, 3, 3, 3, 3, 3, 3, 3, 3, 3}, {4, 4, 4, 4, 4, 4, 4, 4, 4, 4}};
	int fd, tamanho = sizeof(MemoriaCompartilhada);
    MemoriaCompartilhada *memoria_compartilhada;
	pid_t pid;
	srand(time(NULL));
    fd = shm_open("/Ex14", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    if(fd < 0){
        perror("shm_open");
        return 0;
    }
    if(ftruncate(fd, tamanho) < 0){
        perror("ftruncate");
        return 0;
    }
    memoria_compartilhada = (MemoriaCompartilhada*)mmap(NULL, tamanho, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	sem_t *processo_a, *processo_b, *escrita_permitida;
	if ((processo_a = sem_open("/proca", O_CREAT|O_EXCL, 0644, 1)) == SEM_FAILED || (processo_b = sem_open("/procb", O_CREAT|O_EXCL, 0644, 0)) == SEM_FAILED || (escrita_permitida = sem_open("/write", O_CREAT|O_EXCL, 0644, 1)) == SEM_FAILED){
		perror("Erro em sem_open()");
		exit(1);
	}
	for (int i = 0; i < PROCESSO_AX; i++) {
		pid = fork();
		if (pid == 0) {
			sleep(5);
			sem_wait(processo_a);
			sem_wait(escrita_permitida);
			printf("Processo A%d\n", i);
			for(int j = 0; j < QUANTIDADE; j++){
				memoria_compartilhada->numeros[j] = numeros_processo_a[i][j];
				printf(" Memoria[%d] : %d\n",j+1, memoria_compartilhada->numeros[j]);
			}
			printf("\n");
			sem_post(processo_a);
			sem_post(escrita_permitida);
			sem_post(processo_b);
			munmap(memoria_compartilhada, tamanho);
			close(fd);
			return 0;
		}
	}
	for(int i = 0; i < PROCESSO_BX; i++){
		pid = fork();
		if(pid == 0){
			sleep(6);
			sem_wait(processo_b);
			sem_wait(escrita_permitida);
			printf("Processo B%d\n", i);
			for(int j = 0; j < QUANTIDADE; j++){
				memoria_compartilhada->numeros[j] = numeros_processo_b[i][j];
				printf(" Memoria[%d] : %d\n",j+1, memoria_compartilhada->numeros[j]);
			}
			printf("\n");
			
			sem_post(processo_b);
			sem_post(escrita_permitida);
			sem_post(processo_a);
			munmap(memoria_compartilhada, tamanho);
			close(fd);
			return 0;
		}
	}
    
    for(int i = 0; i < (PROCESSO_AX + PROCESSO_BX); i++){
		wait(NULL);
	}
    
	if (sem_unlink("/proca") < 0 || sem_unlink("/procb") < 0 || sem_unlink("/write") < 0){
		perror("Erro em sem_unlink()");
		exit(1);
	}
	munmap(memoria_compartilhada, tamanho);
	close(fd);
	shm_unlink("/Ex14");
	return 0;
}
