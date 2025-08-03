#ifndef _STDBOOL_H
#define _STDBOOL_H

#define __bool_true_false_are_defined 1
#ifndef __cplusplus
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)

#else
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    typedef _Bool bool;
    #else
    typedef signed char bool;
    #endif
    #define true ((bool)1)
    #define false ((bool)0)
#endif
#endif

#endif // STDBOOL_H
