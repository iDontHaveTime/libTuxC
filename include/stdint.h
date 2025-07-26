#ifndef STDINT_H
#define STDINT_H

#ifdef __cplusplus
extern "C"{
#endif

#define INT8_MIN (-128)
#define INT8_MAX 127
#define UINT8_MAX 255

#define INT16_MIN (-32768)
#define INT16_MAX 32767
#define UINT16_MAX 65535

#define INT32_MIN (-2147483647 - 1)
#define INT32_MAX 2147483647
#define UINT32_MAX 4294967295U

#define INT64_MIN (-9223372036854775807LL - 1)
#define INT64_MAX 9223372036854775807LL
#define UINT64_MAX 18446744073709551615ULL

#define FORCE_STDINT_TYPE
#ifndef FORCE_STDINT_TYPE

typedef unsigned char uint8_t;
typedef signed char int8_t;

typedef unsigned short uint16_t;
typedef signed short int16_t;

#ifdef _WIN32
typedef unsigned long uint32_t;
typedef signed long int32_t;
#else
typedef unsigned int uint32_t;
typedef signed int int32_t;
#endif

#ifdef _WIN32
typedef unsigned long long uint64_t;
typedef signed long long int64_t;
#else
typedef unsigned long uint64_t;
typedef signed long int64_t;
#endif

#else

typedef __UINT8_TYPE__ uint8_t;
typedef __INT8_TYPE__  int8_t;

typedef __UINT16_TYPE__ uint16_t;
typedef __INT16_TYPE__ int16_t;

typedef __UINT32_TYPE__ uint32_t;
typedef __INT32_TYPE__ int32_t;

typedef __UINT64_TYPE__ uint64_t;
typedef __INT64_TYPE__ int64_t;

#endif

#if __SIZEOF_POINTER__ == 8

#define INTPTR_MIN INT64_MIN
#define INTPTR_MAX INT64_MAX
#define UINTPTR_MAX UINT64_MAX

typedef uint64_t uintptr_t;
typedef int64_t intptr_t;
#elif __SIZEOF_POINTER__ == 4

#define INTPTR_MIN INT32_MIN
#define INTPTR_MAX INT32_MAX
#define UINTPTR_MAX UINT32_MAX

typedef uint32_t uintptr_t;
typedef int32_t intptr_t;
#elif __SIZEOF_POINTER__ == 2

#define INTPTR_MIN INT16_MIN
#define INTPTR_MAX INT16_MAX
#define UINTPTR_MAX UINT16_MAX

typedef uint16_t uintptr_t;
typedef int16_t intptr_t;
#endif

#ifdef __SIZEOF_INT128__
typedef __uint128_t uint128_t;
typedef __int128_t int128_t;
#else
typedef uint64_t uint128_t;
typedef int64_t int128_t;
#endif

#ifndef __cplusplus
#ifdef _WIN32
#define WCHAR_MIN INT16_MIN
#define WCHAR_MAX INT16_MAX
#else
#define WCHAR_MIN INT32_MIN
#define WCHAR_MAX INT32_MAX
#endif
#endif

typedef long long intmax_t;
typedef unsigned long long uintmax_t;

#ifdef __cplusplus
}
#endif

#endif // STDINT_H
