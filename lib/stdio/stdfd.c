#include "stddef.h"
#include "noncstd/stdiofile.h"

char stdout_buff[BUFSIZ];
char stdin_buff[BUFSIZ];

FILE* stdout = &(FILE){
    .fd = 1, 
    .open_mode = __FILE_MODE_WRITE | __FILE_MODE_BINARY | __FILE_MODE_STACK_ALLOC | __FILE_FLUSH_LINE,
    .buff_end = &stdout_buff[BUFSIZ-1],
    .buff_ptr = stdout_buff,
    .buff_start = stdout_buff,
    .flushed = 0,
    .lastop = __FILE_LAST_OP_NONE,
    .last_flushed = 0,
    .read_ptr = stdout_buff,
    .err = 0,
};

FILE* stderr = &(FILE){
    .fd = 2, 
    .open_mode = __FILE_MODE_WRITE | __FILE_MODE_BINARY,
    .buff_end = NULL,
    .buff_ptr = NULL,
    .buff_start = NULL,
    .flushed = 0,
    .lastop = __FILE_LAST_OP_NONE,
    .last_flushed = 0,
    .read_ptr = NULL,
    .err = 0,
};

FILE* stdin = &(FILE){
    .fd = 0, 
    .open_mode = __FILE_MODE_READ | __FILE_MODE_BINARY | __FILE_MODE_STACK_ALLOC,
    .buff_end = &stdin_buff[BUFSIZ-1],
    .buff_ptr = stdin_buff,
    .buff_start = stdin_buff,
    .flushed = 0,
    .lastop = __FILE_LAST_OP_NONE,
    .last_flushed = 0,
    .read_ptr = stdin_buff,
    .err = 0,
};