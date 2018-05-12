#include <stdio.h>
#include "server.c"
#include "func.c"
#include "login.c"
#include "connection.c"

int carregarClientes();
int opLs(int cliente, int port, char ipCliente[]);
int opUser(int cliente);
void opQuit(int cliente);
int opDir(int cliente);
int opLsLa(int cliente);
int opCwd(int cliente, char pasta[]);
int opCwdPonto(int cliente);
int opPut(int cliente);
int opGet(int cliente);
int opPwd(int cliente);
int opRmd(int cliente, char pasta[]);
int opMkd(int cliente, char pasta[]);
int opDele(int cliente, char arquivo[]);
int iniciarConexaoDados(int cliente, int port, char ipCliente[]);
int opPort(char portas[]);
void loopErro();