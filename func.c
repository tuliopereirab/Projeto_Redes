#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include <sys/ioctl.h>
#include <fcntl.h>

#include <time.h>
#include <pthread.h>

#include <dirent.h>

struct vetChar{
    char byte[8];
};

#define MAXBUF 100

struct argus{
    int port;
};

int statusThread();
void *chamadaThreadData(void *arg);
int iniciarConexaoDados(int cliente, int port, char ipCliente[]);
int calcPort(int val1, int val2);
int calcPortPASV(int val, int set);
void finalizarSessao(int idCliente);
void loopErro();
char* readFileBytes(const char *name);
char* correcaoPort(char aux[]);
int vPasta(char pasta[]);
int cPasta(char pasta[], char nome[], int op);
int ePasta(char pasta[]);
int sairPasta(char pasta[]);
char* rPasta(char pasta[]);
char* aPasta(char pasta[], char newPasta[]);
int contarPastas(char pasta[]);
void conexaoModoPassivo(int port);
int retornarDataCon();
char* ipVarPasv(char ip[]);

void opQuit(int cliente, int idCliente, char ipCliente[]){
    int statusFinalizar;
    char msgEnvia[100];
    printf("QUIT cliente id %i finalizando conexão\n");
    strcpy(msgEnvia, "221 Finalizando conexao\n");
    write(cliente, msgEnvia, strlen(msgEnvia)+1);
    finalizarSessao(idCliente);
    /*statusFinalizar = finalizarConexao();
    if(statusFinalizar != 0){
        printf("Erro ao finalizar conexão: pasta raiz não pode ser retornada\nFINALIZANDO SERVIDOR!\n");
        close(cliente);
        loopErro();
    }else{*/
        //printf("Servidor retornado para pasta raiz\n");
    //}
    printf("Finalizando conexao: pedido cliente IP %s\n", ipCliente);
    close(cliente);
    printf("Conexao finalizada\n");
    printf("--------------------------------------------------------------\n");
}

int opPort(char portas[]){
    int i, j;
    int porta1, porta2, port, tam;
    char aux1[20], aux2[20];
    char *correcaoAux1, *correcaoAux2;
    i = 0, j=0;
    tam = strlen(portas);
    while((i<tam) && (j<4)){
        if(portas[i] == ',')
            j++;
        i++;
    }

    j=0;

    while(portas[i] != ',')
        aux1[j++] = portas[i++];
    j=0;
    i++;
    while(portas[i] != '\0')
        aux2[j++] = portas[i++];
    aux2[j] = '\0';

    tam = strlen(aux1);
    for(i=0; i<tam; i++){
        if((aux1[i] != '0') && (aux1[i] != '1') && (aux1[i] != '2') && (aux1[i] != '3') && (aux1[i] != '4') && (aux1[i] != '5') && (aux1[i] != '6') && (aux1[i] != '7') && (aux1[i] != '8') && (aux1[i] != '9')){
            if(i == 0){
                for(j=0; i<tam; i++){
                    aux1[j] = aux1[j+1];
                }
            }else if(i > 1){
                aux1[i] = '\0';
            }
        }
    }

    tam = strlen(aux2);
    for(i=0; i<tam; i++){
        if((aux2[i] != '0') && (aux2[i] != '1') && (aux2[i] != '2') && (aux2[i] != '3') && (aux2[i] != '4') && (aux2[i] != '5') && (aux2[i] != '6') && (aux2[i] != '7') && (aux2[i] != '8') && (aux2[i] != '9')){
            if(i == 0){
                for(j=0; i<tam; i++){
                    aux2[j] = aux2[j+1];
                }
            }else if(i > 1){
                aux2[i] = '\0';
            }
        }
    }

    correcaoAux1 = correcaoPort(aux1);
    correcaoAux2 = correcaoPort(aux2);

    //printf("Aux corrigidos: %s\t%s\n", correcaoAux1, correcaoAux2);

    porta1 = atoi(aux1);
    porta2 = atoi(aux2);
    port = calcPort(porta1, porta2);
    return port;
}

