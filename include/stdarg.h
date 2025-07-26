#ifndef STDARG_H
#define STDARG_H

#ifdef __cplusplus
extern "C"{
#endif

typedef __builtin_va_list va_list;
#define va_copy(dest, src) __builtin_va_copy(dest, src)
#define va_start(ap, arg) __builtin_va_start(ap, arg)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)

#ifdef __cplusplus
}
#endif

#endif // STDARG_H
