#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int carregarClientes();
int cadastrar(char nome[], char senha[]);


struct clientes{
    char nome[30];
    char senha[30];
    int status;
};

struct clientes *listaClientes;
int nClientes;

int carregarClientes(){
    FILE *arq;

    int i;
    arq = fopen("clientesCadastrados.txt", "r");
    if(arq == NULL){
        return 0;
    }
    fscanf(arq, "%i\n", &nClientes);
    listaClientes = malloc(sizeof(struct clientes));
    for(i=0; i<nClientes; i++){
        fscanf(arq, "%s\t%s\n", &listaClientes[i].nome, &listaClientes[i].senha);
        listaClientes[i].status = 0;
    }
    fclose(arq);
    return 1;
}

int logar(char nome[], char senha[]){
    int i, status = 0;
    FILE *arq;
    i = 0;


    while(i < nClientes){
        if((strcmp(listaClientes[i].nome, nome)) == 0){
            if((strcmp(listaClientes[i].senha, senha)) == 0){
                if(listaClientes[i].status == 0){
                    listaClientes[i].status = 1;
                    return 1; // 1 = cliente logado com sucesso
                }else
                    return 10;  // 10 = cliente já logado
            }else
                return 0;  // 0 = senha incorreta
        }
        i++;
    }


    printf("LOGIN cliente não encontrado\n");
    status = cadastrar(nome, senha);
    return status;
}

int pegarIdCliente(char nome[]){
    int i, id;
    for(i=0; i<nClientes; i++)
        if((strcmp(listaClientes[i].nome, nome)) == 0){
            id = i;
            return id;
        }
}

int cadastrar(char nome[], char senha[]){
    FILE *arq;
    int i;
    nClientes++;


    listaClientes = realloc(listaClientes, sizeof(struct clientes) * nClientes);

    strcpy(listaClientes[nClientes-1].nome, nome);
    strcpy(listaClientes[nClientes-1].senha, senha);
    listaClientes[nClientes-1].status = 1;


    system("rm clientesCadastrados.txt");
    printf("LOGIN cadastrado com sucesso\n");

    arq = fopen("clientesCadastrados.txt", "w");

    fprintf(arq, "%d\n", nClientes);
    for(i=0; i<nClientes; i++){
        fprintf(arq, "%s\t%s", listaClientes[i].nome, listaClientes[i].senha);
        //fprintf(arq, "\t");
        //fprintf(arq, "%s", listaClientes[i].senha);
        if(i < (nClientes-1))
            fprintf(arq, "\n");
    }
    printf("LOGIN lista de clientes atualizada\n");
    fclose(arq);

    strcpy(listaClientes[nClientes-1].nome, nome);
    strcpy(listaClientes[nClientes-1].senha, senha);

    return 2; // 2 - cliente cadastrado com sucesso
}

void finalizarSessao(int idCliente){
    listaClientes[idCliente].status = 0;
    printf("Cliente \'%s\' agora está offline\n", listaClientes[idCliente].nome);
    return 1;
}
