#include "stddef.h"
#include "string.h"
#include "stdint.h"

void* memset(void* dest, int ch, size_t n){
    if(!dest) return NULL;
    if(n == 0) return dest;
    void* ret_val = dest;

    #if defined(__x86_64__) || defined(__i386__)
    // x86-64, i386

    short align = __tuxc_getalignment8(dest);
    align = (align == 8) ? (sizeof(void*) >= 8 ? align : sizeof(void*)) : align;

    while(n % align){
        align >>= 1;
    }

    n /= align;

    #if defined(__x86_64__)
    uint64_t fill = (uint64_t)ch | (uint64_t)ch << 8 | (uint64_t)ch << 16 | (uint64_t)ch << 24 |
                         (uint64_t)ch << 32 | (uint64_t)ch << 40 | (uint64_t)ch << 48 | (uint64_t)ch << 56;
    #else
    uint32_t fill = (uint32_t)ch | (uint32_t)ch << 8 | (uint32_t)ch << 16 | (uint32_t)ch << 24;
    #endif

    if(align == 1){
        __asm__ volatile(
            "cld\n"
            "rep stosb"
            : 
            : "D"(dest), "c"(n), "a"(fill)
            : "memory"
        );
    }
    else if(align == 2){
        __asm__ volatile(
            "cld\n"
            "rep stosw"
            : 
            : "D"(dest), "c"(n), "a"(fill)
            : "memory"
        );
    }
    else if(align == 4){
        __asm__ volatile(
            "cld\n"
            "rep stosl"
            : 
            : "D"(dest), "c"(n), "a"(fill)
            : "memory"
        );
    }
    #if !defined(__x86_64__)
    // i386
    return ret_val;
    #else
    // x86-64
    else if(align == 8){
        __asm__ volatile(
            "cld\n"
            "rep stosq"
            : 
            : "D"(dest), "c"(n), "a"(fill)
            : "memory"
        );
    }
    else{
        return NULL;
    }

    return ret_val;
    #endif
    #else
    // non x86

    while(n--){
        *(char*)dest++ = (char)ch;
    }
    return ret_val;
    #endif
}