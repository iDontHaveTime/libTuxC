#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char* argv[]){
    char* buff = malloc(512);

    memcpy(buff, "Hello", strlen("Hello") + 1);

    puts(buff);

    free(buff);
    
    putchar(__tuxc_endian + '0');

    return 0;
}