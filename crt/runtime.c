#include "cross/stkcgen.h"
#include "stdlib.h"
#include "stdint.h"
#include "runtime.h"
#include "stdnoreturn.h"

extern int main(int, char**);

uintptr_t __stack_chk_guard = 0xDEADBEEF;
void* main_ptr = &main; // let clang handle PIC
void* exit_ptr = &exit;

void run_array(void (**start)(void), void (**end)(void)){
    for(void (**func)(void) = start; func < end; func++){
        (*func)();
    }
}

void run_constructors(void){
    if(__preinit_array_start && __preinit_array_end){
        run_array(__preinit_array_start, __preinit_array_end);
    }
    if(__init_array_start && __init_array_end){
        run_array(__init_array_start, __init_array_end);
    }
}

int __cxa_guard_acquire(uint64_t* guard){
    return !*(char*)guard;
}

void __cxa_guard_release(uint64_t* guard){
    *(char*)guard = 1;
}

void __cxa_guard_abort(uint64_t* guard){
    // nop
}

void __set_schk(){
    __stack_chk_guard = __stack_chk_guard_gen();
    run_constructors();
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

noreturn void  __stack_chk_fail(void){
    exit(1);
}