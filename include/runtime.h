#ifndef _RUNTIME_H
#define _RUNTIME_H

#ifdef __cplusplus
extern "C"{
#endif

#include "noncstd/tuxcmac.h"

#define MAX_CXA_ATEXIT_FUNCS 32

typedef struct{
    void (*func)(void *);
    void *arg;
    void *dso_handle;
} cxa_atexit_entry_t;

typedef void (*ctor_t)(void);

extern ctor_t __init_array_start[] __port_attr((weak));
extern ctor_t __init_array_end[] __port_attr((weak));

extern ctor_t __fini_array_start[] __port_attr((weak));
extern ctor_t __fini_array_end[] __port_attr((weak));

extern void (*__preinit_array_start[])(void) __port_attr((weak));
extern void (*__preinit_array_end[])(void) __port_attr((weak));
extern void (*__init_array_start[])(void) __port_attr((weak));
extern void (*__init_array_end[])(void) __port_attr((weak));

extern char __data_start[];
extern char __data_end[];
extern char __data_load_start[];

extern char __bss_start[];
extern char __bss_end[];

extern char _etext[]; // end of .text
extern char _edata[]; // end of .data
extern char _end[]; // end of all segments

extern char __rel_start[];
extern char __rel_end[];
extern char __text_start[];
extern char __rodata_start[];

extern char __stack_start[];

#ifdef __cplusplus
}
#endif

#endif // RUNTIME_H
