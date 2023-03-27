#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define TAMANHO_ARRAY 50000
#define INTERVALO_PROCURA 5000
#define NUM_FILHOS 10
#define MINIMO_UNIDADES_VENDIDAS 20

typedef struct {
    int customer_code;
    int product_code;
    int quantity;
} SaleRecord;

int sales[TAMANHO_ARRAY];
int products[TAMANHO_ARRAY];
int num_products = 0;

// Funcao que procura os produtos numa determinada parte do array e tambem preenche o array com produtos
void procurar(int indiceInicio, int indiceFim, int pipe_fd[]) {
    int product_code;
    for (int i = indiceInicio; i < indiceFim; i++) { // Por cada intervalo de procura
        SaleRecord venda = {rand(), rand() % TAMANHO_ARRAY, rand() % 26}; // Cria uma venda aleatoria, com a quantidade entre 0 e 25
        if (venda.quantity > MINIMO_UNIDADES_VENDIDAS) { // Se a quantidade vendida for maior que o minimo
            product_code = venda.product_code;
            write(pipe_fd[1], &product_code, sizeof(product_code)); // Escreve o codigo do produto no pipe
        }
    }
    close(pipe_fd[1]);
    return 0;
}

int main() {
    int pipe_fd[NUM_FILHOS][2];
    pid_t pids[NUM_FILHOS];
    srand(time(NULL)); // para gerar numeros aleatorios diferentes

    // criar os pipes e os filhos
    for (int i = 0; i < NUM_FILHOS; i++) {
        if (pipe(pipe_fd[i]) == -1) {
            perror("Ero ao criar pipe");
            return 1;
        }
        pid_t pid = fork();
        if (pid == -1) {
            perror("Erro ao fazer fork");
            return 1;
        } 
        else if (pid == 0) { // filho
            close(pipe_fd[i][0]);  // fechar o lado de leitura do pipe, o filho so escreve no pipe
            int indiceInicio = i * INTERVALO_PROCURA;
            int indiceFim = indiceInicio + INTERVALO_PROCURA;
            procurar(indiceInicio, indiceFim, pipe_fd[i]); // Chamar a funcaoi para popular e procurar o intervalo do array
        } 
        else { // pai
            close(pipe_fd[i][1]);  // parent only reads from pipe
            pids[i] = pid;
        }
    }

    // o processo pai espera que todos os filhos terminem
    for (int i = 0; i < NUM_FILHOS; i++) {
        waitpid(pids[i], NULL, 0);
    }

    printf("\nProdutos que venderam mais que %d unidades numa venda:\n", MINIMO_UNIDADES_VENDIDAS);

    // o processo pai le os codigos dos pipes e guarda os no produtcts
    int product_code;
    for (int i = 0; i < NUM_FILHOS; i++) {
        while (read(pipe_fd[i][0], &product_code, sizeof(product_code)) > 0) { // enquanto houver codigos para ler
            products[num_products++] = product_code; // guarda o codigo do produto no array
        }
        close(pipe_fd[i][0]); // fechar o lado de leitura do pipe
    }

    for (int i = 0; i < num_products; i++) {
        printf("%d ", products[i]);
    }
    printf("\n");
    return 0;
}

