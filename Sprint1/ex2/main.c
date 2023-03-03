#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/*
Adicionado o waitpid() para que após cada fork() o processo pai espere que o processo filho termine.
*/

int main(void) {
	printf("I'm.. \n");
	pid_t process1 = fork(); //fork()A
	if(process1 > 0) { 
        waitpid(process1, NULL, 0); // espera que o processo1 termine
		pid_t process2 = fork(); //fork()B
		if(process2 > 0) { 
			printf("the.. \n"); 
            waitpid(process2, NULL, 0); // espera que o processo2 termine
			pid_t process3 = fork(); //fork()C	
			if(process3 > 0){	
				printf("father! \n");
			}else{
				printf("I'll never join you! \n"); 	
			}
		}else{
			printf("I'll never join you! \n");	
		}
	}else{
		printf("I'll never join you! \n");	
	}
return 0;
}
