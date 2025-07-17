#ifndef STDINT_H
#define STDINT_H

#define INT8_MIN -128
#define INT8_MAX 127

#define UINT8_MAX 255

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

#if defined(_WIN64)
typedef uint64_t uintptr_t;
typedef int64_t intptr_t;
#elif defined(_WIN32)
typedef uint32_t uintptr_t;
typedef int32_t intptr_t;
#elif defined(__x86_64__)
typedef uint64_t uintptr_t;
typedef int64_t intptr_t;
#else
typedef uint32_t uintptr_t;
typedef int32_t intptr_t;
#endif

typedef long long intmax_t;
typedef unsigned long long uintmax_t;

#endif // STDINT_H
