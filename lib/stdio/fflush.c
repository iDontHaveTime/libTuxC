#include "cross/crosscalls.h"
#include "stdio.h"
#include "noncstd/stdiofile.h"

int fflush(FILE* fs){
    if(!fs){
        return __fflush_all();
    }
    cross_lock(&fs->lock);
    fs->read_ptr = fs->buff_start;
    if(fs->buff_ptr == fs->buff_start){
        cross_unlock(&fs->lock);
        return 0;
    }

    if((fs->open_mode & __FILE_MODE_READ) && !(fs->open_mode & __FILE_MODE_WRITE)){
        fs->buff_ptr = fs->buff_start;
        cross_unlock(&fs->lock);
        return 0;
    }

    fs->lastop = __FILE_LAST_OP_NONE;
    fs->last_flushed = *(fs->buff_ptr - 1);

    int64_t cross_res = cross_write(fs->fd, fs->buff_start, fs->buff_ptr - fs->buff_start);
    if(cross_res == -1){
        fs->buff_ptr = fs->buff_start;
        fs->err |= __FILE_ERR_EOF;
        cross_unlock(&fs->lock);
        return EOF;
    }
    fs->flushed = 1;
    fs->buff_ptr = fs->buff_start;
    cross_unlock(&fs->lock);
    return 0;
}