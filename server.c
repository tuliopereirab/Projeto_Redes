#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>

#include <time.h>

#include <pthread.h>

#define PORTA 8080
#define MAXBUF 100
#define taxaPADRAO 100000

int entrarPasta(int nomePasta[]);
void conversa(int cliente, int idCliente, int numThread, int passiveMode, int statusLogin, char ipCliente[], int idTaxa);
int finalizarConexao();

struct argumentos{
    int cliente, idCliente, passiveMode, statusLogin;
    int posicaoControle;
    char ipCliente[INET_ADDRSTRLEN];
    int idTaxa;
};
struct _taxas{
    int status;
    int cOnline;
    char ip[INET_ADDRSTRLEN];
    int maxTaxa;
    int taxaAtual;
};


// ------------------
// EXTERNAS
int pegarIdCliente(char nome[]);
int carregarClientes();
int logar(char nome[], char senha[]);
void finalizarSessao();
int opLs(int cliente, int port, char ipCliente[], int passiveMode, char pasta[],int maxTaxa);
int opPasv(int cliente, int porta, char ipCliente[]);
void opQuit(int cliente, int idCliente, char ipCliente[]);
int opCwd(int cliente, int status, char pasta[]);
char* opCwdPonto(int cliente, char pasta[]);
int opPut(int cliente, char nomeArquivo[], char ipCliente[], int port, int passiveMode,int *maxTaxa);
int opGet(int cliente, char ipCliente[], int port, char nomeArquivo[], int passiveMode,int *maxTaxa);
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
char* verificarString(char pasta[]);
int adicionarArquivo(int taxa, char ip[]);
int lerArquivoTaxas();
int verificaIp(char ip[]);
//-------------------
// INTERNAS
void encontrarComando(char msg[]);
void encontrarParametro(char msg[]);
int opUser(int cliente, char usernameCliente[]);
int entraPasta(char nomePasta[]);
int finalizarConexao();
void loopErro();
void *inicioThread(void *argumentos);
int server(int maxTaxa);
void *controlarTaxas();

int busca_ip(char ip[]);
int regenciaAdd_ip(char ip[]);
int criar_ip(char ip[]);
int adicionar_ip(char ip[], int id);
int zerar_ip(int id);
int acharVazio_ip();
//-------------------

// gerenciamento de numero de pastas
int taxaSetada;
//int taxaPorCliente;
int nClientesAtivos=0;
int controleEscrita=0;
int taxaUtilizada=0;
int taxaDisponivel=0;
int *clientesAtivos;
int *idsClientesAtivos;
struct _taxas *controleTaxas = NULL;
int valIps=0;
int controleAtualiza = 1;
int controleEscritaStruct = 0;
//---------------------------------


int *controleThread=NULL;        // verificar commit versão 7.1
int statusServidor = 0;
char parametro[20];
char comando[5];
int port = PORTA+1; // PORT = PORTA DADOS; PORTA = PORTA CONTROLE
int s;
struct sockaddr_in client;
int addrlen;
char *myIp;



