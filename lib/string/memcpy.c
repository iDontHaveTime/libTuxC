#include "string.h"
#include "stddef.h"

// TO BE OPTIMIZED

void* memcpy(void* dest, const void* src, size_t n){
    if(!dest) return NULL;
    if(!src) return dest;
    if(n == 0) return dest;
    char* startDest = dest;
    const char* startSrc = src;

    for(size_t i = 0; i < n; i++){
        startDest[i] = startSrc[i];
    }

    return dest;
}