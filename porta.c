#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void convertToHex(int decimalNumber, int set);
void concatena();
void inverter();
void arruma();
int convertToDec();
int calcPort(int val1, int val2);
int calcPortPASV(int val, int set);
void separa();
void inverterPasv();
void arrumaPasv();
int convertToDecPASV(int set);


char v1[100], v2[100], conc[100];
int i1, i2, iConc;



int calcPortPASV(int val, int set){
    int i, port1, port2;
    printf("PASV porta: %i\n", val);
    zerar();
    convertToHex(val, 2);
    arrumaPasv();
    inverterPasv();
    separa();
    port1 = convertToDecPASV(0);
    port2 = convertToDecPASV(1);
    printf("PORTAs: %i e %i\n", port1, port2);
    if(set == 0)
        return port1;
    else
        return port2;
}

int calcPort(int val1, int val2){
    int port, i, j;
    zerar();
    convertToHex(val1, 0);
    convertToHex(val2, 1);
    arruma();
    inverter();
    concatena();
    port = convertToDec();
    //printf("PORTA: %i\n", port);
    return port;
}

void convertToHex(int decimalNumber, int set){       // Referencia no link que está em 'Link_Envio'
    long int remainder,quotient;
    int i=1,j,temp;



    quotient = decimalNumber;

    while(quotient!=0){
        temp = quotient % 16;

      //To convert integer into character
        if( temp < 10)
            temp =temp + 48;
        else
            temp = temp + 55;
        if(set == 0){
            v1[i++]= temp;
            i1 = i;
        }else if(set == 1){
            v2[i++] = temp;
            i2 = i;
        }else if(set == 2){
            conc[i++] = temp;
            iConc = i;
        }
        quotient = quotient / 16;
    }
}

void concatena(){
    int i, j=0;
    for(i=0; i<i1; i++){
        conc[j] = v1[i];
        j++;
    }
    for(i=0; i<i2; i++){
        conc[j] = v2[i];
        j++;
    }
    //printf("Conc: %s\n", conc);
}

void separa(){
    int i, j, tam, meio;
    tam = strlen(conc);
    meio = tam/2;
    for(i=0; i<meio; i++){
        v1[i] = conc[i];
    }
    j=0;
    for(i=meio; i<tam; i++){
        v2[j] = conc[i];
        j++;
    }

}

void inverter(){
    v1[i1] = '\0';
    v2[i2] = '\0';
    int i, j;
    char temp[100], temp2[100];
    j = i1-1;
    for(i=0; i<i1; i++){
        temp[j] = v1[i];
        j--;
    }
    for(i=0; i<i1; i++){
        v1[i] = temp[i];
    }
    v1[i] = '\0';

    j = i2-1;
    for(i=0; i<i2; i++){
        temp2[j] = v2[i];
        j--;
    }
    printf("\n");
    for(i=0; i<i2; i++){
        v2[i] = temp2[i];
    }
    v2[i] = '\0';

}

void inverterPasv(){
    conc[iConc] = '\0';
    int i, j;
    char temp[100], temp2[100];
    j = iConc-1;
    for(i=0; i<iConc; i++){
        temp[j] = conc[i];
        j--;
    }
    for(i=0; i<iConc; i++){
        conc[i] = temp[i];
    }
    conc[i] = '\0';


}

void arruma(){
    int i;
    if((v1[0] != '0') && (v1[0] != '1') && (v1[0] != '2') && (v1[0] != '3') && (v1[0] != '4') && (v1[0] != '5') && (v1[0] != '6') && (v1[0] != '7') && (v1[0] != '8') && (v1[0] != '9') && (v1[0] != 'A') && (v1[0] != 'B') && (v1[0] != 'C') && (v1[0] != 'D') && (v1[0] != 'E') && (v1[0] != 'F')){
        for(i=0; i<i1; i++){
            v1[i] = v1[i+1];
        }
        i1--;
    }

    if((v2[0] != '0') && (v2[0] != '1') && (v2[0] != '2') && (v2[0] != '3') && (v2[0] != '4') && (v2[0] != '5') && (v2[0] != '6') && (v2[0] != '7') && (v2[0] != '8') && (v2[0] != '9') && (v2[0] != 'A') && (v2[0] != 'B') && (v2[0] != 'C') && (v2[0] != 'D') && (v2[0] != 'E') && (v2[0] != 'F')){
        for(i=0; i<i2; i++)
            v2[i] = v2[i+1];
        i2--;
    }
}

void arrumaPasv(){
    int i;
    if((conc[0] != '0') && (conc[0] != '1') && (conc[0] != '2') && (conc[0] != '3') && (conc[0] != '4') && (conc[0] != '5') && (conc[0] != '6') && (conc[0] != '7') && (conc[0] != '8') && (conc[0] != '9') && (conc[0] != 'A') && (conc[0] != 'B') && (conc[0] != 'C') && (conc[0] != 'D') && (conc[0] != 'E') && (conc[0] != 'F')){
        for(i=0; i<iConc; i++){
            conc[i] = conc[i+1];
        }
        iConc--;
    }

}

int convertToDec(){
    long int port = strtol(conc, NULL, 16);
    return port;
}

int convertToDecPASV(int set){
    long int port;
    if(set == 0)
        port = strtol(v1, NULL, 16);
    else
        port = strtol(v2, NULL, 16);
    return port;
}

void zerar(){
    int i;
    for(i=0; i<100; i++){
        v1[i] = NULL;
        v2[i] = NULL;
        conc[i] = NULL;
    }
}
