#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void arquivo(char maxTaxaS[]);
int lerArquivo();

void arquivo(char maxTaxaS[]){
    int maxTaxa;
    FILE *arq;
    maxTaxa = atoi(maxTaxaS);
    printf("Taxa máxima recebida: %i B/s.\n", maxTaxa);
    arq = fopen("config", "w");
    fprintf(arq, "%i", maxTaxa);
    fclose(arq);
    printf("Arquivo 'config' criado com sucesso.\n");
    system(EXIT_SUCCESS);
}

int lerArquivo(){
    int maxTaxa;
    FILE *arq;
    arq = fopen("config", "r");
    fscanf(arq, "%i", &maxTaxa);
    printf("Taxa de transferencia máxima setada para: %i Bytes por segundo.\n", maxTaxa);
    fclose(arq);
    return maxTaxa;
}