int opLs(int cliente, int port, char ipCliente[], int passiveMode, char pasta[], int maxTaxa){
    printf("LIST solicitado\n");
    DIR *dir;
    struct dirent *lsdir;
    dir = opendir(pasta);
    struct stat obj;
    int dataCon;
    int tamanho, i;
    //int arquivo;
    char msgEnvia[100];
    char *arquivo;
    int statusPassive;
    char *enviar = (char*)malloc(sizeof(char)*1000);
    FILE *arq;

    if(passiveMode == 0)
        dataCon = iniciarConexaoDados(cliente, port, ipCliente);
    else{
        statusPassive = statusThread();
        if(statusPassive == 0){
            printf("LS servidor nao recebeu conexao dados do modo passivo\n");
            strcpy(msgEnvia, "450 erro ao receber conexao de dados\n");
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
            return 0;
        }else
            dataCon = retornarDataCon();
        //dataCon = conexaoModoPassivo(cliente, port);
    }
    if(dataCon == 0){
        return 0;
    }else{
        /*system("ls >temp.txt");
        system("iconv temp.txt --to-code ASCII >temp2.txt");
        stat("temp2.txt", &obj);
        tamanho = obj.st_size;
        //write(cliente, &tamanho, sizeof(int));
        //arquivo = open("temp2.txt", O_RDONLY);
        arquivo = readFileBytes("temp2.txt");
        for(i=0; i<tamanho; i++){
            write(dataCon, arquivo[i], 1);
        }
        //sendfile(dataCon, arquivo, NULL, tamanho);
        //write(dataCon, arquivo, tamanho);
        */
        while ((lsdir = readdir(dir)) != NULL){
            strcat(enviar, lsdir->d_name);
            strcat(enviar, "  ||  ");
        }
        closedir(dir);
        //printf("ListaPasta: %s\n", enviar);
        write(dataCon, enviar, strlen(enviar)+1);
        free(enviar);
        free(lsdir);
        close(dataCon);
        strcpy(msgEnvia, "250 Arquivo enviado\n");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        printf("LIST enviado\n");
        //system("rm temp.txt");
        //system("rm temp2.txt");
        return 1;
    }
}

int opPasv(int cliente, int porta, char ipCliente[]){
    char h1[4], h2[4], h3[4], h4[4], p1[7], p2[7];
    int i, j=0, z=0, p1int, p2int;
    char msgEnviar[100];
    int port = 0;
    struct argus *args;
    pthread_t t;
    int dataCon;
    char* ipEnvia;
    while(port < 1023)
        port = rand() % 65000;
    printf("PORTA RANDOMICA: %i\n", port);
    //-------------------------------------------------------------
    args = malloc(sizeof(struct argus));
    args->port = port;
    pthread_create(&t, NULL, chamadaThreadData, (void *)&args);
    /*if((pthread_create(&t, NULL, chamadaThreadData, (void *)&args)) == 0)){
        printf("PASV erro ao criar thread de conexao de dados!\n");
        return 0;
    }else
        printf("THread criada!\n");
    *///-------------------------------------------------------------

    for(i=0; i<strlen(ipCliente); i++){
        if(ipCliente[i] == '.'){
            j++;
            z=0;
        }else{
            switch(j){
                case 0:
                    h1[z] = ipCliente[i];
                    break;
                case 1:
                    h2[z] = ipCliente[i];
                    break;
                case 2:
                    h3[z] = ipCliente[i];
                    break;
                case 3:
                    h4[z] = ipCliente[i];
                    break;
            }
            z++;
        }
    }

    //printf("Verificacao: \nH1: %s\nH2: %s\nH3: %s\nH4: %s\n------\n", h1,h2,h3,h4);
    p1int = calcPortPASV(port, 0);
    p2int = calcPortPASV(port, 1);

    sprintf(p1, "%d", p1int);
    sprintf(p2, "%d", p2int);

    if((strlen(h1)) > 3)
        for(i=3; i<strlen(h1); i++)
            h1[i] = '\0';
    if((strlen(h2)) > 3)
        for(i=3; i<strlen(h2); i++)
            h2[i] = '\0';
    if((strlen(h3)) > 3)
        for(i=3; i<strlen(h3); i++)
            h3[i] = '\0';
    if((strlen(h4)) > 3)
        for(i=3; i<strlen(h4); i++)
            h4[i] = '\0';

    for(i=0; i<3; i++){
        if((h1[i] != '0') && (h1[i] != '1') && (h1[i] != '2') && (h1[i] != '3') && (h1[i] != '4') && (h1[i] != '5') && (h1[i] != '6') && (h1[i] != '7') && (h1[i] != '8') && (h1[i] != '9'))
            h1[i] = '\0';
        if((h2[i] != '0') && (h2[i] != '1') && (h2[i] != '2') && (h2[i] != '3') && (h2[i] != '4') && (h2[i] != '5') && (h2[i] != '6') && (h2[i] != '7') && (h2[i] != '8') && (h2[i] != '9'))
            h2[i] = '\0';
        if((h3[i] != '0') && (h3[i] != '1') && (h3[i] != '2') && (h3[i] != '3') && (h3[i] != '4') && (h3[i] != '5') && (h3[i] != '6') && (h3[i] != '7') && (h3[i] != '8') && (h3[i] != '9'))
            h3[i] = '\0';
        if((h4[i] != '0') && (h4[i] != '1') && (h4[i] != '2') && (h4[i] != '3') && (h4[i] != '4') && (h4[i] != '5') && (h4[i] != '6') && (h4[i] != '7') && (h4[i] != '8') && (h4[i] != '9'))
            h4[i] = '\0';
    }
    //h3[1] = '\0';
    strcpy(ipCliente, "127.0.0.1");
    ipEnvia = ipVarPasv(ipCliente);
    //printf("h1: %s\nh2: %s\nh3: %s\nh4: %s\np1: %s\np2: %s\n", h1, h2, h3, h4, p1, p2);
    strcpy(msgEnviar, "227 entrando em modo passivo (");
    /*strcat(msgEnviar, h1);
    strcat(msgEnviar, ",");
    strcat(msgEnviar, h2);
    strcat(msgEnviar, ",");
    strcat(msgEnviar, h3);
    strcat(msgEnviar, ",");
    strcat(msgEnviar, h4);*/
    strcat(msgEnviar, ipEnvia);
    strcat(msgEnviar, ",");
    strcat(msgEnviar, p1);
    strcat(msgEnviar, ",");
    strcat(msgEnviar, p2);
    strcat(msgEnviar, ").\n");
    //strcat(msgEnviar, "\0");
    printf("PASV thread esperando conexao!\n");
    printf("MENSAGEM ENVIAR: %s\n", msgEnviar);
    write(cliente, msgEnviar, strlen(msgEnviar)+1);
    return port;
}

