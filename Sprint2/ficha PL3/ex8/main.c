#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

 typedef struct
{
    char file[100];
    char word[15];
}info;

int main() {
    info inf[10];
    pid_t pid;
    strcpy(inf[0].file, "txt0.txt");
    strcpy(inf[0].word, "shade");
    strcpy(inf[1].file, "txt1.txt");
    strcpy(inf[1].word, "stood");
    strcpy(inf[2].file, "txt2.txt");
    strcpy(inf[2].word, "waves");
    strcpy(inf[3].file, "txt3.txt");
    strcpy(inf[3].word, "alive");
    strcpy(inf[4].file, "txt4.txt");
    strcpy(inf[4].word, "starting");
    strcpy(inf[5].file, "txt5.txt");
    strcpy(inf[5].word, "bustling");
    strcpy(inf[6].file, "txt6.txt");
    strcpy(inf[6].word, "distance");
    strcpy(inf[7].file, "txt7.txt");
    strcpy(inf[7].word, "steady");
    strcpy(inf[8].file, "txt8.txt");
    strcpy(inf[8].word, "stretched");
    strcpy(inf[9].file, "txt9.txt");
    strcpy(inf[9].word, "majestically");

    int *shared_array;
    int fd;
    fd = shm_open("/main", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, 10*sizeof(int));
    shared_array = mmap(NULL, 10*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
     if (shared_array == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    for (int i = 0; i < 10; i++)
    {
       pid = fork();
         if (pid == -1) {
          perror("fork");
          exit(1);
        } else if (pid == 0) {  
            FILE *fp;
            char *line = NULL;
            size_t len = 0;
            ssize_t read;
            int count = 0;
            fp = fopen(inf[i].file, "r");
            if (fp == NULL)
                exit(EXIT_FAILURE);
            while ((read = getline(&line, &len, fp)) != -1) {
                char *token = strtok(line, " ");
                while (token != NULL) {
                    if (strcmp(token, inf[i].word) == 0) {
                        count++;
                    }
                    token = strtok(NULL, " ");
                }
            }
            shared_array[i] = count;
            fclose(fp);
            if (line)
                free(line);
            close(fd);
            exit(0);
        }

    }
    for (int i = 0; i < 10; i++)
    {
        wait(NULL);
    }
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n",shared_array[i]);
    }
    munmap(shared_array, 10*sizeof(int));
    close(fd);
    shm_unlink("/main");   
    return 0;
}