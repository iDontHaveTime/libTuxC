#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
void* memcpy(void* dest, const void* src, size_t n);
void* mempcpy(void* dest, const void* src, size_t n);

// returns 1 to found if c is found
void* memcpy_c(void* dest, const void* src, size_t n, char c, char* found);
void* mempcpy_c(void* dest, const void* src, size_t n, char c, char* found);

char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);

#endif // STRING_H