void *chamadaThreadData(void *arg){
    struct argus *args = arg;
    printf("PASV thread dados iniciada\n");
    conexaoModoPassivo(args->port);
}

int opCwd(int cliente, int status, char pasta[]){
    char msgEnvia[100];
    printf("CWD solicitado\n");
    if(status == 0){
        strcpy(msgEnvia, "250 diretorio acessado com sucesso\n");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        printf("CWD diretório \'%s\' acessado\n", pasta);
        return 1;
    }else{
        strcpy(msgEnvia, "550 diretorio nao pode ser acessado\n");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        printf("CWD diretório \'%s\' inexistente\n", pasta);
        return 0;
    }
}

char* opCwdPonto(int cliente, char pasta[]){
    char msgEnvia[100];
    int status;
    char *new;
    printf("CDUP solicitado\n");
    new = rPasta(pasta);
    //printf("NEW: %s\n", new);
    strcpy(msgEnvia, "200 diretorio alterado\n");
    write(cliente, msgEnvia, strlen(msgEnvia)+1);
    printf("CDUP diretório anterior acessado\n");

    return new;
    /*}else{
        strcpy(msgEnvia, "550 erro ao acessar diretorio\n");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        printf("CDUP erro ao acessar diretório anterior\n");
        return '\0';
    }*/
}

