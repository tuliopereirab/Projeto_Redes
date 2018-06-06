#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>

#include <pthread.h>

#define PORTA 8080
#define MAXBUF 100

int entrarPasta(int nomePasta[]);
void conversa(int cliente, int idCliente, char ipCliente[], int passiveMode, int statusLogin);
int finalizarConexao();

struct argumentos{
    int cliente, idCliente, passiveMode, statusLogin;
    char ipCliente[INET_ADDRSTRLEN];
    int posicaoControle;
};


// ------------------
// EXTERNAS
int carregarClientes();
int logar(char nome[], char senha[]);
void finalizarSessao();
int opLs(int cliente, int port, char ipCliente[], int passiveMode);
void opQuit(int cliente, int idCliente, char ipCliente[]);
int opCwd(int cliente, int status, char pasta[]);
char* opCwdPonto(int cliente, char pasta[]);
int opPut(int cliente, char nomeArquivo[], char ipCliente[], int port, int passiveMode);
int opGet(int cliente, char ipCliente[], int port, char nomeArquivo[], int passiveMode);
int opPwd(int cliente, char endereco[]);
int opRmd(int cliente, char pasta[]);
int opMkd(int cliente, char pasta[]);
int opDele(int cliente, char arquivo[]);
int opPort(char portas[]);
void finalizarSessao(int idCliente);
char* getMyIp();
int buscarThread(int controle[], int nThreads);
int vPasta(char pasta[]);
int cPasta(char pasta[], char nome[], int op);
int ePasta(char pasta[]);
int sairPasta(char pasta[]);
char* rPasta(char pasta[]);
char* aPasta(char pasta[], char newPasta[]);
int contarPastas(char pasta[]);
//-------------------
// INTERNAS
void encontrarComando(char msg[]);
void encontrarParametro(char msg[]);
int opUser(int cliente, char usernameCliente[]);
int entraPasta(char nomePasta[]);
int finalizarConexao();
void loopErro();
void *inicioThread(void *argumentos);
//-------------------



int *controleThread=NULL;        // verificar commit versão 7.1
int statusServidor = 0;
char parametro[20];
char comando[5];
int port = PORTA+1; // PORT = PORTA DADOS; PORTA = PORTA CONTROLE
int s;
struct sockaddr_in client;
int addrlen;
char *myIp = NULL;

int main(){
    pthread_t *t=NULL;
    int nThreadsOn=1;
    struct argumentos args;
    //----------------------
    int idCliente;
    char ipCliente[INET_ADDRSTRLEN];
    int passiveMode = 0;
    //int statusLogin=0;
    //----------------------
    char nomeCliente[20], nomePasta[50];
    int s, client_s;
    struct sockaddr_in self;
    addrlen = sizeof(client);
    int status;
    char msgEnviar[100];
    char msgRecebe[100];
    int statusClientes, statusLogin;
    char senhaCliente[15], usernameCliente[15];
    int statusFinalizar, statusPastaRaiz;
    int nThreadCriadas=1, threadDisp;


    statusPastaRaiz = chdir("pastasClientes");
    if(statusPastaRaiz != 0){
        printf("Erro ao abrir pasta raiz.\n");
        return 0;
    }




    statusClientes = carregarClientes();

    if(statusClientes == 0){
        printf("Erro ao carregar clientes\n");
        return 0;
    }

    s = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&self, sizeof(self));
    self.sin_family = AF_INET;
    self.sin_port = htons(PORTA);
    self.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&self, sizeof(self));
    listen(s, 5);
    printf("--------------------------------------------------------------\n");
    printf("Servidor ONLINE\n");
    myIp = getMyIp();
    printf("\n--------------------------------------------------------------\n");

    while(1){
        client_s = accept(s, (struct sockaddr *)&client, &addrlen);
        //----------------
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&client;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        inet_ntop(AF_INET, &ipAddr, ipCliente, INET_ADDRSTRLEN);
        //--------------------
        printf("Ip Cliente: %s\n", ipCliente);
        printf("Conexão solicitada\n");
        args.idCliente = idCliente;
        args.cliente = client_s;
        args.passiveMode = passiveMode;
        args.statusLogin = statusLogin;
        strcpy(args.ipCliente, ipCliente);
        if(t == NULL){
            t = malloc(sizeof(pthread_t));
            controleThread = malloc(sizeof(int));
            threadDisp=0;
        }
        else{
            threadDisp = buscarThread(controleThread, nThreadCriadas);
            if(threadDisp == -1){
                t = realloc(t, sizeof(pthread_t)*nThreadsOn);
                controleThread = realloc(controleThread, sizeof(int)*nThreadCriadas+1);
                threadDisp = nThreadCriadas;
                nThreadCriadas++;
            }
        }
        args.posicaoControle = threadDisp;
        if((pthread_create(&t[threadDisp], NULL, inicioThread, (void *)&args) == 0)){
            controleThread[threadDisp] = 1;
            nThreadsOn++;
        }else{
            printf("Erro ao criar threads!\n");
            loopErro();
        }
        //strcpy(msgEnviar, "220 Servico pronto\n");
        //write(client_s, msgEnviar, strlen(msgEnviar)+1);
        //conversa(client_s);
        //close(client_s);
        //printf("Conexao finalizada\n");
        //printf("--------------------------------------------------------------\n");
    }
}

