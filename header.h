#include <stdio.h>
#include "server.c"
#include "func.c"
#include "login.c"
#include "connection.c"

int carregarClientes();
void finalizarSessao();
int logar(char nome[], char senha[]);
int opLs(int cliente, int port, int port2, char ipCliente[]);
int opUser(int cliente);
void opQuit(int cliente);
int opCwd(int cliente, char pasta[]);
int opCwdPonto(int cliente);
int opPut(int cliente);
int opGet(int cliente, char ipCliente[], int port, int port2, char nomeArquivo[]);
int opPwd(int cliente);
int opRmd(int cliente, char pasta[]);
int opMkd(int cliente, char pasta[]);
int opDele(int cliente, char arquivo[]);
int iniciarConexaoDados(int cliente, int port, int port2, char ipCliente[]);
int opPort(char portas[], int set);
void loopErro();
