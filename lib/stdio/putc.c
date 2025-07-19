#include "cross/crosscalls.h"
#include "stddef.h"
#include "stdio.h"

int putc(int ch, FILE* f){
    unsigned char c = (unsigned char)ch;
    return fwrite(&c, 1, 1, f) == 1 ? c : EOF;
}

int putchar(int ch){
    return putc(ch, stdout);
}