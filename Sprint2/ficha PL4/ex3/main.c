#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>

#define MAX_FRASES 50
#define MAX_CHARS 80

struct memoria_partilhada {
    int contador;
    char texto[MAX_FRASES][MAX_CHARS];
};

int main() {
    srand(time(NULL));
    int fd = shm_open("/ex02", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }
    ftruncate(fd, sizeof(struct memoria_partilhada));
    struct memoria_partilhada *mem = (struct memoria_partilhada *) mmap(NULL, sizeof(struct memoria_partilhada), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    mem->contador = 0;
    
    sem_t *sem = sem_open("/sem_ex02", O_CREAT | O_RDWR, 0666, 1);
    
    while (mem->contador < MAX_FRASES) {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 12;
        
        if (sem_timedwait(sem, &ts) == -1) {
            printf("Deadline missed.\n");
            continue;
        }
        
        int index = -1;
        for (int i = 0; i < MAX_FRASES; i++) {
            if (mem->texto[i][0] == '\0') {
                index = i;
                break;
            }
        }
        
        if (index == -1) {
            printf("memoria cheia.\n");
            break;
        }
        
        int random_number = rand() % 100 + 1;
        
        if (random_number <= 30 && mem->contador > 0) {
            mem->texto[mem->contador - 1][0] = '\0';
            mem->contador--;
            printf("Removi a ultima linha escrita.\n");
        } else {
            int pid = getpid();
            char texto[MAX_CHARS];
            snprintf(texto, MAX_CHARS, "I'm the Father - with PID %d", pid);
            strncpy(mem->texto[index], texto, MAX_CHARS);
            mem->contador++;
            printf("Escrevi: %s\n", mem->texto[index]);
        }
        
        sem_post(sem);
        
        int random_time = rand() % 5 + 1;
        sleep(random_time);
    }
    
    sem_close(sem);
    
    if (munmap(mem, sizeof(struct memoria_partilhada)) == -1) {
        perror("munmap");
        return 1;
    }
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return 0;
}
