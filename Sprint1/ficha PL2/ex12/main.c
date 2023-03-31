#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
struct pedido
{
    int id;
    int child;
};
struct product
{
    int id;
    char name[20];
    int price;
};

int main()
{
    int pipe_fdPedido[2];
    int pipe_fdPedidoChild[5][2];
    pid_t pid;
    int status[5];
    struct product p1;
    struct product p2;
    struct product p3;
    struct product p4;
    struct product p5;
    strcpy("leite", p1.name);
    strcpy("pao", p2.name);
    strcpy("carne", p3.name);
    strcpy("arroz", p4.name);
    strcpy("feijao", p5.name);
    p1.id = 1;
    p2.id = 2;
    p3.id = 3;
    p4.id = 4;
    p5.id = 5;
    p1.price = 10;
    p2.price = 5;
    p3.price = 20;
    p4.price = 15;
    p5.price = 10;
    if (pipe(pipe_fdPedido) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 5; i++)
    {
        if (pipe(pipe_fdPedidoChild[i]) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 5; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            close(pipe_fdPedido[0]);
            close(pipe_fdPedidoChild[i][1]);
            struct pedido pe1;
            pe1.id = i;
            pe1.child = i;
        }
    }
    if (pid == 0)
    {
        close(pipe_fdPedido[1]);
        close(pipe_fdPedidoChild[0]);
        int bet;
        srand(bet);
        while (1)
        {

            int credit;
            int signal;
            read(pipe_fdPedido[0], &signal, sizeof(signal));
            if (signal == 0)
            {
                printf("Child process terminating\n");
                exit(EXIT_SUCCESS);
            }

            bet = (rand() % 5) + 1;
            printf("Child process betting %d\n", bet);

            write(pipe_fdPedidoChild[1], &bet, sizeof(bet));

            read(pipe_fdPedido[0], &credit, sizeof(credit));
            printf("Child process new credit: %d euros\n", credit);
        }
        /*
                close(pipe_fdPedido[0]);
                close(pipe_fdPedido[1]);
                exit(EXIT_SUCCESS);
                */
    }
    else
    {
        close(pipe_fdPedido[0]);
        close(pipe_fdPedidoChild[1]);

        while (credit > 0)
        {
            int number;
            int bet;
            srand(bet);
            number = (rand() % 5) + 1;
            printf("\nParent process generated %d\n", number);

            int signal = 1;
            write(pipe_fdPedido[1], &signal, sizeof(signal));

            read(pipe_fdPedidoChild[0], &bet, sizeof(bet));
            printf("Parent process read bet %d\n", bet);
            if (number == bet)
            {
                credit += 10;
            }
            else
            {
                credit -= 5;
            }
            write(pipe_fdPedido[1], &credit, sizeof(credit));
            sleep(1);
        }

        int signal = 0;
        write(pipe_fdPedido[1], &signal, sizeof(signal));

        wait(&status);

        printf("Parent process terminating\n");
        close(pipe_fdPedido[0]);
        close(pipe_fdPedido[1]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}