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

void crt_memcpy(void* dest, const void* src, size_t n){
    char* d = (char*)dest;
    const char* s = (const char*)src;
    while(n--){
        *d++ = *s++;
    }
}

void crt_memset(void* dest, int val, size_t n){
    unsigned char* d = (unsigned char*)dest;
    while(n--){
        *d++ = (unsigned char)val;
    }
}

void crt_init(){
    #ifndef __linux__
    size_t data_size = __data_end - __data_start;
    size_t bss_size = __bss_end - __bss_start;
    crt_memcpy(__data_start, __data_load_start, data_size);
    crt_memset(__bss_start, 0, bss_size);
    #endif
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
    abort();
}

void __set_schk(){
    crt_init();
    __stack_chk_guard = __stack_chk_guard_gen();
    run_constructors();
}

__attribute__((naked, used, visibility("default"))) void _start(uintptr_t argc, char** argv){
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