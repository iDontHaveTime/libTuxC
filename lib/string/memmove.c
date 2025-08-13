#include "string.h"

void* memmove(void* dest, const void* src, size_t n){
    if(!dest) return NULL;
    if(!src) return dest;
    if(n == 0) return dest;

    if(dest == src) return dest;

    #if defined(__x86_64__) || defined(__i386__)

    #ifndef __x86_64__
    return dest;
    #else

    return dest;
    #endif
    
    #else

    char* d = dest;
    const char* s = src;

    if(d < s){
        for(size_t i = 0; i < n; i++)
            d[i] = s[i];
    } 
    else{
        for(size_t i = n; i > 0; i--)
            d[i-1] = s[i-1];
    }

    return dest;
    #endif
}