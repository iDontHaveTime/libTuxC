#include "assert.h"
#include "stdlib.h"
#include "stdio.h"

noreturn void __assert_fail(const char* expr_s, const char* file, long line, const char* func){
    fprintf(stderr, "%s:%ld %s: Assertion \'%s\' failed.\n", file, line, func, expr_s);
    abort();
}