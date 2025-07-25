#include "assert.h"
#include "stdlib.h"

noreturn void __assert_fail(const char* expr_s, const char* file, long line, const char* func){
    abort();
}