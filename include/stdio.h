#ifndef STDIO_H
#define STDIO_H

#include "noncstd/stdiofile.h"

#define EOF -1

int putchar(int ch);
int putc(int ch, FILE* f);

int fputs(const char* str, FILE* fs);
int fflush(FILE* fs);

int puts(const char* str);

FILE* fopen(const char* fileName, const char* mode);
int fclose(FILE* fst);

#endif // STDIO_H
