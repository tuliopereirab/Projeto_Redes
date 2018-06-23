#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>

#include <time.h>

#include <pthread.h>

struct _cliente{
    int taxa;
    char ip[INET_ADDRSTRLEN];
};

int numIps=0;

struct _cliente *clientes = NULL;

// INTERNAS
int adicionarArquivo(int taxa, char ip[]);
int lerArquivoTaxas();
int verificaIp(char ip[]);
void escreverNoArquivo();
//void quebrarIp(char ipCliente[]);
char* ipVar(char ip[]);
// GLOBAIS
char h1[4];
char h2[4];
char h3[4];
char h4[4];
char valIp[100];
/*
int main(){
    int op, taxa;
    op = adicionarArquivo(10, "10.0.03.2");
    op = adicionarArquivo(20, "10.0.03.8");
    op = adicionarArquivo(1120, "10.42.03.8");
    op = lerArquivoTaxas();
    taxa = verificaIp("10.0.03.8");
    int i;
    printf("Taxa: %i\n", taxa);
}*/


int adicionarArquivo(int taxa, char ip[]){
    int i;
    char *valIp;
    numIps++;
    if(numIps == 1)
        clientes = (struct _cliente*)malloc(sizeof(struct _cliente));
    else
        clientes = realloc(clientes, sizeof(struct _cliente)*numIps);
    //quebrarIp(ip);
    clientes[numIps-1].taxa = taxa;
    valIp = ipVar(ip);
    strcpy(clientes[numIps-1].ip, valIp);
    printf("Mostrar IP Struct: %s\n", clientes[numIps-1].ip);
    escreverNoArquivo();
    return 1;
}

void escreverNoArquivo(){
    FILE *arquivo;
    int i;
    arquivo = fopen("../gerenciaTaxas", "w+");
    fprintf(arquivo, "%i\n", numIps);
    for(i=0;i<numIps; i++){
        printf("Escrevendo..\t%i\t%s\n", clientes[i].taxa, clientes[i].ip);
        fprintf(arquivo, "%i %s", clientes[i].taxa, clientes[i].ip);
        if(i != (numIps-1))
            fprintf(arquivo, "\n");
    }
    fclose(arquivo);
}

int lerArquivoTaxas(){
    char *ip;
    int i, tam;
    FILE *arq;
    arq = fopen("gerenciaTaxas", "r");
    if(arq == NULL)
        return 0;
    fscanf(arq, "%i\n", &tam);
    numIps = tam;
    if(tam != 0)
        clientes = (struct _cliente*)malloc(sizeof(struct _cliente)*tam);
    else
        return 0;
    for(i=0; i<tam; i++){
        fscanf(arq, "%i %s\n", &clientes[i].taxa, &clientes[i].ip);
    }
    fclose(arq);
    return 1;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>

#include <time.h>

#include <pthread.h>

struct _cliente{
    int taxa;
    char ip[INET_ADDRSTRLEN];
};

int numIps=0;

struct _cliente *clientes = NULL;

// INTERNAS
int adicionarArquivo(int taxa, char ip[]);
int lerArquivoTaxas();
int verificaIp(char ip[]);
void escreverNoArquivo();
int returnId(char ip[]);
//void quebrarIp(char ipCliente[]);
char* ipVar(char ip[]);
// GLOBAIS
char h1[4];
char h2[4];
char h3[4];
char h4[4];
char valIp[100];
/*
int main(){
    int op, taxa;
    op = adicionarArquivo(10, "10.0.03.2");
    op = adicionarArquivo(20, "10.0.03.8");
    op = adicionarArquivo(1120, "10.42.03.8");
    op = lerArquivoTaxas();
    taxa = verificaIp("10.0.03.8");
    int i;
    printf("Taxa: %i\n", taxa);
}*/


int adicionarArquivo(int taxa, char ip[]){
    int i;
    char *valIp;
    int status = lerArquivoTaxas();
    status = returnId(ip);
    if(status == -1){
        printf("Ip ainda não está na lista, adicionando-o...\n");
        numIps++;
        if(numIps == 1)
            clientes = (struct _cliente*)malloc(sizeof(struct _cliente));
        else
            clientes = realloc(clientes, sizeof(struct _cliente)*numIps);
        //quebrarIp(ip);
        clientes[numIps-1].taxa = taxa;
        valIp = ipVar(ip);
        strcpy(clientes[numIps-1].ip, valIp);
        //printf("Mostrar IP Struct: %s\n", clientes[numIps-1].ip);
    }else{
        printf("Cliente já está na lista, atualizando taxa...\n");
        clientes[status].taxa = taxa;
    }
    escreverNoArquivo();
    printf("Arquivo atualizado com sucesso!\n");
    return 1;
}

void escreverNoArquivo(){
    FILE *arquivo;
    int i;
    arquivo = fopen("gerenciaTaxas", "w+");
    fprintf(arquivo, "%i\n", numIps);
    for(i=0;i<numIps; i++){
        //printf("Escrevendo..\t%i\t%s\n", clientes[i].taxa, clientes[i].ip);
        fprintf(arquivo, "%i %s", clientes[i].taxa, clientes[i].ip);
        if(i != (numIps-1))
            fprintf(arquivo, "\n");
    }
    fclose(arquivo);
}

int lerArquivoTaxas(){
    char *ip;
    int i, tam;
    FILE *arq;
    arq = fopen("gerenciaTaxas", "r");
    if(arq == NULL)
        return 0;
    fscanf(arq, "%i\n", &tam);
    numIps = tam;
    if(tam != 0)
        clientes = (struct _cliente*)malloc(sizeof(struct _cliente)*tam);
    else
        return 0;
    for(i=0; i<tam; i++){
        fscanf(arq, "%i %s\n", &clientes[i].taxa, &clientes[i].ip);
    }
    fclose(arq);
    return 1;
}

int returnId(char ip[]){
    int i;
    for(i=0; i<numIps; i++)
        if((strcmp(ip, clientes[i].ip)) == 0)
            return i;
    return -1;
}


int verificaIp(char ip[]){
    int i;
    for(i=0; i<numIps; i++){
        if((strcmp(ip, clientes[i].ip)) == 0)
            return clientes[i].taxa;
    }
    return -1;         // ip não está na lista
}

int verificaIp(char ip[]){
    int i;
    for(i=0; i<numIps; i++){
        if((strcmp(ip, clientes[i].ip)) == 0)
            return clientes[i].taxa;
    }
    return -1;
}
