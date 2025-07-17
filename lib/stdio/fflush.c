#include "cross/crosscalls.h"
#include "stdio.h"
#include "noncstd/stdiofile.h"

int fflush(FILE* fs){
    if(fs->buff_ptr == fs->buff_start) return 0;

    if(!(fs->open_mode & __FILE_MODE_WRITE)) return EOF;

    fs->lastop = __FILE_LAST_OP_NONE;
    fs->last_flushed = *(fs->buff_ptr - 1);

    int64_t cross_res = cross_write(fs->fd, fs->buff_start, fs->buff_ptr - fs->buff_start);
    if(cross_res == -1){
        fs->buff_ptr = fs->buff_start;
        return EOF;
    }
    fs->flushed = 1;
    fs->buff_ptr = fs->buff_start;
    return 0;
}