#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define STR_SIZE 50
#define NR_DISC 10
//commit fix

//struct tirada do enunciado
struct aluno {
    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
};

// Queremos partilhar algumas informacoes do aluno, tais como:
// - o aluno (struct aluno)
// - a maior nota e a menor nota
// - a media das notas
// - um flag para indicar que os dados estao prontos para ler/escrever (data_ready)
struct shared_data {
    struct aluno student;
    int highest_grade;
    int lowest_grade;
    float media;
    int data_ready;
};

int main() {
    int fd;
    struct shared_data *data;
    pid_t pid1, pid2;

    // Create a shared memory area
    fd = shm_open("/shared_memory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(fd, sizeof(struct shared_data)) == -1) {
        perror("ftruncate");
        exit(1);
    }
    data = (struct shared_data *)mmap(NULL, sizeof(struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // inicializar os dados
    data->highest_grade = 0; // Inicializamos a 0(nota minima) para poder haver notas mais altas
    data->lowest_grade = 20; // Inicializamos a 20(nota maxima) para poder haver notas mais baixas
    data->media = 0.0; 
    data->data_ready = 0;

    // criar os processos
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(1);
    } else if (pid1 == 0) {  // primeiro filho
        // esperar que o utilizador introduza os dados
        while (!data->data_ready) {
            sleep(1);
        }
        // calcular a maior e a menor nota
        for (int i = 0; i < NR_DISC; i++) {
            int grade = data->student.disciplinas[i];
            if (grade > data->highest_grade) {
                data->highest_grade = grade;
            }
            if (grade < data->lowest_grade) {
                data->lowest_grade = grade;
            }
        }
        // Imprimir a informacao
        printf("Processo filho 1 \n");
        printf("numero estudante: %d\n", data->student.numero);
        printf("nome estudante: %s\n", data->student.nome);
        printf("nota + alta: %d\n", data->highest_grade);
        printf("nota + baixa: %d\n", data->lowest_grade);
        exit(0);
    } else {

        pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            exit(1);
        } else if (pid2 == 0) {  // segundo filho
            // esperar pelos dados
            while (!data->data_ready) {
                sleep(1);
            }
            // calcular a media das notas
            float sum = 0.0;
            for (int i = 0; i < NR_DISC; i++) {
                sum += data->student.disciplinas[i];
            }
            data->media = sum / NR_DISC;
            // imprimir a informacao
            printf("Processo filho 2\n ");
            printf("Student number: %d\n", data->student.numero);
            printf("Student name: %s\n", data->student.nome);
            printf("Average grade: %.2f\n", data->media);
            exit(0);
        }
       
}

// preencher os dados
printf("numero estudante: ");
scanf("%d", &data->student.numero);
printf("nome do estudante: ");
scanf("%s", data->student.nome);
for (int i = 0; i < NR_DISC; i++) {
    printf("nota da disciplina %d: ", i+1);
    scanf("%d", &data->student.disciplinas[i]);
}
data->data_ready = 1; // os dados estao prontos

// esperar que os 2 filhos terminem
int status;
waitpid(pid1, &status, 0);
waitpid(pid2, &status, 0);

// eliminar a memoria partilhada
if (munmap(data, sizeof(struct shared_data)) == -1) {
    perror("munmap");
    exit(1);
}
if (shm_unlink("/shared_memory") == -1) {
    perror("shm_unlink");
    exit(1);
}

return 0;
}