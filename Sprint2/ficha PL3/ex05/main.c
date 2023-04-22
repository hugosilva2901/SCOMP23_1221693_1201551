#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <unistd.h>

#define SHM_SIZE sizeof(int) 

int *inteiro_partilhado;
int *flag;

// Handler que ao  receber um sinal SIGUSR1, altera o valor de flag para 1
// e ao receber um sinal SIGUSR2, altera o valor de flag para 0
void signal_handler(int signal) {
  if (signal == SIGUSR1) {
    *flag = 1;
  } else if (signal == SIGUSR2) {
    *flag = 0;
  }
}

int main() {
  int fd;
  pid_t pid;
  int i;

    // Temos um file descriptor para a shared memory
  flag = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); // para efeitos de sincronização
  if (flag == MAP_FAILED) {
    perror("erro mmap flag");
    exit(1);
  }
  *flag = 1; // inicializar flag a 1

  // Temos um file descriptor para a shared memory
  fd = shm_open("/ex05", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    perror("erro shm_open");
    exit(1);
  }

    //fttruncate para definir o tamanho da shared memory
  if (ftruncate(fd, SHM_SIZE) == -1) {
    perror("erro ftruncate");
    exit(1);
  }

    // Temos um apontador *inteiro_partilhado, que vai ser usado para aceder ao valor 100
  inteiro_partilhado = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (inteiro_partilhado == MAP_FAILED) {
    perror("erro mmap");
    exit(1);
  }

  *inteiro_partilhado = 100; // inicializar valor a 100

  pid = fork();
  if (pid == -1) {
    perror("erro fork");
    exit(1);
  } else if (pid == 0) { // filho
    for (i = 0; i < 1000000; i++) {
      while (*flag != 1) {
        // esperar pelo sinal
      }
      *flag = 0; // bloquear
      (*inteiro_partilhado)++; // incrementar
      (*inteiro_partilhado)--; // decrementar
      kill(getppid(), SIGUSR1); // notificar o pai
    }
    exit(0); // terminar com sucesso
  } else { // pai
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    for (i = 0; i < 1000000; i++) {
      while (*flag != 0) {
        // aguardar pelo sinal
      }
      *flag = 1; 
      (*inteiro_partilhado)++;
      (*inteiro_partilhado)--;
      kill(pid, SIGUSR1); // notificar o filho
      while (*flag != 1) {
        // aguardar pelo sinal
      }
      *flag = 0; 
      kill(pid, SIGUSR2); // notificar o filho
    }
    printf("valr final: %d\n", *inteiro_partilhado);
  }

  if (munmap(inteiro_partilhado, SHM_SIZE) == -1) {
    perror("erro munmap");
exit(1);
}

if (shm_unlink("/ex05") == -1) {
perror("erro shm_unlink");
exit(1);
}

if (munmap(flag, sizeof(int)) == -1) {
perror("erro munmap flag");
exit(1);
}

return 0;
}






