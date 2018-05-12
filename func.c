#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include <fcntl.h>


#define MAXBUF 100

int iniciarConexaoDados(int cliente, int port, char ipCliente[]);
void loopErro();


void opQuit(int cliente){
    int statusFinalizar;
    char msgEnvia[100];
    strcpy(msgEnvia, "221 Finalizando conexao\n");
    write(cliente, msgEnvia, strlen(msgEnvia)+1);
    statusFinalizar = finalizarConexao();
    if(statusFinalizar != 0){
        printf("Erro ao finalizar conexão: pasta raiz não pode ser retornada\nFINALIZANDO SERVIDOR!\n");
        close(cliente);
        loopErro();
    }else{
        printf("Servidor retornado para pasta raiz\n");
    }
    printf("Finalizando conexao: pedido cliente\n");
}

int opPort(char portas[]){
    int i, j;
    int porta, tam;
    char aux[20];
    i = 0, j=0;
    printf("AQUI CHEGOU!\n: %s", portas);
    tam = strlen(portas);
    while((i<tam) && (j<4)){
        if(portas[i] == ',')
            j++;
        i++;
    }
    j=0;
    while(portas[i] != ',')
        aux[j++] = portas[i++];


    printf("aux: %s\n", aux);
    porta = atoi(aux);
    printf("Porta: %i\n", porta);
    return porta;
}

int opLs(int cliente, int port, char ipCliente[]){
    printf("LIST solicitado\n");

    struct stat obj;
    int dataCon;
    int tamanho;
    int arquivo;
    char msgEnvia[100];
    FILE *arq;

    dataCon = iniciarConexaoDados(cliente, port, ipCliente);
    if(dataCon == 0){
        return 0;
    }else{
        system("ls >temp.txt");
        stat("temp.txt", &obj);
        tamanho = obj.st_size;
        write(cliente, &tamanho, sizeof(int));
        arquivo = open("temp.txt", O_RDONLY);
        sendfile(dataCon, arquivo, NULL, tamanho);
        strcpy(msgEnvia, "250 Arquivo enviado\n");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        close(dataCon);
        printf("LIST enviado\n");
        system("rm temp.txt");
        return 1;
    }
}

int opPasv(int cliente, int port, char ipCliente[]){
    char h1[4], h2[4], h3[4], h4[4], p1[7], p2[7];
    int i, j=0, z=0;
    char msgEnviar[100];
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
    sprintf(p1, "%i", port+1);
    sprintf(p2, "%i", port+2);


    if((strlen(h1)) > 3)
        for(i=3; i<strlen(h1); i++)
            h1[i] = NULL;
    if((strlen(h2)) > 3)
        for(i=3; i<strlen(h2); i++)
            h2[i] = NULL;
    if((strlen(h3)) > 3)
        for(i=3; i<strlen(h3); i++)
            h3[i] = NULL;
    if((strlen(h4)) > 3)
        for(i=3; i<strlen(h4); i++)
            h4[i] = NULL;

    for(i=0; i<3; i++){
        if((h1[i] != '0') && (h1[i] != '1') && (h1[i] != '2') && (h1[i] != '3') && (h1[i] != '4') && (h1[i] != '5') && (h1[i] != '6') && (h1[i] != '7') && (h1[i] != '8') && (h1[i] != '9'))
            h1[i] = NULL;
        if((h2[i] != '0') && (h2[i] != '1') && (h2[i] != '2') && (h2[i] != '3') && (h2[i] != '4') && (h2[i] != '5') && (h2[i] != '6') && (h2[i] != '7') && (h2[i] != '8') && (h2[i] != '9'))
            h2[i] = NULL;
        if((h3[i] != '0') && (h3[i] != '1') && (h3[i] != '2') && (h3[i] != '3') && (h3[i] != '4') && (h3[i] != '5') && (h3[i] != '6') && (h3[i] != '7') && (h3[i] != '8') && (h3[i] != '9'))
            h3[i] = NULL;
        if((h4[i] != '0') && (h4[i] != '1') && (h4[i] != '2') && (h4[i] != '3') && (h4[i] != '4') && (h4[i] != '5') && (h4[i] != '6') && (h4[i] != '7') && (h4[i] != '8') && (h4[i] != '9'))
            h4[i] = NULL;
    }

    printf("h1: %s\nh2: %s\nh3: %s\nh4: %s\np1: %s\np2: %s\n", h1, h2, h3, h4, p1, p2);
    strcpy(msgEnviar, "227 Entrando em modo passivo (");
    strcat(msgEnviar, h1);
    strcat(msgEnviar, ",");
    strcat(msgEnviar, h2);
    strcat(msgEnviar, ",");
    strcat(msgEnviar, h3);
    strcat(msgEnviar, ",");
    strcat(msgEnviar, h4);
    strcat(msgEnviar, ",");
    strcat(msgEnviar, p1);
    strcat(msgEnviar, ",");
    strcat(msgEnviar, p2);
    strcat(msgEnviar, ")\n");

    printf("MENSAGEM ENVIAR: %s\n", msgEnviar);
    write(cliente, msgEnviar, strlen(msgEnviar)+1);

    return 1;
}

