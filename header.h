#include <stdio.h>
#include "server.c"
#include "func.c"
#include "login.c"
#include "connection.c"
#include "porta.c"

int carregarClientes();
void finalizarSessao();
int logar(char nome[], char senha[]);
int opLs(int cliente, int port, char ipCliente[]);
int opUser(int cliente);
void opQuit(int cliente);
int opCwd(int cliente, char pasta[]);
int opCwdPonto(int cliente);
int opPut(int cliente, char nomeArquivo[], char ipCliente[], int port);
int opGet(int cliente, char ipCliente[], int port, char nomeArquivo[]);
int opPwd(int cliente);
int opRmd(int cliente, char pasta[]);
int opMkd(int cliente, char pasta[]);
int opDele(int cliente, char arquivo[]);
int iniciarConexaoDados(int cliente, int port, char ipCliente[]);
int opPort(char portas[]);
int calcPort(int val1, int val2);
int calcPortPASV(int val, int set);
void loopErro();
