#ifndef RUNTIME_H
#define RUNTIME_H

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

extern void (*__preinit_array_start[])(void) __attribute__((weak));
extern void (*__preinit_array_end[])(void) __attribute__((weak));
extern void (*__init_array_start[])(void) __attribute__((weak));
extern void (*__init_array_end[])(void) __attribute__((weak));

#ifdef __cplusplus
}
#endif

#endif // RUNTIME_H
