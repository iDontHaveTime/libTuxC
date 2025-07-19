#include "stddef.h"
#include "stdio.h"
#include "string.h"

int fputs(const char* str, FILE* fs){
    if(!str || !fs) return EOF;

    size_t slen = strlen(str);

    return fwrite(str, 1, slen, fs) == slen ? 0 : EOF;
}

int puts(const char* str){
    return (fputs(str, stdout) != EOF ? (putchar('\n') != EOF ? 0 : EOF) : EOF);
}