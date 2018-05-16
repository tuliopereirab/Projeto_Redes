#include <stdio.h>
#include "server.c"
#include "func.c"
#include "login.c"
#include "connection.c"
#include "porta.c"
#include "apoio.c"

// LOGIN.C
int carregarClientes();
void finalizarSessao(int idCliente);
int logar(char nome[], char senha[]);
int pegarIdCliente(char nome[]);
//--------------------------
// SERVER.C
int opUser(int cliente);
void loopErro();
//--------------------------
// FUNC.C
int opLs(int cliente, int port, char ipCliente[]);
void opQuit(int cliente, int idCliente);
int opCwd(int cliente, char pasta[]);
int opCwdPonto(int cliente);
int opPut(int cliente, char nomeArquivo[], char ipCliente[], int port);
int opGet(int cliente, char ipCliente[], int port, char nomeArquivo[]);
int opPwd(int cliente);
int opRmd(int cliente, char pasta[]);
int opMkd(int cliente, char pasta[]);
int opDele(int cliente, char arquivo[]);
int opPort(char portas[]);
//--------------------------
// CONNECTION.C
int iniciarConexaoDados(int cliente, int port, char ipCliente[]);
//--------------------------
// PORTA.C
int calcPort(int val1, int val2);
int calcPortPASV(int val, int set);
//--------------------------
// APOIO.c
char* readFileBytes(const char *name);
