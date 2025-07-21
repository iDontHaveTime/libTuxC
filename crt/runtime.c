#include "cross/stkcgen.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"

extern int main(int, char**);

uintptr_t __stack_chk_guard = 0xDEADBEEF;
void* main_ptr = &main; // let clang handle PIC
void* exit_ptr = &exit;

void __set_schk(){
    __stack_chk_guard = __stack_chk_guard_gen();
}

__attribute__((naked)) void _start(uintptr_t argc, char** argv){
    __asm__ volatile(
        "pushq %%rbx\n"
        "leaq 8(%%rsp), %%rbx"
        :
        :
        : "rbx"
    );

    __asm__ volatile(
        "call __set_schk"
    );

    __asm__ volatile(
        "movq (%%rbx), %%rdi\n"
        "leaq 8(%%rbx), %%rsi\n"
        "movq %0, %%rax\n"
        "call *%%rax"
        : 
        : "r"(main_ptr)
        : "rdi", "rsi", "rax"
    );

    __asm__ volatile(
        "popq %%rbx"
        :
        :
        : "rbx"
    );
    __asm__ volatile(
        "movq %%rax, %%rdi\n"
        "movq %0, %%rax\n"
        "call *%%rax"
        :
        : "r"(exit_ptr)
        : "rdi", "rax"
    );
}

void __attribute__((noreturn)) __stack_chk_fail(void){

    exit(1);
    for(;;){}
}