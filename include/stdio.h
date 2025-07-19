#ifndef STDIO_H
#define STDIO_H

#include "noncstd/stdiofile.h"
#include "stddef.h"

#define EOF (-1)
#define L_tmpnam 64
#define TMP_MAX 9999

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int putchar(int ch);
int putc(int ch, FILE* f);

int fflush(FILE* fs);
int __fflush_all(void);

void clearerr(FILE* fs);

int feof(FILE* fs);
int ferror(FILE* fs);

size_t fread(void* mem, size_t size, size_t count, FILE* stream);
size_t fwrite(const void* mem, size_t size, size_t count, FILE* stream);

int fputs(const char* str, FILE* fs);
int puts(const char* str);

FILE* fopen(const char* fileName, const char* mode);
int fclose(FILE* fst);

int fcloseall(void);

int fgetc(FILE* fs);
char* fgets(char* buff, size_t n, FILE* fs);

char* gets(char* buff);
int getchar(void);

int ungetc(int c, FILE* fs);

#define fputc putc
#define getc fgetc

#endif // STDIO_H
