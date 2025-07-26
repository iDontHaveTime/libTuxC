#ifndef STDIO_H
#define STDIO_H

#ifdef __cplusplus
extern "C"{
#endif

#include "noncstd/stdiofile.h"
#include "stddef.h"
#include "stdarg.h"

#if defined(__GNUC__) || defined(__clang__)
#define __fmt__func__(x, y) __attribute__((format(printf, x, y)))
#else
#define __fmt__func__(x, y)
#endif  

#define __PRINTF_BUFFER_SIZE__ 512

#define EOF (-1)
#define L_tmpnam 64
#define TMP_MAX 9999

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define TUXC_UNKNOWN_ENDIAN 0
#define TUXC_BIG_ENDIAN 1
#define TUXC_LITTLE_ENDIAN 2

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define __tuxc_endian TUXC_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define __tuxc_endian TUXC_BIG_ENDIAN
#else
#define __tuxc_endian TUXC_UNKNOWN_ENDIAN
#endif

int putchar(int ch);
int putc(int ch, FILE* f);

int fflush(FILE* fs);
int __fflush_all(void);
int __flush_unlock(FILE* fs);

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

int setvbuf(FILE* fs, char* buff, int mode, size_t size);
void setbuf(FILE* fs, char* buff);

void perror(const char* str);

int printf(const char* fmt, ...) __fmt__func__(1, 2);
int vprintf(const char* fmt, va_list args);

int sprintf(char* str, const char* fmt, ...) __fmt__func__(2, 3);
int vsprintf(char* str, const char* fmt, va_list args);

int snprintf(char* str, size_t max, const char* fmt, ...) __fmt__func__(3, 4);
int vsnprintf(char* str, size_t max, const char* fmt, va_list args);

int fprintf(FILE* fs, const char* fmt, ...) __fmt__func__(2, 3);
int vfprintf(FILE* fs, const char* fmt, va_list args);

#define fputc putc
#define getc fgetc

#ifdef __cplusplus
}
#endif

#endif // STDIO_H
