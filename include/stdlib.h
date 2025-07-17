#ifndef STDLIB_H
#define STDLIB_H

#include "stddef.h"
#include "noncstd/tuxcmac.h"

#define ATEXIT_MAX 64

void exit(int code);
void _exit(int code);
void abort(void);

int atexit(void (*f)(void));

void* malloc(size_t size) __port_attr((malloc));
void free(void* ptr);
void* realloc(void* mem, size_t newSize);

void* aligned_malloc(size_t align, size_t size);
size_t _tuxc_malloc_usersize(void* ptr);
size_t _tuxc_malloc_fullsize(void* ptr);

#endif // STDLIB_H
