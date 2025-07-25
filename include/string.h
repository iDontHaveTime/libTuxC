#ifndef STRING_H
#define STRING_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stddef.h"

const char* strerror(int errnum);

extern short __tuxc_align_lookup8[8];
#define __tuxc_getalignment8(n) (__tuxc_align_lookup8[(size_t)n & 0x7])

size_t strlen(const char* str);
void* memcpy(void* dest, const void* src, size_t n);
void* mempcpy(void* dest, const void* src, size_t n);

// returns 1 to found if c is found
void* memcpy_c(void* dest, const void* src, size_t n, char c, char* found);
void* mempcpy_c(void* dest, const void* src, size_t n, char c, char* found);

char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);

#ifdef __cplusplus
}
#endif

#endif // STRING_H