void *inicioThread(void *argumentos){
    struct argumentos *args = argumentos;
    char msgEnvia[100];
    strcpy(msgEnvia, "220 Servico pronto\n");
    write(args->cliente, msgEnvia, strlen(msgEnvia));
    printf("THREAD criando thread para cliente IP %s\n", args->ipCliente);
    conversa(args->cliente, args->idCliente, args->ipCliente, args->passiveMode, args->statusLogin);
    controleThread[args->posicaoControle] = 0;
}

void conversa(int cliente, int idCliente, char ipCliente[], int passiveMode, int statusLogin){
    int op = 88;
    int status;
    int i, j;
    char msgRecebe[100], msgEnvia[100];
    int codigoEnvia;
    char usernameCliente[20];
    char pastaAtual[100], pasta1[150];
    char* auxPasta;
    int pasta=0;
    strcpy(pastaAtual, "");
    pastaAtual[0] = '\0';
    printf("ipCliente: %s\n", ipCliente);
    printf("----------__");
    do{
        printf("ipCliente: %s\n", ipCliente);
        printf("----------__");
    
        //system("pwd");
        //printf("------------------\n");
        for(i=0;i<100; i++)
            msgRecebe[i] = NULL;
        read(cliente, msgRecebe, MAXBUF);

        printf("\nRECEBE: %s\n", msgRecebe);

        if((strlen(msgRecebe)) > 4){
            encontrarComando(msgRecebe);
            encontrarParametro(msgRecebe);
        }else
            strcpy(comando, msgRecebe);

        if((strcmp(comando, "USER")) == 0)
            op = 40;
        else{
            if(statusLogin == 1){
                if((strcmp(comando, "LIST")) == 0)
                    op = 1;
                else if((strcmp(comando, "CWD")) == 0)
                    op = 4;
                else if((strcmp(comando, "CDUP")) == 0)
                    op = 5;
                else if((strcmp(comando, "PWD")) == 0)
                    op = 6;
                else if((strcmp(comando, "STOR")) == 0)
                    op = 0;
                    //op = 10;
                else if((strcmp(comando, "RETR")) == 0)
                    op = 11;
                else if((strcmp(comando, "RMD")) == 0)
                    op = 20;
                else if((strcmp(comando, "MKD")) == 0)
                    op = 21;
                else if((strcmp(comando, "DELE")) == 0)
                    op = 30;
                else if((strcmp(comando, "SYST")) == 0)
                    op=41;
                else if((strcmp(comando, "PORT")) == 0)
                    op = 50;
                else if((strcmp(comando, "PASV")) == 0)
                    op = 51;
                else if((strcmp(comando, "NOOP")) == 0)
                    op = 98;
                else if((strcmp(comando, "QUIT")) == 0)
                    op = 99;
                else
                    op = 0;
            }else
                op = 70;
        }

        switch(op){
            case 1:
                status = opLs(cliente, port, ipCliente, passiveMode);
                break;
            case 4:
                /*pasta = opCwd(cliente, parametro);
                if(status == 1)
                    pasta++;
                */
                auxPasta = aPasta(pastaAtual, parametro);
                //printf("AuxAtual: %s\n", auxPasta);
                status = vPasta(auxPasta);
                //system("pwd");
                //printf("Status: %i\n", status);
                if(status == 0){
                    pasta++;
                    strcpy(pastaAtual, auxPasta);
                }
                status = opCwd(cliente, status, parametro);
                break;
            case 5:
                printf("Pasta 1: %i\n", pasta);
                if(pasta > 0){
                    pasta--;
                    auxPasta = opCwdPonto(cliente, pastaAtual);
                    strcpy(pastaAtual, auxPasta);
                }else{
                    strcpy(msgEnvia, "550 Ja esta na pasta raiz do usuario\n");
                    write(cliente, msgEnvia, strlen(msgEnvia)+1);
                    printf("CDUP cliente tentado acessar fora da sua pasta\n");
                }
                break;
            case 6:
                status = opPwd(cliente, pastaAtual);
                break;
            case 10:
                auxPasta = aPasta(pastaAtual, auxPasta);
                status = opPut(cliente, parametro, ipCliente, port, passiveMode);
                break;
            case 11:
                auxPasta = aPasta(pastaAtual, parametro);
                status = opGet(cliente, ipCliente, port, auxPasta, passiveMode);
                break;
            case 20:
                auxPasta = aPasta(pastaAtual, parametro);
                status = opRmd(cliente, auxPasta);
                break;
            case 21:
                auxPasta = aPasta(pastaAtual, parametro);
                status = opMkd(cliente, auxPasta);
                break;
            case 30:
                auxPasta = aPasta(pastaAtual, parametro);
                status = opDele(cliente, auxPasta);
                break;
            case 40:
                strcpy(usernameCliente, parametro);
                statusLogin = opUser(cliente, usernameCliente);
                if((statusLogin == 99) || (statusLogin == 0))
                    op = 99;
                else{
                    idCliente = pegarIdCliente(usernameCliente);
                    printf("Parametro: %s\nID: %i\n", usernameCliente, idCliente);
                    printf("ID cliente: %i\n", idCliente);
                    strcpy(pasta1, "pasta_");
                    strcat(pasta1, usernameCliente);
                    strcpy(pastaAtual, pasta1);
                    //vPasta(pastaAtual);
                    pasta=0;
                }
                break;
            case 41:
                printf("SYST solicitado\n");
                strcpy(msgEnvia, "215 UNIX\n");
                printf("SYST enviado: UNIX\n");
                write(cliente, msgEnvia, strlen(msgEnvia)+1);
                break;
            case 50:
                port = opPort(parametro);
                printf("PORT porta redefida para %i\n", port);
                strcpy(msgEnvia, "200 porta redefinida\n");
                write(cliente, msgEnvia, strlen(msgEnvia)+1);
                passiveMode = 0;
                break;
            case 51:
                port = opPasv(cliente, PORTA, ipCliente);
                printf("Porta: %i\n", port);
                passiveMode = 1;
                break;
            case 70:
                strcpy(msgEnvia, "530 usuario nao logado\n");
                write(cliente, msgEnvia, strlen(msgEnvia)+1);
                break;
            case 98:
                strcpy(msgEnvia, "200 OK\n");
                write(cliente, msgEnvia, strlen(msgEnvia)+1);
                break;
            case 99:
                printf("QUIT solicitado pelo cliente\n\n");
                statusLogin = 0;
                passiveMode = 0;
                opQuit(cliente, idCliente, ipCliente);
                break;
            case 0:
                printf("ERRO, comando digitado invalido\n\n");
                strcpy(msgEnvia, "500 Comando nao reconhecido\n");
                write(cliente, msgEnvia, strlen(msgEnvia)+1);
                break;
            default:
                printf("Erro! OP setado incorretamente\n\n");

        }
    }while(op != 99);
    system(EXIT_SUCCESS);
}

