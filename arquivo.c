#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void arquivo(char maxTaxaS[]);
int lerArquivo();

void arquivo(char maxTaxaS[]){
    int maxTaxa;
    FILE *arq;
    maxTaxa = atoi(maxTaxaS);
    printf("Taxa máxima recebida: %i Kbps.\n", maxTaxa);
    arq = fopen("config", "w");
    maxTaxa = maxTaxa/8*1000;          // salva no arquivo uma taxa em bytes, enquanto o cliente digita em Kbps
    fprintf(arq, "%i", maxTaxa);
    fclose(arq);
    printf("Arquivo 'config' criado com sucesso.\n");
    system(EXIT_SUCCESS);
}

int lerArquivo(){
    int maxTaxa;
    FILE *arq;
    arq = fopen("config", "r");
    fscanf(arq, "%i", &maxTaxa);            // ele salva e lê em bytes, porém apresenta para o cliente em Kbps
    maxTaxa = maxTaxa;
    printf("Taxa de transferencia máxima setada para: %i Kbps.\n", maxTaxa*8/1000);
    fclose(arq);
    return maxTaxa;
}
