#include "stddef.h"
#include "string.h"

char* strcpy(char* dest, const char* src){
    if(!dest) return NULL;
    if(!src) return dest;
    char* original_dest = dest;

    while((*dest++ = *src++));
    return dest;
}

char* strncpy(char* dest, const char* src, size_t n){
    if(!dest) return NULL;
    if(!src) return dest;
    if(n == 0) return dest;
    char* destptr = dest;
    const char* destend = dest + n;

    while(destptr < destend){
        *destptr = *src;
        src++;
        destptr++;
        if(*src == '\0'){
            break;
        }
    }

    while(destptr < destend){
        *destptr = '\0';
        destptr++;
    }

    return dest;
}