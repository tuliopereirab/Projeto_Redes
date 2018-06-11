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

int dataCon1;
int status = 0;
int statusThread();

int iniciarConexaoDados(int cliente, int port, char ipCliente[]){
    int dataCon;
    char msgEnvia[100];
    int status;
    struct sockaddr_in dest;

    dataCon = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    inet_aton(ipCliente, &dest.sin_addr.s_addr);
    printf("CONEXAO iniciando conexao de dados com cliente %s:%i\n", ipCliente, port);
    status = connect(dataCon, (struct sockaddr *)&dest, sizeof(dest));

    if(status == 0){
        printf("CONEXAO conexao de dados estabelecida com cliente\n");
        strcpy(msgEnvia, "125 conexao de dados criada\n");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        return dataCon;
    }else{
        printf("CONEXAO erro ao iniciar conexao de dados\n");
        strcpy(msgEnvia, "450 erro ao criar conexao de dados\n");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        return 0;
    }

}

void conexaoModoPassivo(int port){
    int s;
    struct sockaddr_in self;
    int addrlen;
    struct sockaddr_in client;
    addrlen = sizeof(client);
    s = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&self, sizeof(self));
    self.sin_family = AF_INET;
    self.sin_port = htons(port);
    self.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&self, sizeof(self));
    listen(s, 5);
    printf("PASV aguardando conexão do cliente\n");
    dataCon1 = accept(s, (struct sockaddr *)&client, &addrlen);
    status = 1;
    printf("PASV cliente se conectou\n");
    pthread_exit(NULL);
    //system(EXIT_SUCCESS);
}

int retornarDataCon(){
    status = 0;
    return dataCon1;
}

int statusThread(){
    return status;
}
