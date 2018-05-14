#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char convertToHex(int decimalNumber);


int main(){
    int status;
    status = calcPort(4, 15);
    return 0;
}
int calcPort(int val1, int val2){
    int port, i, j;
    const char *v1[] = convertToHex(val1);
    const char *v2[] = convertToHex(val2);
    printf("\nEquivalent hexadecimal value of decimal number %d: %s", val1,v1);
    for(j = i -1 ;j> 0;j--)
        printf("%c",v1[j]);

    printf("\nEquivalent hexadecimal value of decimal number %d: %s",val2, v2);

    return 0;
}

char convertToHex(int decimalNumber){       // Referencia no link que está em 'Link_Envio'
    long int remainder,quotient;
    int i=1,j,temp;
    char hexadecimalNumber[100];

    quotient = decimalNumber;

    while(quotient!=0){
         temp = quotient % 16;

      //To convert integer into character
      if( temp < 10)
           temp =temp + 48;
      else
         temp = temp + 55;

      hexadecimalNumber[i++]= temp;
      quotient = quotient / 16;
    }

    //printf("Equivalent hexadecimal value of decimal number %d: ",decimalNumber);
    //for(j = i -1 ;j> 0;j--)
      //printf("%c",hexadecimalNumber[j]);

    return hexadecimalNumber;
}
