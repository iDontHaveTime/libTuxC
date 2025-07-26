#include "stddef.h"
#include "string.h"
#include "stdlib.h"

char* strdup(const char* str){
    if(!str) return NULL;
    size_t len = strlen(str);

    char* alloc_buff = malloc(len + 1);
    if(!alloc_buff) return NULL;
    memcpy(alloc_buff, str, len);
    alloc_buff[len] = '\0';
    return alloc_buff;
}

char* strndup(const char* str, size_t max){
    if(!str) return NULL;
    size_t len = strnlen(str, max);

    char* alloc_buff = malloc(len + 1);
    if(!alloc_buff) return NULL;
    memcpy(alloc_buff, str, len);
    alloc_buff[len] = '\0';
    return alloc_buff;
}