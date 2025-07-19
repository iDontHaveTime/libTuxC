#include "stdio.h"
#include "stdlib.h"

int main(int argc, char* argv[]){
    char* buff = malloc(512);

    fgets(buff, 512, stdin);

    free(buff);

    return 0;
}