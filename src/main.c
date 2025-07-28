#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int main(int argc, char* argv[]){
    char* ptr = malloc(50);
    if(!ptr) return 1;
    printf("String is %s\n", "Hello");

    memset(ptr, 'a', 24);
    ptr[23] = '\0';
    printf("%s\n", ptr);

    printf("the size is %zd\n", _tuxc_malloc_usersize(ptr));
    printf("the size is %zd\n", _tuxc_malloc_fullsize(ptr));
    printf("%d\n", -24);
    
    printf("|%u|%5u|%05u|%.3u|\n", 42, 42, 42, 42);
    printf("|%-5u|%u|\n", 42, 1234567890);

    free(ptr);
    ptr = malloc(50);

    return 0;
}