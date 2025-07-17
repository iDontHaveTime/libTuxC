#ifndef TUXCMAC_H
#define TUXCMAC_H

#if defined(__GNUC__) || defined(__clang__)
#define __port_attr(x) __attribute__(x)
#else
#define __port_attr(x)
#endif  

#if defined(__linux__) && defined(__x86_64__)

#define TUXC_GP_ARGC 6
#define TUXC_FP_ARGC 8
#define TUXC_ARG_ONE_REG "rdi"
#define TUXC_ARG_TWO_REG "rsi"
#define TUXC_ARG_THREE_REG "rdx"
#define TUXC_ARG_FOUR_REG "rcx"
#define TUXC_ARG_FIVE_REG "r8"
#define TUXC_ARG_SIX_REG "r9"
#define TUXC_FARG_ONE_REG "xmm0"
#define TUXC_FARG_TWO_REG "xmm1"
#define TUXC_FARG_THREE_REG "xmm2"
#define TUXC_FARG_FOUR_REG "xmm3"
#define TUXC_FARG_FIVE_REG "xmm4"
#define TUXC_FARG_SIX_REG "xmm5"
#define TUXC_FARG_SEVEN_REG "xmm6"
#define TUXC_FARG_EIGHT_REG "xmm7"

#define TUXC_GP_REG_ARRAY {ARG_ONE_REG, ARG_TWO_REG, ARG_THREE_REG, ARG_FOUR_REG, ARG_FIVE_REG, ARG_SIX_REG}
#define TUXC_FP_REG_ARRAY {FARG_ONE_REG, FARG_TWO_REG, FARG_THREE_REG, FARG_FOUR_REG, FARG_FIVE_REG, FARG_SIX_REG, FARG_SEVEN_REG, FARG_EIGHT_REG}

#define TUXC_HIDDEN_PTR_SIZE 16

#else

#define TUXC_GP_ARGC 6
#define TUXC_FP_ARGC 8
#define TUXC_GP_REG_ARRAY ((void*)0)
#define TUXC_FP_REG_ARRAY ((void*)0)

#define TUXC_HIDDEN_PTR_SIZE 8

#endif

#endif // TUXCMAC_H
