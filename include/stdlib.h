#ifndef STDLIB_H
#define STDLIB_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stddef.h"
#include "noncstd/tuxcmac.h"
#include "stdnoreturn.h"

#define ATEXIT_MAX 64

noreturn void exit(int code);
noreturn void _exit(int code);
noreturn void abort(void);

int atexit(void (*f)(void));

void* malloc(size_t size) __port_attr((malloc));
void free(void* ptr);
void* realloc(void* mem, size_t newSize);
void* calloc(size_t num, size_t size);

char* itoa(int num, char* buffer, int base);
char* ltoa(long num, char* buffer, int base);
char* lltoa(long long num, char* buffer, int base);

char* utoa(unsigned int num, char* buffer, int base);
char* lutoa(unsigned long num, char* buffer, int base);
char* llutoa(unsigned long long num, char* buffer, int base);

void* aligned_malloc(size_t align, size_t size);
size_t _tuxc_malloc_usersize(void* ptr);
size_t _tuxc_malloc_fullsize(void* ptr);

#ifdef __cplusplus
}
#endif

#endif // STDLIB_H
