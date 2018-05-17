#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char* readFileBytes(const char *name)    // Link de referencia da função está no arquivo de Links
{
    FILE *fl = fopen(name, "r");
    fseek(fl, 0, SEEK_END);
    long len = ftell(fl);
    char *ret = malloc(len);
    fseek(fl, 0, SEEK_SET);
    fread(ret, 1, len, fl);
    fclose(fl);
    return ret;
}

char* correcaoPort(char aux[]){
    int i, tam;
    tam = strlen(aux);
    for(i=0; i<tam; i++)
        if((aux[i] == '\n') || (aux[i] == ' ') || (isdigit((int)aux[i]) == 0)){
            aux[i] = '\0';
            i=tam;
        }
    return aux;
}