int opDir(int cliente){
    return 1;
}

int opLsLa(int cliente){
   return 1;
}

int opCwd(int cliente, char pasta[]){
    int status;
    char msgEnvia[100];
    printf("CWD solicitado\n");
    status = chdir(pasta);
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

int opCwdPonto(int cliente){
    char msgEnvia[100];
    int status;
    printf("CDUP solicitado\n");
    status = chdir("..");
    if(status == 0){
        strcpy(msgEnvia, "200 diretorio alterado\n");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        printf("CDUP diretório anterior acessado\n");
        return 1;
    }else{
        strcpy(msgEnvia, "550 erro ao acessar diretorio\n");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        printf("CDUP erro ao acessar diretório anterior\n");
        return 0;
    }
}

int opPut(int cliente){
    printf("PUT aguardando arquivo\n");
    char msgRecebe[100];
    int tamanho, fileh;
    char *arquivo;
    char nomeArquivo[25];
    read(cliente, msgRecebe, MAXBUF);
    if((strcmp(msgRecebe, "ok")) == 0){
        printf("PUT cliente ok\n");
        read(cliente, nomeArquivo, 25);
        printf("PUT recebendo arquivo \'%s\'\n", nomeArquivo);
        read(cliente, &tamanho, sizeof(int));

        arquivo = malloc(tamanho);
        read(cliente, arquivo, tamanho);

        fileh = open(nomeArquivo, O_CREAT | O_EXCL | O_WRONLY, 0666);
        write(fileh, arquivo, tamanho, 0);
        close(fileh);

        printf("PUT arquivo recebido\n");
        return 1;
    }else{
        printf("PUT erro do cliente\n");
        printf("PUT finalizado\n");
        return 0;
    }
}

int opGet(int cliente){
    printf("GET solicitado\n");
    char msgEnvia[100];
    char nomeArquivo[25];
    struct stat obj;
    int arquivo, tamanho;
    read(cliente, nomeArquivo, 25);
    if(fopen(nomeArquivo, "r+") == NULL){
        strcpy(msgEnvia, "error");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        printf("GET status = \'%s\' enviado\n", msgEnvia);
        printf("GET finalizado\n");
        return 0;
    }else{
        strcpy(msgEnvia, "ok");
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
    }
    printf("GET enviando arquivo \'%s\'\n", nomeArquivo);
    stat(nomeArquivo, &obj);

    arquivo = open(nomeArquivo, O_RDONLY);
    tamanho = obj.st_size;

    if(arquivo == 1)
        tamanho = 0;
    write(cliente, &tamanho, sizeof(int));
    if(tamanho)
        sendfile(cliente, arquivo, NULL, tamanho);

    printf("GET arquivo enviado\n");
    return 1;
}



int opPwd(int cliente){
    printf("PWD solicitado\n");
    int i;
    char endereco[100];
    char msgEnvia[100];
    int arquivo;
    FILE *arq;

    system("pwd >temp.txt");

    arq = fopen("temp.txt", "r");
    i = 0;
    while((endereco[i] = fgetc(arq)) != EOF)
        i++;
    endereco[i] = NULL;
    fclose(arq);

    strcpy(msgEnvia, "257 ");
    strcat(msgEnvia, endereco);
    write(cliente, msgEnvia, strlen(msgEnvia)+1);

    printf("PWD enviado\n");
    return 1;
}

int opRmd(int cliente, char pasta[]){
    printf("RMD solicitado\n");
    char msgEnvia[100];

    printf("RMD pasta \'%s\'\n", pasta);
        printf("RMD confirmado pelo usuário na pasta \'%s\'\n", pasta);
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
        strcpy(msgEnvia, "250 deletado com sucesso\n");   // confirmação de apagado
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        return 1;
    }else{
        printf("DELE erro ao deletar arquivo \'%s\'\n", arquivo);
        strcpy(msgEnvia, "550 arquivo nao pode ser deletado\n");   // confirmação de erro
        write(cliente, msgEnvia, strlen(msgEnvia)+1);
        return 0;
    }
}
