#ifndef _ASSERT_H
#define _ASSERT_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stdnoreturn.h"

#ifdef NDEBUG
    #define assert(expr) ((void)0)
#else
    #define assert(expr) \
        ((expr) ? (void)0 : __assert_fail(#expr, __FILE__, __LINE__, __func__))
#endif

#if __STDC_VERSION__ >= 201112L
    #define static_assert _Static_assert
#else
    #define static_assert(expr, msg) ((void)0)
#endif

noreturn void __assert_fail(const char* expr_s, const char* file, long line, const char* func);

#ifdef __cplusplus
}
#endif

#endif // ASSERT_H
