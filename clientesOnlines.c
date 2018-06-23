/*#include <stdio.h>
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

#define taxaPADRAO 100000


struct gerenciaClientes{
    char ip;
    int taxa;
    int taxaInicial;
    int numOnline;
};

int tamGerencia=0;


struct gerenciaClientes *gerenciar = NULL;

int verificarAtivo(char ip[]){
    int i, taxa;

    if(gerenciar == NULL){
        return -1;      // retorna que ip não está ativo
    }else{
        for(i=0; i<tamGerencia; i++){
            if((strcmp(gerencia[i].ip, ip)) == 0)
                return i;
        }
        return -1;       // retorna cliente inativo
    }

}

int ativarCliente(char ip[]){
    if(gerenciar == NULL){
        gerenciar = (struct gerenciaClientes*)malloc(sizeof(struct gerenciaClientes));
        tamGerencia++;
        taxa = adicionarTaxaIp(ip, tamGerencia-1);    // verificou que o ip ainda não esta ativo
    }else{
        tamGerencia++;
        gerenciar = realloc(gerenciar, sizeof(struct gerenciaClientes)*tamGerencia);
        taxa = adicionarTaxaIp(ip, tamGerencia-1)
    }
    return tamGerencia-1;
}

int atualizarTaxaIp(int id){

}

int adicionarTaxaIp(char ip[] int id){
    int taxa;
    char* ipVal;
    ipVal = ipVar(ip);
    printf("Ip var: %s\n", ipVal);
    taxa = verificaIp(ipVal);
    if(taxa == -1){          // retorna ip inexistente
        taxa = taxaPADRAO;
    }
    strcpy(gerenciar[id].ip, ip);
    gerenciar[id].taxaInicial = taxa;
    gerenciar[id].taxa = taxa;
    gerenciar[id].numOnline = 1;

}

int getTaxaId(int id){              // quando foi acessado pelo id, retorna valor da taxa
    return gerenciar[id].taxa;
}*/
