#ifndef STDIOFILE_H
#define STDIOFILE_H

enum{
    __FILE_MODE_WRITE = 0x1,
    __FILE_MODE_BINARY = 0x2,
    __FILE_MODE_READ = 0x4,
    __FILE_MODE_STACK_ALLOC = 0x8,
    __FILE_FLUSH_LINE = 0x10,
};

enum{
    __FILE_LAST_OP_NONE,
    __FILE_LAST_OP_WRITE,
    __FILE_LAST_OP_READ,
};

typedef struct{
    char* buff_start;
    char* buff_end;
    char* buff_ptr;
    unsigned int fd;
    int open_mode;
    int lastop;
    char flushed;
    char last_flushed;
} FILE;

extern FILE* stdout;
extern FILE* stderr;
extern FILE* stdin;

#endif // STDIOFILE_H
