#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_FRASES 50
#define MAX_CHARS 80

struct memoria_partilhada {
    int contador;
    char texto[MAX_FRASES][MAX_CHARS];
};
int main() {
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
    while (1) {
        printf("Total Linhas: %d\n", mem->contador);
        for (int i = 0; i < MAX_FRASES; i++) {
            if (mem->texto[i][0] != '\0') {
                printf("%s\n", mem->texto[i]);
            }
        }
        sleep(1);
    }
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
