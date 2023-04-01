    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/wait.h>
    #include <time.h>

    #define TAMANHO_ARRAY 50000
    #define NUMERO_FILHOS 10
    #define INTERVALO 5000

    typedef struct {
        int customer_code;
        int product_code;
        int quantity;
    } Sale;

    Sale sales[TAMANHO_ARRAY];

    int main() {
        srand(time(NULL));
        // preencher o array sales com valores aleatórios
        for (int i = 0; i < TAMANHO_ARRAY; i++) {
            sales[i].customer_code = rand();
            sales[i].product_code = rand() % 1000; 
            sales[i].quantity = rand() % 22;  //se aumentar muito os possiveis valores para quantidade, vamos ter imensas entries no array de produtos com +20 vendas.
        }

        
        int pipes[NUMERO_FILHOS][2]; 

        // criar os pipes
        for (int i = 0; i < NUMERO_FILHOS; i++) {
            if (pipe(pipes[i]) == -1) {
                perror("Erro cao criar pipe");
            return 1;
            }

            pid_t pid = fork(); // criar um filho

            if (pid == -1) {
                perror("Errro ao criar filho");
                return 1;
            } 
            else if (pid == 0) { // processo filho
                close(pipes[i][0]); // fechar o lado de leitura do pipe, filho so escreve

                int inicio= i * INTERVALO; 
                int fim = inicio+ INTERVALO;

                // Verificar se a quantidade é maior que 20 e escrever no pipe
                for (int j = inicio; j < fim; j++) {
                    Sale* sale = &sales[j];
                    if (sale->quantity > 20) { 
                        write(pipes[i][1], &sale->product_code, sizeof(int)); // Escrever no pipe
                    }
                }

                close(pipes[i][1]); // fecha o lado de escrita do pipe
                exit(0);
            } else { // processo pai
                close(pipes[i][1]); // fecha o lado de escrita do pipe
            }
        }

        // declarar o array products e ler os dados do pipe
        int products[TAMANHO_ARRAY];
        int products_count = 0;

        // Por cada pipe
        for (int i = 0; i < NUMERO_FILHOS; i++) {
            int product_code;
            ssize_t bytes_read;
            // Enquanto houver dados para ler
            while ((bytes_read = read(pipes[i][0], &product_code, sizeof(int))) > 0) {
                products[products_count++] = product_code; // Adicionar ao array o código do produto
            }
            close(pipes[i][0]); // fecha o lado de leitura do pipe
        }

        printf("Codigos de produtos com vendas superiores a 20 unidades:\n");
        printf("Quantidade de produtos: %d\n", products_count);
        for (int i = 0; i < products_count; i++) {
        printf("%d ", products[i]);
        }

        return 0;
    }
