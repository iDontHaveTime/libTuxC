#include "cross/stkcgen.h"
#include "stdlib.h"
#include "stdint.h"

extern int main(int, char**);

uintptr_t __stack_chk_guard;

void _start(){
    __stack_chk_guard = __stack_chk_guard_gen();
    
    uintptr_t argc;
    char** argv;

    __asm__ volatile (
        "movq (%%rsp), %0\n"
        "leaq 8(%%rsp), %1\n"
        : "=r"(argc), "=r"(argv)
    );
    int exitCode = main(argc, argv);

    exit(exitCode);
}

void __attribute__((noreturn)) __stack_chk_fail(void){

    exit(1);
    for(;;){}
}