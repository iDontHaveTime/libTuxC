#ifndef STRING_H
#define STRING_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stddef.h"

const char* strerror(int errnum);

#ifdef __x86_64__
#define __TUXC_16_BYTES_ALLOWED__
#endif

extern short __tuxc_align_lookup8[8];
#define __tuxc_getalignment8(n) (__tuxc_align_lookup8[(size_t)n & 0x7])

size_t strnlen(const char* str, size_t max);
size_t strlen(const char* str);

char* strdup(const char* str);
char* strndup(const char* str, size_t max);

void* memcpy(void* dest, const void* src, size_t n);
void* mempcpy(void* dest, const void* src, size_t n);

void* memset(void* dest, int ch, size_t n);

// sets found to 1 if found c while copying
void* memcpy_c(void* dest, const void* src, size_t n, char c, char* found);
void* mempcpy_c(void* dest, const void* src, size_t n, char c, char* found);

char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);

#ifdef __cplusplus
}
#endif

#endif // STRING_H
