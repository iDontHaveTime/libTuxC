#ifndef STKCGEN_H
#define STKCGEN_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"

uintptr_t __stack_chk_guard_gen();

#ifdef __cplusplus
}
#endif

#endif // STKCGEN_H
