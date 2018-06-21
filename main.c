#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int server(float maxTaxa);
void arquivo(char maxTaxaS[]);
float lerArquivo();
void ajuda();

int main(int argc, char **argv){
    int op;
    float maxTaxa;
    if(argc == 1){
        printf("Erro ao iniciar servidor, por favor utilize './Server -h' ou './Server --help' para receber ajuda.\n");
        return 0;
    }else{
        if(((strcmp(argv[1], "-i")) == 0) || ((strcmp(argv[1], "--iniciar")) == 0)){    // inicia servidor
            printf("Iniciar servidor!\n");
            maxTaxa = lerArquivo();
            printf("Taxa máxima recebida.\n");
            server(maxTaxa);
        }
        else if(((strcmp(argv[1], "-a")) == 0) || ((strcmp(argv[1], "--arquivo")) == 0))
            if(argc == 3)
                arquivo(argv[2]);
            else{
                printf("Erro ao salvar arquivo, por favor utilize './Server -h' ou './Server --help' para receber ajuda.\n");
                return 0;
            }
        else if(((strcmp(argv[1], "-h")) == 0) || ((strcmp(argv[1], "--help")) == 0))
            ajuda();
    }
    return 0;
}
