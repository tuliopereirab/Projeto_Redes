#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>


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
