#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>


/*Programa de consult all
	Pattern : Semaforo inicializado a 1 (mutual exclusion accessing shared resources)

	Pseudo-code:
		esperar que o semaforo esteja livre

		Se o index for 0
			imprimir não há dados

		Caso contrário
			for(numero do index)
				obter estrutura presente no index
					imprimir numero
					imprimir nome
					imprimir morada


		libertar o semaforo(sem_post)				
*/

typedef struct{
	int number;
	char name[50];
	char address[100];
}personal_data;

typedef struct{
	personal_data records[100];
	int index;
}SharedData;

int main(void){
	sem_t *sem;
	int fd = shm_open("/ex8", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("erro no shm_open\n");
		return 1;
	}
	int size = sizeof(SharedData);
	if(ftruncate(fd, size) == -1){
		perror("erro no ftruncate\n");
		return 1;
	}
	SharedData *sd = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(sd == NULL){
		printf("erro no mmap\n");
		return 1;
	}
	sem = sem_open("/sem8", O_CREAT, 0644, 1);
	if(sem == SEM_FAILED){
		printf("erro no sem_open\n");
		return 1;
	}
	sem_wait(sem);
	if(sd->index == 0){
		printf("empty\n");
	}else{
		int i;
		for(i = 0; i < sd->index; i++){
			personal_data *r = &(sd->records[i]);
			printf("\nNumber: %d  Name: %s  Address: %s\n", r->number, r->name, r->address);
			printf("\n");
		}
	}
	sem_post(sem);
	sem_unlink("/sem8");
	munmap(sd, size);
	close(fd);
	return 0;
}