#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char* argv[]){
    char* buff = malloc(512);

    fputs("Input: ", stdout);
    fflush(stdout);
    fgets(buff, 512, stdin);

    fputs("Your message: ", stdout);

    buff[strlen(buff) - 1] = '\0'; // replace \n to \0
    puts(buff);

    free(buff);

    return 0;
}