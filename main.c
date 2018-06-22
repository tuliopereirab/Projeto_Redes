#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int server(int maxTaxa);
void arquivo(char maxTaxaS[]);
int lerArquivo();
void ajuda();
int lerArquivoTaxas();

int main(int argc, char **argv){
    int op, status;
    int maxTaxa;
    if(argc == 1){
        printf("Erro ao iniciar servidor, por favor utilize './Server -h' ou './Server --help' para receber ajuda.\n");
        return 0;
    }else{
        if(((strcmp(argv[1], "-i")) == 0) || ((strcmp(argv[1], "--iniciar")) == 0)){    // inicia servidor
            printf("Iniciar servidor!\n");
            maxTaxa = lerArquivo();
            printf("Taxa máxima recebida.\n");
            status = lerArquivoTaxas();
            if(status == 1)
                printf("Taxas dos clientes carregadas.\n");
            else
                printf("Nenhuma taxa para carregar.\n");
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
