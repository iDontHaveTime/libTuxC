#ifndef STDNORETURN_H
#define STDNORETURN_H

#ifdef __cplusplus
extern "C"{
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    #define noreturn _Noreturn
#elif defined(__GNUC__) || defined(__clang__)
    #define noreturn __attribute__((noreturn))
#elif defined(_MSC_VER)
    #define noreturn __declspec(noreturn)
#else
    #define noreturn
#endif

#ifdef __cplusplus
}
#endif

#endif // STDNORETURN_H
