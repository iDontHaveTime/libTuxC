#include "cross/stkcgen.h"
#include "stdint.h"

uintptr_t __stack_chk_guard_gen(){
    uintptr_t res;
    
    #if defined(__x86_64__)
    int ok;
    __asm__ volatile(
        "rdrand %0; setc %1"
        : "=r"(res), "=qm"(ok)
        :
        : "cc"
    );

    if(!ok){
        __asm__ volatile("rdtsc" : "=A"(res));
    }
    #endif

    return res;
}