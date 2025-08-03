#include "noncstd/stdiofile.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "cross/crosscalls.h"

size_t fwrite(const void* mem, size_t size, size_t count, FILE* stream){
    if(!mem || !stream || size == 0 || count == 0) return 0;

    if(stream->lastop == __FILE_LAST_OP_READ) return 0;
    if(!(stream->open_mode & __FILE_MODE_WRITE)) return 0;

    if(ferror(stream)) return 0;

    cross_lock(&stream->lock);

    char nl = 0;

    stream->lastop = __FILE_LAST_OP_WRITE;

    size_t total = size * count;
    const char* src = (const char*)mem;

    if(!stream->buff_start){
        int64_t res = cross_write(stream->fd, src, total);
        cross_unlock(&stream->lock);
        if(res < 0) return 0;
        return res / size;
    }

    size_t buflen = stream->buff_end - stream->buff_start;
    size_t bufused = stream->buff_ptr - stream->buff_start;
    size_t bufavail = buflen - bufused;

    if(total == 1){
        if(bufavail >= 1){
            *stream->buff_ptr++ = *(char*)mem;
            cross_unlock(&stream->lock);
            return 1;
        }
    }

    size_t written = 0;

    if(total > bufavail){
        if(bufavail > 0){
            stream->buff_ptr = mempcpy_c(stream->buff_ptr, src, bufavail, '\n', &nl);
            src += bufavail;
            total -= bufavail;
            written += bufavail;
            if(__flush_unlock(stream) == EOF){
                cross_unlock(&stream->lock);
                return written / size;
            }
        }

        while(total >= buflen){
            stream->buff_ptr = mempcpy(stream->buff_start, src, buflen);
            src += buflen;
            total -= buflen;
            written += buflen;
            if(__flush_unlock(stream) == EOF){
                cross_unlock(&stream->lock);
                return written / size;
            }
        }

        if(total > 0){
            stream->buff_ptr = mempcpy_c(stream->buff_start, src, total, '\n', &nl);
            written += total;
        }
    }
    else{
        stream->buff_ptr = mempcpy_c(stream->buff_ptr, src, total, '\n', &nl);
        written += total;
    }

    if(nl && stream->open_mode & __FILE_FLUSH_LINE){
        __flush_unlock(stream);
    }
    cross_unlock(&stream->lock);
    return written / size;
}