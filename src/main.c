#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char* argv[]){
    char* buff = malloc(512);

    memcpy(buff, "Hello", strlen("Hello") + 1);

    puts(buff);

    free(buff);

    return 0;
}