int opPut(int cliente, char nomeArquivo[], char ipCliente[], int port, int passiveMode, int *maxTaxa){
    printf("PUT aguardando arquivo\n");
    int dataCon;
    int tamanho, fileh, i, j, z;
    ssize_t tamT;
    char *arquivo = NULL;
    char msgEnvia[100];
    char temp[100];
    struct vetChar *vet;
    int tamArq;
    int statusPassive;
    if(passiveMode == 0)
        dataCon = iniciarConexaoDados(cliente, port, ipCliente);
    else{
        statusPassive = statusThread();
        if(statusPassive == 0){
            printf("LS servidor nao recebeu conexao dados do modo passivo\n");
            strcpy(msgEnvia, "450 erro ao receber conexao de dados\n");
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
            return 0;
        }else
            dataCon = retornarDataCon();
        //dataCon = conexaoModoPassivo(cliente, port);
    }

    if(dataCon == 0){
        printf("ERRO ao abrir conexao de dados, PUT finalizado\n");
        return 0;
    }

    printf("PUT recebendo arquivo \'%s\'\n", nomeArquivo);
    /*i = 1;
    tamT = read(dataCon, temp, 1);
    while(tamT != 0){
        if(arquivo == NULL)
            arquivo = (char*)malloc(sizeof(char)*8);
        else
            arquivo = realloc(arquivo, sizeof(char)*8*i);
        z = 0;
        for(j=(8*(i-1)); j<(8*i); i++){
            arquivo[j] = temp[z];
            z++;
        }
        i++;
        tamT = read(dataCon, temp, 1);
    }
    printf("Recebeu!\n");
    tamArq = 8*(i-1);
    fileh = open(nomeArquivo, O_CREAT | O_EXCL | O_WRONLY, 0666);
    for(i=0; i<tamArq; i++){
        write(fileh, arquivo[i], sizeof(char), 0);
    }
    close(fileh);*/
    //read(cliente, &tamanho, sizeof(int));
    //arquivo = malloc(sizeof(char)*8);
    /*
    vet = malloc(sizeof(struct vetChar));
    int n;
    i=1;
    while(1){
        n = read(dataCon, vet[i-1].byte, maxTaxa);
        printf("Recebeu %i bytes\n", n);
        //printf("Leu: %s\n", vet[i-1].byte);
        if(n <= 0 || n == -1)
            break;
        i++;
        vet = realloc(vet, sizeof(struct vetChar)*i);
        //sleep(1);
    }
    printf("Recebeu!\n");
    tamArq = 8*(i-1);
    fileh = open(nomeArquivo, O_CREAT | O_EXCL | O_WRONLY, 0666);
    for(i=0; i<tamArq; i++){
        write(fileh, vet[i].byte, sizeof(char)*8, 0);
    }
    close(fileh);
    /*read(dataCon, temp, sizeof(char)*8);
    printf("Recebeu: %s\n", temp);
    i=1;
    while((strcmp(temp, "00000010")) != 0){
        printf("Mandou %i\n", i);
        strcpy(arquivo[i-1], temp);
        i++;
        arquivo = realloc(NULL, sizeof(char)*8*i);
        read(dataCon, temp, 100);
    }

    */

    //read(cliente, arquivo, tamanho);
    /*tamanho = 0;
    ioctl(dataCon, FIONREAD, &tamanho);
    read(dataCon, arquivo, tamanho);
    */

    /*fileh = open(nomeArquivo, O_CREAT | O_EXCL | O_WRONLY, 0666);
    write(fileh, arquivo, tamanho, 0);
    close(fileh);*/

    FILE *received_file;
    received_file = fopen(nomeArquivo, "w+");
    int len, tam = *maxTaxa;
    printf("TAMANHO: %i\n", tam);
    char *buffer;
    buffer = (char*)malloc(sizeof(char)*tam);
    printf("Iniciando recebimento!\n");
    while((len=recv(dataCon, buffer, *maxTaxa, 0) > 0)){
        printf("Recebeu!\n");
        fwrite(buffer, sizeof(char), len, received_file);
        printf("Receive %i bytes\n", len);
        sleep(1);
    }
    fclose(received_file);
    close(dataCon);

    strcpy(msgEnvia, "250 arquivo recebido\n");
    write(cliente, msgEnvia, strlen(msgEnvia)+1);
    printf("PUT arquivo recebido\n");
    return 1;
}

