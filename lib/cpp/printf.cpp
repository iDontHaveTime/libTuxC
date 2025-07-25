#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"

struct FMTOUT{
    int flags = 0;
};

extern "C"{

char* vsprintf_engine(const char* fmt, va_list args, size_t* len){
    // formatting, dynamically allocated
    bool fmting = false;
    
    size_t bufsize = BUFSIZ;
    char* buff = (char*)malloc(bufsize);
    size_t l = 0;

    while(*fmt != '\0'){
        if(!fmting){
            if(*fmt == '%'){
                fmting = true;
            }
            else{
                if(l >= bufsize){
                    bufsize <<= 1;
                    void* temp = realloc(buff, bufsize);
                    if(!temp){
                        buff[l-1] = '\0';
                        *len = l;
                        return buff;
                    }
                    buff = (char*)temp;
                }
                buff[l++] = *fmt;
            }
        }
        else{
            fmting = false;
        }
        fmt++;
    }

    *len = l;
    buff[l] = '\0';

    return buff;
}

int vsnprintf(char* str, size_t max, const char* fmt, va_list args){
    size_t len;
    char* pf = vsprintf_engine(fmt, args, &len);
    if(!pf){
        return EOF;
    }

    if(max > 0){
        size_t to_write = (len >= max) ? max - 1 : len;
        memcpy(str, pf, to_write);
        str[to_write] = '\0';
    }
    free(pf);
    return len;
}

int vsprintf(char* str, const char* fmt, va_list args){
    size_t len;
    char* pf = vsprintf_engine(fmt, args, &len);
    if(!pf){
        return EOF;
    }
    memcpy(str, pf, len);
    free(pf);
    return len;
}

int vfprintf(FILE* fs, const char* fmt, va_list args){
    size_t len;
    char* pf = vsprintf_engine(fmt, args, &len);
    if(!pf){
        return EOF;
    }
    if(fwrite(pf, 1, len, fs) != len){
        return EOF;
    }
    free(pf);
    return len;
}

int vprintf(const char* fmt, va_list args){
    return vfprintf(stdout, fmt, args);
}

int snprintf(char* str, size_t max, const char* fmt, ...){
    va_list ls;
    va_start(ls, fmt);
    int p = vsnprintf(str, max, fmt, ls);
    va_end(ls);
    return p;
}

int printf(const char* fmt, ...){
    va_list ls;
    va_start(ls, fmt);
    int p = vprintf(fmt, ls);
    va_end(ls);
    return p;
}

int sprintf(char* str, const char* fmt, ...){
    va_list ls;
    va_start(ls, fmt);
    int p = vsprintf(str, fmt, ls);
    va_end(ls);
    return p;
}

int fprintf(FILE* fs, const char* fmt, ...){
    va_list ls;
    va_start(ls, fmt);
    int p = vfprintf(fs, fmt, ls);
    va_end(ls);
    return p;
}

}