int opUser(int cliente, char usernameCliente[]){
    char msgEnvia[100], msgRecebe[100];
    char senhaCliente[20];
    char nomePasta[50];
    //char usernameCliente[20];
    int status, i;
    int statusLogin;

    //strcpy(usernameCliente, parametro);
    printf("Cliente \'%s\' tentando login\n", usernameCliente);

    strcpy(msgEnvia, "331 aguardando senha\n");
    write(cliente, msgEnvia, strlen(msgEnvia)+1);
    read(cliente, msgRecebe, MAXBUF);

    encontrarComando(msgRecebe);
    if((strcmp(comando, "PASS")) == 0){
        encontrarParametro(msgRecebe);
        i = 0;
        while(parametro[i] != '\0'){
            senhaCliente[i] = parametro[i];
            i++;
        }
        senhaCliente[i] = '\0';
        //strcpy(senhaCliente, parametro);
        status = 0;
    }else{
        status = 30;
    }

    if(status == 0)
        statusLogin = logar(usernameCliente, senhaCliente);

    /*if((statusLogin == 1) || (statusLogin == 2)){
        idCliente = pegarIdCliente(usernameCliente);
        printf("ID cliente: %i\n", idCliente);
    }*/

    if(statusLogin == 1){
        status = 0;
        printf("Cliente \'%s\' logado com sucesso\n", usernameCliente);
    }else if(statusLogin == 2){
        status = 0;
        printf("Cliente \'%s\' cadastrado com sucesso\n", usernameCliente);
    }else if(statusLogin == 0){
        printf("Senha para \'%s\' incorreta\n", usernameCliente);
        status = 40;
    }else if(statusLogin == 10){
        printf("Cliente \'%s\' já está logado\n", usernameCliente);
        status = 50;
    }else{
        printf("Erro desconhecido\n");
        status = 50;
    }

    strcpy(nomePasta, "pasta_");
    strcat(nomePasta, usernameCliente);

    if(status == 0)
        status = entraPasta(nomePasta);


    if(status == 0)
        status = 1;


    if(status == 1){
        if(statusLogin == 1){
            strcpy(msgEnvia, "230 Usuario logado com sucesso\n");
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
        }else if(statusLogin == 2){
            strcpy(msgEnvia, "230 Usuario cadastrado com sucesso\n");
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
        }
        return 1;
    }else{
        if(status == 10){
            strcpy(msgEnvia, "421 erro ao criar pasta do cliente\n");
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
            printf("Finalizando conexão: erro ao criar pasta do cliente \'%s\'.\n", usernameCliente);
            return 0;
        }else if(status == 20){
            strcpy(msgEnvia, "421 erro ao criar pasta do cliente\n");
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
            printf("Finalizando conexão: erro ao acessar a pasta do cliente \'%s\'.\n", usernameCliente);
            return 0;
        }else if(status == 30){
            strcpy(msgEnvia, "500 comando esperado PASS\n");
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
            return 0;
        }else if(status == 40){
            strcpy(msgEnvia, "501 senha incorreta\n");
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
            printf("Senha incorreta!\n");
            return 0;
        }else if(status == 50){
            strcpy(msgEnvia, "501 cliente ja esta logado\n");
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
            printf("Cliente \'%s\' já está logado\n", usernameCliente);
            return 0;
        }else{
            strcpy(msgEnvia, "421 erro desconhecido\n");
            write(cliente, msgEnvia, strlen(msgEnvia)+1);
            printf("Finalizando conexão: erro desconhecido\n");
            return 99;
        }
    }
}

