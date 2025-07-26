#include "stdlib.h"
#include "stdio.h"

int main(int argc, char* argv[]){
    void* ptr = malloc(24);
    printf("String is %-2.1sH %s %c %s\n", "Hello", "another?", 'H', argv[0]);
    free(ptr);
    
    return 0;
}