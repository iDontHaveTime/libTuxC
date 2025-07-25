#ifndef STDIOFILE_H
#define STDIOFILE_H

#ifdef __cplusplus
extern "C"{
#endif

#include "cross/crosscalls.h"
#include "stddef.h"

#define BUFSIZ 4096

enum{
    __FILE_MODE_WRITE = 0x1,
    __FILE_MODE_BINARY = 0x2,
    __FILE_MODE_READ = 0x4,
    __FILE_MODE_STACK_ALLOC = 0x8,
    __FILE_FLUSH_LINE = 0x10,
};

#define _IONBF 0
#define _IOLBF 1
#define _IOFBF 2

enum{
    __FILE_LAST_OP_NONE,
    __FILE_LAST_OP_WRITE,
    __FILE_LAST_OP_READ,
};

enum{
    __FILE_ERR_EOF = 0x1
};

typedef struct{
    char* buff_start;
    char* buff_end;
    char* buff_ptr;
    char* read_ptr;
    size_t arrpos;
    unsigned int fd;
    int open_mode;
    int lastop;
    cross_mutex lock;
    short err;
    char flushed;
    char last_flushed;
} FILE;

extern FILE* stdout;
extern FILE* stderr;
extern FILE* stdin;

#define stdin stdin
#define stdout stdout
#define stderr stderr

#ifdef __cplusplus
}
#endif

#endif // STDIOFILE_H
