#ifndef STDDEF_H
#define STDDEF_H

#ifdef __cplusplus
extern "C"{
#endif
typedef __SIZE_TYPE__ size_t;

#if __SIZEOF_SIZE_T__ == 8
typedef __INT64_TYPE__ ssize_t;
#elif __SIZEOF_SIZE_T__ == 4
typedef __INT32_TYPE__ ssize_t;
#elif __SIZEOF_SIZE_T__ == 2
typedef __INT16_TYPE__ ssize_t;
#endif

typedef __PTRDIFF_TYPE__ ptrdiff_t;
#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif

#define NULL ((void*)0)

#ifndef __cplusplus
    #if defined(__SIZEOF_INT128__)
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
