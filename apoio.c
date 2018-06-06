#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int ipToInt(char ip[], int op);
int main(){
    int v[4];
    int i;
    for(i=0; i<4; i++){
        v[i] = ipToInt("127.123.43.32", i);
        printf("Valor[%i]: %i\n", i, v[i]);
    }
    return 0;

}

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

int buscarThread(int controle[], int nThreads){
    int i;
    for(i=0; i<nThreads; i++)
        if(controle[i] == 0)
            return i;
    return -1;
}

int ipToInt(char ip[], int op){
    char val[4];
    int tam = strlen(ip);
    int i, j,z;
    int retorno;
    switch(op){
        case 0:
            i=0;
            while(ip[i] != '.'){
                val[i] = ip[i];
                i++;
            }
            retorno = atoi(val);
            return retorno;
            break;
        case 1:
            i=0;
            j=0;
            z=0;
            while(i<tam){
                if(z==1){
                    if(ip[i] == '.'){
                        retorno = atoi(val);
                        return retorno;
                    }
                    val[j] = ip[i];
                    j++;
                }
                if((z==0)&&(ip[i] == '.'))
                    z++;
                i++;
            }
            retorno = atoi(val);
            return retorno;
            break;
        case 2:
            i=0;
            j=0;
            z=0;
            while(i<tam){
                if(z==2){
                    if(ip[i] == '.'){
                        retorno = atoi(val);
                        return retorno;
                    }
                    val[j] = ip[i];
                    j++;
                }

                if(((z==0) || (z==1)) && (ip[i] == '.'))
                    z++;
                i++;
            }
            retorno = atoi(val);
            return retorno;
            break;
        case 3:
            i=0;
            j=0;
            z=0;
            while(i<tam){

                if(z==3){
                    if(ip[i] == '.' || ip[i] == ' '){
                        retorno = atoi(val);
                        return retorno;
                    }
                    val[j] = ip[i];
                    j++;
                }
                if(((z == 0) || (z==1) || (z==2)) && (ip[i] == '.'))
                    z++;
                i++;
            }
            retorno = atoi(val);
            return retorno;
            break;
    }
}
