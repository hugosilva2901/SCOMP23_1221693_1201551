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


/*Programa de insert*/
/*Tambem existe programa de consult e consult_all*/

/*Pattern:
	Temos um semaforo inicializado a 1(mutual exclusion accessing shared resources)

Pseudo-code:
	esperar que o semaforo esteja livre  
	  
	  Pedir numero do utilizador
	  Ler numero do utilizador
	  registar numero do utilizador na estrutura

	  Pedir nome do utilizador
	  Ler nome do utilizador
	  registar nome do utilizador na estrutura

	  pedir morada do utilizador
	  Ler morada do utilizador
	  registar morada do utilizador na estrutura

	  index++

	libertar o semaforo


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
	char name[50];
	char address[100];
	int fd = shm_open("/ex8", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("Open error\n");
		exit(0);
	}
	int size = sizeof(SharedData); 
	if(ftruncate(fd, size) == -1){
		perror("Ftruncate error\n");
		exit(0);
	}
	SharedData *sd = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(sd == NULL){
		printf("Erro no mmap\n");
		exit(0);
	}
	sem = sem_open("/sem8", O_CREAT, 0644, 1); 
	if(sem == SEM_FAILED){
		printf("Erro no sem_open\n");
		return 0;
	}
	sem_wait(sem);      
	personal_data *r = &(sd->records[sd->index]); 
	printf("Number:\n");	
	scanf("%d", &(r->number));
	printf("Name:\n");		scanf("%s", name);
	strcpy(r->name, name);
	printf("Adress:\n"); 
	scanf("%s", address);
	strcpy(r->address, address); 
	sd->index++; 
	sem_post(sem);
	sem_unlink("/sem8");
	munmap(sd, size);
	close(fd);
	return 0;
}