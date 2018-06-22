#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void arquivo(char maxTaxaS[]);
float lerArquivo();

void arquivo(char maxTaxaS[]){
    float maxTaxa;
    FILE *arq;
    maxTaxa = atof(maxTaxaS);
    printf("Taxa máxima recebida: %.2f B/s.\n", maxTaxa);
    arq = fopen("config", "w");
    fprintf(arq, "%.2f", maxTaxa);
    fclose(arq);
    printf("Arquivo 'config' criado com sucesso.\n");
    system(EXIT_SUCCESS);
}

float lerArquivo(){
    float maxTaxa;
    FILE *arq;
    arq = fopen("config", "r");
    fscanf(arq, "%f", &maxTaxa);
    printf("Taxa de transferencia máxima setada para: %.2f Bytes por segundo.\n", maxTaxa);
    fclose(arq);
    return maxTaxa;
}
