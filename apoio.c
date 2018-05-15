#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char* readFileBytes(const char *name)    // Link de referencia da função está no arquivo de Links
{
    FILE *fl = fopen(name, "r");
    fseek(fl, 0, SEEK_END);
    long len = ftell(fl);
    char *ret = malloc(len);
    fseek(fl, 0, SEEK_SET);
    fread(ret, 1, len, fl);
    fclose(fl);
    return ret;
}
