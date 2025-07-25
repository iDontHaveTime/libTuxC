#include "stdio.h"
#include "errno.h"
#include "string.h"
#include <stdio.h>

void perror(const char* str){
    const char* msg = strerror(errno);
    if(str && *str){
        fputs(str, stderr);
        fputs(": ", stderr);
    }
    fputs(msg, stderr);
    fputc('\n', stderr);
}