#include "stdlib.h"
#include "stdio.h"

int main(int argc, char* argv[]){
    void* ptr = malloc(24);
    printf("String is %-2.1sH %s %c\n", "Hello", "another?", 'H');
    free(ptr);

    return 0;
}