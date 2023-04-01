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
    pid_t pid[5];
    int status[5];
    struct product p[20];
    
    strcpy( p[0].name,"leite");
    strcpy( p[1].name,"queijo");
    strcpy( p[2].name,"pao");
    strcpy( p[3].name,"carne");
    strcpy( p[4].name,"arroz");
    strcpy( p[5].name,"feijao");
    strcpy( p[6].name,"frango");
    strcpy( p[7].name,"peixe");
    strcpy( p[8].name,"batata");
    strcpy( p[9].name,"cenoura");
    strcpy( p[10].name,"alface");
    strcpy( p[11].name,"tomate");
    strcpy( p[12].name,"cebola");
    strcpy( p[13].name,"alho");
    strcpy( p[14].name,"azeite");
    strcpy( p[15].name,"vinho");
    strcpy( p[16].name,"cerveja");
    strcpy( p[17].name,"agua");
    strcpy( p[18].name,"sumo");
    strcpy( p[19].name,"refrigerante");


    p[0].id=1;
    p[1].id=2;
    p[2].id=3;
    p[3].id=4;
    p[4].id=5;
    p[5].id=6;
    p[6].id=7;
    p[7].id=8;
    p[8].id=9;
    p[9].id=10;
    p[10].id=11;
    p[11].id=12;
    p[12].id=13;
    p[13].id=14;
    p[14].id=15;
    p[15].id=16;
    p[16].id=17;
    p[17].id=18;
    p[18].id=19;
    p[19].id=20;
    p[0].price=10;
    p[1].price=20;
    p[2].price=30;
    p[3].price=40;
    p[4].price=50;
    p[5].price=60;
    p[6].price=15;
    p[7].price=25;
    p[8].price=35;
    p[9].price=45;
    p[10].price=55;
    p[11].price=65;
    p[12].price=20;
    p[13].price=30;
    p[14].price=40;
    p[15].price=50;
    p[16].price=60;
    p[17].price=70;
    p[18].price=12;
    p[19].price=22; 

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
        pid[i] = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid[i] == 0)
        {
            
            close(pipe_fdPedido[0]);
            for (int k = 0; k < 5; k++)
            {
            close(pipe_fdPedidoChild[k][1]);
            }
            
            int j=0;
            struct pedido pe1;
            srand( pe1.id);
            while (j<5)
            {
            pe1.id= (rand() % 20) ;
            pe1.child = i;
            write(pipe_fdPedido[1], &pe1, sizeof(pe1));
            struct product pchild;
            
            read(pipe_fdPedidoChild[i][0],&pchild,sizeof(pchild));
            printf("product id %d name %s price %d\n ",pchild.id,pchild.name,pchild.price);
            j++;
            }
            close(pipe_fdPedido[1]);
            for (int i = 0; i < 5; i++)
            {
                close(pipe_fdPedidoChild[i][0]);
            }
            exit(EXIT_SUCCESS);     
        }
    }
        close(pipe_fdPedido[1]);
            for (int i = 0; i < 5; i++)
            {
                close(pipe_fdPedidoChild[i][0]);
            }
        ssize_t n=1;
        while (n!=0)
        {
        struct pedido msg;
        n = read(pipe_fdPedido[0], &msg, sizeof(msg));

        printf("\nPedido do produto %d com o nome %s do filho %d\n", p[msg.id].id,p[msg.id].name, msg.child);
        write(pipe_fdPedidoChild[msg.child][1], &p[msg.id], sizeof(p[msg.id]));
        sleep(1);
        }
        close(pipe_fdPedido[0]);
            for (int k = 0; k < 5; k++)
            {
            close(pipe_fdPedidoChild[k][1]);
            }
    

    return 0;
}