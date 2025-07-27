#include "cross/crosscalls.h"
#include "noncstd/stdiofile.h"
#include "stdio.h"

int fseek(FILE* fs, long offset, int whence){
    if(!fs) return EOF;

    cross_lock(&fs->lock);

    if(!fs->buff_start){
        // no buffer
        int64_t ret = cross_seek(fs->fd, offset, whence);
        if(ret < 0){
            cross_unlock(&fs->lock);
            return EOF;
        }
        cross_unlock(&fs->lock);
        return 0;
    }

    if(__flush_unlock(fs) == EOF){
        cross_unlock(&fs->lock);
        return EOF;
    }
    int64_t ret = cross_seek(fs->fd, offset, whence);

    cross_unlock(&fs->lock);
    return ret < 0 ? EOF : 0;
}

long ftell(FILE* fs){
    if(!fs) return EOF;

    cross_lock(&fs->lock);

    int64_t pos = cross_seek(fs->fd, 0, SEEK_CUR);
    if(pos < 0){
        cross_unlock(&fs->lock);
        return EOF;
    }

    if(!fs->buff_start){
        // no buffer
        cross_unlock(&fs->lock);
        return (long)pos;
    }

    switch(fs->lastop){
        case __FILE_LAST_OP_READ:
            pos -= (fs->buff_ptr - fs->read_ptr);
            break;
        case __FILE_LAST_OP_WRITE:
            pos += (fs->buff_ptr - fs->buff_start);
            break;
        case __FILE_LAST_OP_NONE:
        default:
            break;
    }

    cross_unlock(&fs->lock);
    return (long)pos;
}

void rewind(FILE* fs){
    if(!fs) return;
    fseek(fs, 0, SEEK_SET);
    clearerr(fs);
}