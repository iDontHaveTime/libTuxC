#include "string.h"
#include "stddef.h"

void* memcpy(void* dest, const void* src, size_t n){
    if(!dest) return NULL;
    if(!src) return dest;
    if(n == 0) return dest;

    #if defined(__x86_64__) || defined(__i386__)
    __asm__ volatile(
        "cld\n"
        "rep movsb"
        : 
        : "D"(dest), "S"(src), "c"(n)
        : "memory"
    );

    #ifndef __x86_64__
    // i386
    return dest;
    #else
    // x86-64
    // movsq

    return dest;
    #endif
    #else
    char* startDest = dest;
    const char* startSrc = src;

    for(size_t i = 0; i < n; i++){
        startDest[i] = startSrc[i];
    }

    return dest;
    #endif
}