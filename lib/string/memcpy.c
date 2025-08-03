#include "string.h"
#include "stddef.h"

void* memcpy(void* dest, const void* src, size_t n){
    if(!dest) return NULL;
    if(!src) return dest;
    if(n == 0) return dest;

    if(dest == src) return dest;
    
    #if defined(__x86_64__) || defined(__i386__)
    if(n < sizeof(void*)){
        char* d = dest;
        const char* s = src;
        while(n--){
            *d++ = *s++;
        }
        return dest;
    }


    short dest_a = __tuxc_getalignment8(dest);
    short src_a = __tuxc_getalignment8(src);

    short align = dest_a <= src_a ? dest_a : src_a;

    align = (align == 8) ? (sizeof(void*) >= 8 ? align : 4) : align;

    while(n % align){
        align >>= 1;
    }

    n /= align;

    if(align == 1){
        __asm__ volatile(
            "cld\n"
            "rep movsb"
            : 
            : "D"(dest), "S"(src), "c"(n)
            : "memory"
        );
    }
    else if(align == 2){
        __asm__ volatile(
            "cld\n"
            "rep movsw"
            : 
            : "D"(dest), "S"(src), "c"(n)
            : "memory"
        );
    }
    else if(align == 4){
        __asm__ volatile(
            "cld\n"
            "rep movsd"
            : 
            : "D"(dest), "S"(src), "c"(n)
            : "memory"
        );
    }

    #ifndef __x86_64__
    // i386
    return dest;
    #else
    // x86-64
    // movsq

    if(align == 8){
        __asm__ volatile(
            "cld\n"
            "rep movsq"
            : 
            : "D"(dest), "S"(src), "c"(n)
            : "memory"
        );
    }

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