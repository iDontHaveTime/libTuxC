#include "stdio.h"
#include <stdio.h>

int putchar(int ch){
    return putc(ch, stdout);
}