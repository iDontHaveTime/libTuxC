#include "stdio.h"
#include "errno.h"
#include "string.h"
#include <stdio.h>

void perror(const char* str){
    const char* msg = strerror(errno);
    if(str && *str){
        fprintf(stderr, "%s: %s\n", str, msg);
    }
    else{
        fprintf(stderr, "%s\n", msg);
    }
}