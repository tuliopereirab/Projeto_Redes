#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void ajuda();
void ajuda(){
    printf("===========================================================\n");
    printf("\t\t**** AJUDA ****\n");
    printf("* INICIAR SERVIDOR:\n");
    printf("\t1- Primeiro crie o arquivo de configuração utilizando './Server -a <taxa_maxima_transferencia>'.\n");
    printf("\t2- Com o arquivo criado, utilize './Server -i'.\n");
    printf("-----------------------------------------------\n");
    printf("* COMANDOS:\n");
    printf("\t- './Server -a <taxa_maxima_transferencia>': cria o arquivo de configuração com a taxa máxima de transferencia.\n");
    printf("\t- './Server --arquivo <taxa_maxima_transferencia>': cria o arquivo de configuração com a taxa máxima de transferencia.\n");
    printf("\t- './Server -i': inicia o servidor.\n");
    printf("\t- './Server --iniciar': inicia o servidor.\n");
    printf("\t- './Server -h': entra no ajuda.\n");
    printf("\t- './Server --help': entra no ajuda.\n");
    printf("-----------------------------------------------\n");
    printf("** TAXA MÁXIMA DE TRANSFERENCIA: o valor da taxa de transferência deve ser passado em Mbps.\n");
    printf("===========================================================\n");
    system(EXIT_SUCCESS);
}