int server(int maxTaxa){
    pthread_t *t=NULL, *controleTaxa;
    int nThreadsOn=1;
    struct argumentos args;
    int idTaxas;
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
    int taxaCliente;

    taxaSetada = maxTaxa;            // recebe os valores de taxas
    taxaDisponivel = maxTaxa;         // recebe os valores de taxas


    statusPastaRaiz = chdir("pastasClientes");
    if(statusPastaRaiz != 0){
        printf("Erro ao abrir pasta raiz.\n");
        return 0;
    }




    statusClientes = carregarClientes();
    pthread_create(&controleTaxa, NULL, controlarTaxas, NULL);

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
    myIp = getMyIp();
    printf("--------------------------------------------------------------\n");
    printf("Servidor ONLINE\n");
    printf("Meu IP: %s\n", myIp);
    printf("--------------------------------------------------------------\n");

    while(1){
        client_s = accept(s, (struct sockaddr *)&client, &addrlen);
        //----------------
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&client;
        struct in_addr ipAddr = pV4Addr->sin_addr;
        inet_ntop(AF_INET, &ipAddr, ipCliente, INET_ADDRSTRLEN);
        //--------------------
        printf("Ip Cliente: %s\n", ipCliente);
        printf("Conexão solicitada\n");
        idTaxas = busca_ip(ipCliente);
        printf("Retornou %i\n", idTaxas);
        if(idTaxas == -1){
            idTaxas = regenciaAdd_ip(ipCliente);
        }
        printf("Taxa de transferencia do cliente %s setada para %i\n", ipCliente, controleTaxas[idTaxas].maxTaxa);
        args.idCliente = idCliente;
        args.cliente = client_s;
        args.passiveMode = passiveMode;
        args.statusLogin = statusLogin;
        args.idTaxa = idTaxas;
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
        //strcpy(ip[threadDisp].ipCliente, ipCliente);
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
    char intIpCliente[INET_ADDRSTRLEN];
    strcpy(intIpCliente, args->ipCliente);
    char msgEnvia[100];
    strcpy(msgEnvia, "220 Servico pronto\n");
    write(args->cliente, msgEnvia, strlen(msgEnvia));
    printf("THREAD criando thread para cliente IP %s\n", intIpCliente);
    conversa(args->cliente, args->idCliente, args->posicaoControle, args->passiveMode, args->statusLogin, intIpCliente, args->idTaxa);
}

void *controlarTaxas(){
    printf("Thread de controle iniciada!\n");
    int i;
    while(1){
        if(valIps != 0){
                for(i=0; i<valIps; i++){
                    if(controleTaxas[i].cOnline != 0){
                        controleTaxas[i].taxaAtual = controleTaxas[i].maxTaxa/controleTaxas[i].cOnline;
                    }else{
                        controleTaxas[i].taxaAtual = 0;
                    }
                    printf("Taxa atual cliente %s: %i\n", controleTaxas[i].ip, controleTaxas[i].taxaAtual);
                }

        }
        controleAtualiza = 1;
        //printf("Atualizou!\n");
        sleep(1);
    }

}

void conversa(int cliente, int idCliente, int numThread, int passiveMode, int statusLogin, char ipCliente[], int idTaxa){
    int op = 88;
    int status;
    int i, j;
    char msgRecebe[100], msgEnvia[100];
    int codigoEnvia;
    char usernameCliente[20];
    char pastaAtual[100], pasta1[150];
    char* auxPasta;
    int statusPasta;
    int pasta=0;
    int taxaPorCliente;
    strcpy(pastaAtual, "");
    pastaAtual[0] = '\0';
    // --------------------------------
    while(controleEscrita != 0){}
    controleEscrita = 1;
    nClientesAtivos++;
    controleEscrita = 0;
    // --------------------------------
    do{
        printf("------------\n");
        // ---------------------------------
        //printf("Taxa de transferencia por cliente: %i bytes/seg\n", taxaPorCliente);
        // ---------------------------------
        printf("ipCliente: %s\n", ipCliente);


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
                    op = 10;
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
                statusPasta = vPasta(pastaAtual);
                if(statusPasta == 0){
                    controleTaxas[idTaxa].cOnline++;
                    status = opLs(cliente, port, ipCliente, passiveMode, pastaAtual, &controleTaxas[idTaxa].taxaAtual);
                    controleTaxas[idTaxa].cOnline--;
                }else{
                    printf("LIST pasta invalida, impossível continuar executando este cliente\n");
                    strcpy(msgEnvia, "450 erro ao acessar a pasta, recomendavel utilizar 'quit' e conectar novamente.\n");
                    write(cliente, msgEnvia, strlen(msgEnvia)+1);
                }
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
                //printf("Pasta 1: %i\n", pasta);
                if(pasta > 0){
                    pasta--;
                    auxPasta = opCwdPonto(cliente, pastaAtual);
                    auxPasta = verificarString(auxPasta);
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
                statusPasta = vPasta(pastaAtual);
                if(statusPasta == 0){
                    auxPasta = aPasta(pastaAtual, parametro);
                    if(controleTaxas[idTaxa].cOnline != 0){
                        while(controleEscritaStruct != 0){}
                        controleEscritaStruct = 1;       // indica que comecou a escreve na struct
                        controleTaxas[idTaxa].cOnline++;    // escreveu na struct
                        controleEscritaStruct = 0;       // indica que acabou de escrever na struct
                        controleAtualiza = 0;         // indica que precisa de uma atualização da taxa de envio atual
                        while(controleAtualiza == 0) {}       // aguarda enquanto a thread de atualização não atualiza o valor de taxa atual
                        status = opPut(cliente, auxPasta, ipCliente, port, passiveMode, &controleTaxas[idTaxa].taxaAtual);
                        while(controleEscritaStruct != 0){}
                        controleEscritaStruct = 1;
                        controleTaxas[idTaxa].cOnline--;
                        controleEscritaStruct = 0;
                    }else{
                        if(controleTaxas[idTaxa].maxTaxa <= taxaDisponivel){
                            while(controleEscrita != 0) {}
                            controleEscrita = 1;
                            taxaDisponivel = taxaDisponivel - controleTaxas[idTaxa].maxTaxa;
                            printf("Atualizou Taxa disponivel 1: %i\n", taxaDisponivel);
                            controleEscrita = 0;
                            printf("Taxa disponivel: %i\n", taxaDisponivel);
                            while(controleEscritaStruct != 0){}
                            controleEscritaStruct = 1;
                            controleTaxas[idTaxa].cOnline++;
                            controleEscritaStruct = 0;
                            controleAtualiza = 0;
                            while(controleAtualiza == 0) {}
                            status = opPut(cliente, auxPasta, ipCliente, port, passiveMode, &controleTaxas[idTaxa].taxaAtual);
                            while(controleEscritaStruct != 0) {}
                            controleEscritaStruct = 1;
                            controleTaxas[idTaxa].cOnline--;
                            controleEscritaStruct = 0;
                            if(controleTaxas[idTaxa].cOnline == 0){
                                while(controleEscrita != 0) {}
                                controleEscrita = 1;
                                taxaDisponivel = taxaDisponivel + controleTaxas[idTaxa].maxTaxa;
                                printf("Atualizou Taxa disponivel 2: %i\n", taxaDisponivel);
                                controleEscrita = 0;
                            }
                        }else{
                            printf("GET largura de banda do servidor excedida, get cancelado.\n");
                            strcpy(msgEnvia, "450 largura de banda do servidor excedida, GET cancelado.\n");
                            write(cliente, msgEnvia, strlen(msgEnvia)+1);
                        }
                    }
                }else{
                    printf("PUT pasta invalida, impossível continuar executando este cliente\n");
                    strcpy(msgEnvia, "450 erro ao acessar a pasta\n");
                    write(cliente, msgEnvia, strlen(msgEnvia)+1);
                }
                break;
            case 11:
                statusPasta = vPasta(pastaAtual);
                if(statusPasta == 0){
                    auxPasta = aPasta(pastaAtual, parametro);
                    if(controleTaxas[idTaxa].cOnline != 0){
                        while(controleEscritaStruct != 0){}
                        controleEscritaStruct = 1;       // indica que comecou a escreve na struct
                        controleTaxas[idTaxa].cOnline++;    // escreveu na struct
                        controleEscritaStruct = 0;       // indica que acabou de escrever na struct
                        controleAtualiza = 0;         // indica que precisa de uma atualização da taxa de envio atual
                        while(controleAtualiza == 0) {}       // aguarda enquanto a thread de atualização não atualiza o valor de taxa atual
                        status = opGet(cliente, ipCliente, port, auxPasta, passiveMode, &controleTaxas[idTaxa].taxaAtual);
                        while(controleEscritaStruct != 0){}
                        controleEscritaStruct = 1;
                        controleTaxas[idTaxa].cOnline--;
                        controleEscritaStruct = 0;
                    }else{
                        if(controleTaxas[idTaxa].maxTaxa <= taxaDisponivel){
                            while(controleEscrita != 0) {}
                            controleEscrita = 1;
                            taxaDisponivel = taxaDisponivel - controleTaxas[idTaxa].maxTaxa;
                            printf("Atualizou Taxa disponivel 1: %i\n", taxaDisponivel);
                            controleEscrita = 0;
                            printf("Taxa disponivel: %i\n", taxaDisponivel);
                            while(controleEscritaStruct != 0){}
                            controleEscritaStruct = 1;
                            controleTaxas[idTaxa].cOnline++;
                            controleEscritaStruct = 0;
                            controleAtualiza = 0;
                            while(controleAtualiza == 0) {}
                            status = opGet(cliente, ipCliente, port, auxPasta, passiveMode, &controleTaxas[idTaxa].taxaAtual);
                            while(controleEscritaStruct != 0) {}
                            controleEscritaStruct = 1;
                            controleTaxas[idTaxa].cOnline--;
                            controleEscritaStruct = 0;
                            if(controleTaxas[idTaxa].cOnline == 0){
                                while(controleEscrita != 0) {}
                                controleEscrita = 1;
                                taxaDisponivel = taxaDisponivel + controleTaxas[idTaxa].maxTaxa;
                                printf("Atualizou Taxa disponivel 2: %i\n", taxaDisponivel);
                                controleEscrita = 0;
                            }
                        }else{
                            printf("GET largura de banda do servidor excedida, get cancelado.\n");
                            strcpy(msgEnvia, "450 largura de banda do servidor excedida, GET cancelado.\n");
                            write(cliente, msgEnvia, strlen(msgEnvia)+1);
                        }
                    }
                }else{
                    printf("GET pasta invalida, impossível continuar executando este cliente\n");
                    strcpy(msgEnvia, "450 erro ao acessar a pasta\n");
                    write(cliente, msgEnvia, strlen(msgEnvia)+1);
                }
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
                //port = opPasv(cliente, PORTA, ipCliente);
                port = opPasv(cliente, PORTA, myIp);
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
    controleThread[numThread] = 0;
    // -------------------------------
    //while(controleEscrita != 0){}         // aguarda qualquer outra função acabar de escrever
    //taxaDisponivel = taxaDisponivel + taxaPorCliente;     // avisa ao servidor que a taxa está disponível para ser utilizada
    //controleEscrita = 1;                  // diz que irá começar a escrveer
    nClientesAtivos--;                    // atualiza o valor
    //controleEscrita=0;                    // diz qeu acabou de escrever
    //-----------------------------------
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

char* verificarString(char pasta[]){
    int i, tam, nPastas, z=0;
    char *new, *new2;
    tam = strlen(pasta);
    nPastas = contarPastas(pasta)-1;
    //printf("Inicial: %s, %i\n", pasta, nPastas);
    new = (char*)malloc(sizeof(char)*tam);
    new2 = (char*)malloc(sizeof(char)*tam);
    for(i=0; i<tam; i++){
        if((pasta[i] == '/') && (z==(nPastas-1))){
            new[i] = '\0';
            return new;
        }
        else if((pasta[i] == '/') && (z != (nPastas-1))){
            z++;
            //printf("Barra!\n");
            new[i] = pasta[i];
        }else if(isascii((int)pasta[i]) != 0)
            new[i] = pasta[i];
        //printf("%i: %s\n", i, new);
    }
    strcpy(new2, new);
    return new2;
}

int regenciaAdd_ip(char ip[]){
    int id;
    printf("Entrou regencia!\n");
    id = acharVazio_ip();
    printf("Entrou regencia 2!\n");
    if(id == -1){
        printf("Entrou regencia3!\n");
        id = criar_ip(ip);
        printf("Entrou regencia4!\n");
    }else{
        id = adicionar_ip(ip, id);
    }
    return id;
}
int busca_ip(char ip[]){
    int i;
    printf("Chegou aqui!\n");
    if(valIps == 0){
        return -1;           // -1 mostra que o ip ainda não está no vetor
    }else{
        printf("Entra aqui\n");
        for(i=0;i<valIps;i++)
            if(controleTaxas[i].status == 1)
                if((strcmp(controleTaxas[i].ip, ip)) == 0)
                    return i;        // retorna o valor do id a ser usado em 'controleTaxas[i].ip'
        return -1;      // -1 mostra que o ip ainda não está no vetor
    }
}
int criar_ip(char ip[]){
    int taxaCliente;
    taxaCliente = verificaIp(ip);
    printf("Taxa criar: %i\n", taxaCliente);
    if(taxaCliente == -1)
        taxaCliente = taxaPADRAO;
    if(valIps == 0){
        controleTaxas = (struct _taxas*)malloc(sizeof(struct _taxas));
        valIps++;
    }else{
        valIps++;
        controleTaxas = realloc(controleTaxas, sizeof(struct _taxas)*valIps);
    }
    controleTaxas[valIps-1].status = 1;
    strcpy(controleTaxas[valIps-1].ip, ip);
    controleTaxas[valIps-1].maxTaxa = taxaCliente;
    controleTaxas[valIps-1].taxaAtual = 0;
    controleTaxas[valIps-1].cOnline = 0;
    printf("AQUI: %i\n", controleTaxas[valIps-1].cOnline);
    return valIps-1;      // retorna o id
}

int adicionar_ip(char ip[], int id){
    int taxaCliente = verificaIp(ip);
    controleTaxas[id].status = 1;
    strcpy(controleTaxas[id].ip, ip);
    controleTaxas[id].maxTaxa = taxaCliente;
    controleTaxas[id].taxaAtual = 0;
    controleTaxas[id].cOnline = 0;
    return id;    // retorna o id
}



int zerar_ip(int id){
    controleTaxas[id].status = 0;
    return 1;
}

int acharVazio_ip(){
    int i;
    if(valIps == 0)
        return -1;
    for(i=0; i<valIps; i++)
        if(controleTaxas[i].status == 0)
            return i;     // retorna o valor da posição vazia
    return -1;     // retorna que não existe posição vazia
}
