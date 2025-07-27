#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int main(int argc, char* argv[]){
    char* ptr = malloc(24);
    if(!ptr) return 1;
    printf("String is %s\n", "Hello");

    memset(ptr, 'a', 24);
    ptr[23] = '\0';

    printf("%s\n", ptr);

    free(ptr);

    return 0;
}