#include "stddef.h"
#include "noncstd/stdiofile.h"

#define STARTING_STDOUT_BUFF 256
#define STARTING_STDIN_BUFF 1024

char stdout_buff[STARTING_STDOUT_BUFF];
char stdin_buff[STARTING_STDIN_BUFF];

FILE* stdout = &(FILE){
    .fd = 1, 
    .open_mode = __FILE_MODE_WRITE | __FILE_MODE_BINARY | __FILE_MODE_STACK_ALLOC | __FILE_FLUSH_LINE,
    .buff_end = &stdout_buff[STARTING_STDOUT_BUFF-1],
    .buff_ptr = stdout_buff,
    .buff_start = stdout_buff,
    .flushed = 0,
    .lastop = __FILE_LAST_OP_NONE,
    .last_flushed = 0,
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
};

FILE* stdin = &(FILE){
    .fd = 0, 
    .open_mode = __FILE_MODE_READ | __FILE_MODE_BINARY | __FILE_MODE_STACK_ALLOC,
    .buff_end = &stdin_buff[STARTING_STDIN_BUFF-1],
    .buff_ptr = stdin_buff,
    .buff_start = stdin_buff,
    .flushed = 0,
    .lastop = __FILE_LAST_OP_NONE,
    .last_flushed = 0,
};