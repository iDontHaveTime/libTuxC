#include "stddef.h"
#include "string.h"

void* memcpy_c(void* dest, const void* src, size_t n, char c, char* found){
    if(n == 0) return dest;
    if(!dest) return NULL;
    if(!src) return dest;
    if(!found) return memcpy(dest, src, n);

    char* startDest = dest;
    const char* startSrc = src;

    *found = 0;

    for(size_t i = 0; i < n; i++){
        startDest[i] = startSrc[i];
        if(startSrc[i] == c) *found = 1;
    }

    return dest;
}

void* mempcpy_c(void* dest, const void* src, size_t n, char c, char* found){
    return (char*)memcpy_c(dest, src, n, c, found) + n;
}