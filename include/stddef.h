#ifndef STDDEF_H
#define STDDEF_H

#ifdef __cplusplus
extern "C"{
#endif

#ifdef _WIN64
typedef unsigned long long size_t;
typedef signed long long ptrdiff_t;
#else
typedef unsigned long size_t;
typedef signed long ptrdiff_t;
#endif
#ifdef _WIN32
typedef short wchar_t;
#else
typedef int wchar_t;
#endif

#define NULL ((void*)0)

#ifndef __cplusplus
    #if defined(__GNUC__) || defined(__clang__)
        #define __INT128_DEFINED__
        typedef unsigned __int128 __u128;
        typedef __int128 __i128;
    #endif
#endif

typedef union{
    long long   __ll;
    long double __ld;
#ifdef __INT128_DEFINED__
    __i128 __i128_val;
#endif
} max_align_t;

#ifdef __cplusplus
}
#endif

#endif // STDDEF_H