void encontrarComando(char msg[]){
    int i;
    char *com;
    for(i=0; i<4; i++){
        comando[i] = NULL;
        if(msg[i] != ' ')
            comando[i] = msg[i];
        else
            comando[i] = '\0';
    }
    comando[4] = '\0';
    if(comando[0] == 'P')
        if(comando[1] == 'W')
            if(comando[2] == 'D')
                comando[3] = NULL;
    printf("Comando: %s\n", comando);
}

void encontrarParametro(char msg[]){
    int i, j=0;
    int stat = 1;
    int tam = strlen(msg);

    for(i=0; i<20; i++)
        parametro[i] = NULL;

    i = strlen(comando)+1;
    while(i<(tam-2)){
        if(msg[i] != '\n')
            parametro[j] = msg[i];
        else{
            parametro[j] = NULL;
            i=tam;
        }
        i++;
        j++;
    }
    parametro[j] = '\0';

    printf("Parametro: %s\n", parametro);
}

int entraPasta(char nomePasta[]){
    int status;
    status = vPasta(nomePasta);
    if(status == 0)
        return status;
    else{
        status = cPasta("", nomePasta, 0);
        return status;
    }
}
/*
int finalizarConexao(){
    int i;
    int status=0;
    if(pasta == 0)
        return chdir("..");
    else if(pasta > 0){
        for(i=0; i<pasta; i++){
            if(status == 0)
                status = chdir("..");
        }
        if(status == 0)
            return chdir("..");
        else
            return status;
    }
}
*/
void loopErro(){
    char msgEnvia[100];
    int cliente;
    int i=0;
    strcpy(msgEnvia, "421 servidor impossibilitado para receber conexao\n");
    while(1){
        cliente = accept(s, (struct sockaddr *)&client, &addrlen);
        i++;
        printf("Tentativa de login %i! Servidor IMPOSSIBILITADO\n", i);
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        close(cliente);
    }
}
