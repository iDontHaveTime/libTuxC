#ifndef STDDEF_H
#define STDDEF_H

#ifdef __cplusplus
extern "C"{
#endif
typedef __typeof__(sizeof(void*)) size_t;
#if __SIZEOF_POINTER__ == 8
//typedef __UINT64_TYPE__ size_t;
typedef __INT64_TYPE__ ptrdiff_t;
#elif __SIZEOF_POINTER__ == 4
typedef __INT32_TYPE__ ptrdiff_t;
#elif __SIZEOF_POINTER__ == 2
typedef __INT16_TYPE__ ptrdiff_t;
#endif

#ifndef __cplusplus
#ifdef _WIN32
typedef short wchar_t;
#else
typedef int wchar_t;
#endif
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

#if defined(__clang__) || defined(__GNUC__)
#define offsetof(st, m) \
    __builtin_offsetof(st, m)
#else
#define offsetof(st, m) \
    ((size_t)((char*)&((st*)0)->m - (char*)0))
#endif 

#ifdef __cplusplus
}
#endif

#endif // STDDEF_H
