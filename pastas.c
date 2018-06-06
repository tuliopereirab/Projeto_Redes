#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <pthread.h>

int vPasta(char pasta[]);
int cPasta(char pasta[], char nome[], int op);
int ePasta(char pasta[]);
int sairPasta(char pasta[]);
char* rPasta(char pasta[]);
char* aPasta(char pasta[], char newPasta[]);
int contarPastas(char pasta[]);


int vPasta(char pasta[]){
    int i, status, val = contarPastas(pasta);
    printf("ValPastas: %i\n", val);
    status = chdir(pasta);
    if(status == 0) sairPasta(pasta);
    return status;
}

int cPasta(char pasta[], char nome[], int op){
    int status, tam;
    char *pastaCriar;
    if(op != 0)
        pastaCriar = aPasta(pasta, nome);
    else{
        tam = strlen(nome);
        pastaCriar = (char*)malloc(sizeof(char)*tam);
        strcpy(pastaCriar, nome);
    }
    status = mkdir(pastaCriar, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
    return status;
}

int ePasta(char pasta[]){
    int status;
    status = chdir(pasta);
    return status;
}

int sairPasta(char pasta[]){
    int i;
    int val = contarPastas(pasta);
    printf("Valor: %i\n", val);
    for(i=val; i>0; i--){
        chdir("..");
    }
    return 1;
}

char* rPasta(char pasta[]){
    char* new;
    int newTam=0, ultTam;
    int i, posUltBarra, tam;
    tam = strlen(pasta);
    ultTam=0;
    for(i=0; i<tam; i++){
        if(pasta[i] == '/'){
            posUltBarra = i;
            ultTam = i;
        }
    }
    new = (char*)malloc(sizeof(char)*ultTam);
    for(i=0; i<ultTam; i++){
        new[i] = pasta[i];
    }
    return new;
}

char* aPasta(char pasta[], char newPasta[]){
    char *pasta1;
    int tam;
    tam = strlen(pasta);
    pasta1 = (char*)malloc(sizeof(char)*tam);
    strcpy(pasta1, pasta);
    tam = tam + strlen(newPasta);
    pasta1 = realloc(pasta1, sizeof(char)*tam);
    strcat(pasta1, "/");
    strcat(pasta1, newPasta);
    return pasta1;
}

int contarPastas(char pasta[]){
    int tam, n=0, i;
    tam = strlen(pasta);
    for(i=0; i<tam; i++){
        if(pasta[i] == '/')
            n++;
    }
    n++;
    return n;
}