int opGet(int cliente, char ipCliente[], int port, char nomeArquivo[], int passiveMode, int *maxTaxa){
    printf("GET solicitado\n");
    char msgEnvia[100];
    struct stat obj;
    int tamanho, i;
    int dataCon;
    int arquivo;
    int statusPassive;
    //char *arquivo;
    int nBytes = 0;

    //printf("Entrou GET, taxa: %i\n", maxTaxa);
    if(fopen(nomeArquivo, "r+") == NULL){
        strcpy(msgEnvia, "550 Arquivo inexistente\n");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        printf("GET status = \'%s\' enviado\n", msgEnvia);
        printf("GET finalizado\n");
        return 0;
    }

    if(passiveMode == 0)
        dataCon = iniciarConexaoDados(cliente, port, ipCliente);
    else{
        statusPassive = statusThread();
        if(statusPassive == 0){
            printf("LS servidor nao recebeu conexao dados do modo passivo\n");
            strcpy(msgEnvia, "450 erro ao receber conexao de dados\n");
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
            return 0;
        }else
            dataCon = retornarDataCon();
        //dataCon = conexaoModoPassivo(cliente, port);
    }

    if(dataCon == 0){
        printf("ERRO na conexao de dados, GET finalizado\n");
        return 0;
    }

    printf("GET enviando arquivo \'%s\'\n", nomeArquivo);
    stat(nomeArquivo, &obj);

    arquivo = open(nomeArquivo, O_RDONLY);
    //arquivo = readFileBytes(nomeArquivo);
    tamanho = obj.st_size;

    if(arquivo == 1)
        tamanho = 0;
    //write(dataCon, &tamanho, sizeof(int));
    int sent_bytes = 0;
    int offset = 0;
    /*if(tamanho){
        sendfile(dataCon, arquivo, NULL, tamanho);
    }*/
    //printf("Bytes enviados: %i\n", sent_bytes);
    printf("GET enviando a taxa de %.2f Bytes por segundo\n", *maxTaxa);
    while((sent_bytes = sendfile(dataCon, arquivo, NULL, *maxTaxa)) > 0){
        printf("Bytes enviados: %i\n", sent_bytes);
        printf("Enviou");
        sleep(1);
    }
    /*
    tamanho = strlen(arquivo);

    i=0;

    while(i < tamanho){
        if(nBytes >= maxTaxa){
            printf("Esperou!\n");
            sleep(1);
            nBytes = 0;
        }
        //printf("%c\n", arquivo[i]);
        sendfile(dataCon, &arquivo[i], NULL, sizeof(char));
        printf("Enviou!\n");
        nBytes++;
        i++;
    }*/
    printf("GET arquivo enviado\n");
    strcpy(msgEnvia, "226 Arquivo enviado com sucesso\n");
    write(cliente, msgEnvia, strlen(msgEnvia)+1);
    close(dataCon);
    return 1;
}



int opPwd(int cliente, char endereco[]){
    printf("PWD solicitado\n");
    int i;
    //char endereco[100];
    char msgEnvia[100];


    strcpy(msgEnvia, "257 ");
    strcat(msgEnvia, endereco);
    strcat(msgEnvia, "\n");
    write(cliente, msgEnvia, strlen(msgEnvia)+1);

    printf("PWD enviado\n");
    return 1;
}

int opRmd(int cliente, char pasta[]){
    printf("RMD solicitado\n");
    char msgEnvia[100];

    printf("RMD pasta \'%s\'\n", pasta);
        if(rmdir(pasta) == 0){
            printf("RMD pasta \'%s\' removida com sucesso\n", pasta);
            strcpy(msgEnvia, "250 pasta removida com sucesso\n"); // confirmação de exclusão
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
        }else{
            printf("RMD pasta \'%s\' não pode ser removida\n", pasta);
            strcpy(msgEnvia, "550 pasta nao pode ser removida\n");
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
            return 0;
        }
    return 1;
}

int opMkd(int cliente, char pasta[]){
    printf("MKD solicitado\n");
    char msgEnvia[100];

    printf("MKD pasta \'%s\'\n", pasta);

    if(mkdir(pasta, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0){
        printf("MKD pasta \'%s\' criada com sucesso\n", pasta);
        strcpy(msgEnvia, "257 pasta criada com sucesso\n");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        return 1;
    }else{
        printf("MKD erro ao criar pasta \'%s\'\n", pasta);
        strcpy(msgEnvia, "550 pasta nao pode ser criada\n");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        return 0;
    }
}

int opDele(int cliente, char arquivo[]){
    printf("DELE solicitado\n");
    char msgEnvia[100];
    char confirm[5];

    printf("DELE arquivo \'%s\'\n", arquivo);

    if(remove(arquivo) == 0){
        printf("DELE arquivo \'%s\' deletado com sucesso\n", arquivo);
        strcpy(msgEnvia, "250 Deletado com sucesso\n");   // confirmação de apagado
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        return 1;
    }else{
        printf("DELE erro ao deletar arquivo \'%s\'\n", arquivo);
        strcpy(msgEnvia, "550 Arquivo nao pode ser deletado\n");   // confirmação de erro
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        return 0;
    }
}
