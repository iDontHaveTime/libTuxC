#ifndef STDARG_H
#define STDARG_H

#ifdef __cplusplus
extern "C"{
#endif

typedef __builtin_va_list va_list;
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg

#ifdef __cplusplus
}
#endif

#endif // STDARG_H
