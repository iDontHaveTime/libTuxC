#include "cross/stkcgen.h"
#include "stdlib.h"
#include "stdint.h"

extern int main(int, char**);

uintptr_t __stack_chk_guard;

void _start(){
    __asm__ volatile(
        "movq %%rsp, %%r11\n"
        "pushq %%rbx\n"
        "movq %%r11, %%rbx"
        :
        :
        : "r11", "rbx"
    );

    __stack_chk_guard = __stack_chk_guard_gen();
    
    uintptr_t argc;
    char** argv;

    __asm__ volatile(
        "movq (%%rbx), %0\n"
        "leaq 8(%%rbx), %1\n"
        : "=r"(argc), "=r"(argv)
    );
    int exitCode = main(argc, argv);

    __asm__ volatile(
        "popq %%rbx"
    );
    exit(exitCode);
}

void __attribute__((noreturn)) __stack_chk_fail(void){

    exit(1);
    for(;;){}
}