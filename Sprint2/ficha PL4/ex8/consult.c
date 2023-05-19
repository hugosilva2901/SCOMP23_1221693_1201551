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


/*Programa de consult
	Pattern: semaforo inicializado a 1 (mutual exclusion accessing shared resources)

	Pseudocode:

		esperar que o semaforo esteja livre

		Pedir numero do utilizador
		ler numero do utilizador

		Se o index for 0
			imprimir não há dados

		Caso contrário
			for(numero do index)
				obter estrutura presente no index
					Se o numero da estrutura for igual ao numero do utilizador
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
	int number;
	int fd = shm_open("/ex8", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
    perror("fd  \n");
		exit(EXIT_FAILURE);
	}
	int size = sizeof(SharedData);
	if(ftruncate(fd, size) == -1){
		perror("ftruncate \n");
		exit(0);
	}
	SharedData *sd = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(sd == NULL){
		printf("mmap\n");
		exit(0);
	}	
	sem = sem_open("/sem8", O_CREAT, 0644, 1);
	if(sem == SEM_FAILED){
		printf("  sem_open\n");
		return 0;
	}
	sem_wait(sem);
	printf("number:\n");
	scanf("%d", &number);

	if(sd->index == 0){
		printf("No personal_datas available.\n");
	}
	else{
		int i;
		for(i = 0; i < sd->index; i++){
			personal_data *r = &(sd->records[i]);
			if(r->number == number){
				printf("Number: %d  Name: %s  Address: %s\n",r->number, r->name, r->address);
				printf("\n");
			}
		}
	}
	sem_post(sem);
	sem_unlink("/sem8");
	munmap(sd, size);
	close(fd);
	return 0;
}