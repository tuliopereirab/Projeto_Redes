#include <stdio.h>
#include "main.c"
#include "arquivo.c"
#include "server.c"
#include "func.c"
#include "login.c"
#include "connection.c"
#include "porta.c"
#include "apoio.c"
#include "getMyIp.c"
#include "pastas.c"
#include "help.c"
#include "gerencia.c"

// MAIN.c
int main(int argc, char **argv);
//--------------------------
// LOGIN.C
int carregarClientes();
void finalizarSessao(int idCliente);
int logar(char nome[], char senha[]);
int pegarIdCliente(char nome[]);
//--------------------------
// SERVER.C
int server(int maxTaxa);
int opUser(int cliente, char usernameCliente[]);
void loopErro();
//--------------------------
// FUNC.C
int opLs(int cliente, int port, char ipCliente[], int passiveMode, char pasta[],int maxTaxa);
void opQuit(int cliente, int idCliente, char ipCliente[]);
int opCwd(int cliente, int status, char pasta[]);
char* opCwdPonto(int cliente, char pasta[]);
int opPut(int cliente, char nomeArquivo[], char ipCliente[], int port, int passiveMode,int maxTaxa);
int opGet(int cliente, char ipCliente[], int port, char nomeArquivo[], int passiveMode,int *maxTaxa);
int opPwd(int cliente, char endereco[]);
int opRmd(int cliente, char pasta[]);
int opMkd(int cliente, char pasta[]);
int opDele(int cliente, char arquivo[]);
int opPort(char portas[]);
//--------------------------
// CONNECTION.C
int iniciarConexaoDados(int cliente, int port, char ipCliente[]);
void conexaoModoPassivo(int port);
int retornarDataCon();
int statusThread();
//--------------------------
// PORTA.C
int calcPort(int val1, int val2);
int calcPortPASV(int val, int set);
//--------------------------
// APOIO.c
char* readFileBytes(const char *name);
char* correcaoPort(char aux[]);
int buscarThread(int controle[], int nThreads);
char* ipVar(char ip[]);
char* ipVarPasv(char ip[]);
//--------------------------
// GETMYIP.C
char* getMyIp();
//--------------------------
// PASTAS.C
int vPasta(char pasta[]);
int cPasta(char pasta[], char nome[], int op);
int ePasta(char pasta[]);
int sairPasta(char pasta[]);
char* rPasta(char pasta[]);
char* aPasta(char pasta[], char newPasta[]);
int contarPastas(char pasta[]);
char* verificarString(char pasta[]);
//--------------------------
// HELP.C
void ajuda();
//--------------------------
// ARQUIVO.C
void arquivo(char maxTaxaS[]);
int lerArquivo();
//--------------------------
// GERENCIA.C
int adicionarArquivo(int taxa, char ip[]);
int lerArquivoTaxas();
int verificaIp(char ip[]);
//--------------------------
