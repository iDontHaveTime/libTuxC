#include "stddef.h"
#include "string.h"

char* strcpy(char* dest, const char* src){
    if(!dest) return NULL;
    if(!src) return dest;

    char* original_dest = dest;

    short dest_a = __tuxc_getalignment8(dest);
    short src_a = __tuxc_getalignment8(src);

    short align = dest_a <= src_a ? dest_a : src_a;

    if(align == 1){
        while((*dest++ = *src++));
    }
    else{
        memcpy(dest, src, strlen(src) + 1);
    }

    return original_dest;
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