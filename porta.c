#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void convertToHex(int decimalNumber, int set);

char v1[100], v2[100], conc[100];
int i1, i2;

int main(){
    int status;
    status = calcPort(50, 80);
    return 0;
}
int calcPort(int val1, int val2){
    int port, i, j;
    convertToHex(val1, 0);
    convertToHex(val2, 1);
    printf("\nEquivalent hexadecimal value of decimal number %d: ", val1);
    for(j = i1 ;j> 0;j--)
        printf("%c",v1[j]);

    printf("\nEquivalent hexadecimal value of decimal number %d: ",val2);
    for(j = i1;j> 0;j--)
        printf("%c",v2[j]);
    printf("\n");
    arruma();
    inverter();
    concatena();
    printf("\nEquivalent hexadecimal value of decimal number %d: ", val1);
    for(j = i1 ;j> 0;j--)
        printf("%c",v1[j]);

    printf("\nEquivalent hexadecimal value of decimal number %d: ",val2);
    for(j = i1;j> 0;j--)
        printf("%c",v2[j]);
    printf("\n");
    return 0;
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
    }else{
        v2[i++] = temp;
        i2 = i;
    }
    quotient = quotient / 16;
    }

//    printf("Equivalent hexadecimal value of decimal number %d: ",decimalNumber);
//    for(j = i -1 ;j> 0;j--)
      //printf("%c",v2[j]);

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
    printf("Conc: %s\n", conc);
}

void inverter(){
    v1[i1] = '\0';
    v2[i2] = '\0';
    int i, j;
    char temp[100], temp2[100];
    j = i1;
    for(i=0; i<i1; i++){
        temp[j] = v1[i];
        j--;
    }
    for(i=0; i<i1; i++){
        v1[i] = temp[i];
        printf("%c", v1[i]);
    }
    v1[i] = '\0';

    j = i2;
    for(i=0; i<i2; i++){
        temp2[j] = v2[i];
        j--;
    }
    printf("\n");
    for(i=0; i<i2; i++){
        v2[i] = temp2[i];
        printf("%c", v2[i]);
    }
    v2[i] = '\0';
    printf("\n");

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


    printf("ARRUMA!\n");
    for(i=0; i<i1; i++)
        printf("%c", v1[i]);
    printf("\n");
    for(i=0; i<i2; i++)
        printf("%c", v2[i]);
    printf("\n");
}
