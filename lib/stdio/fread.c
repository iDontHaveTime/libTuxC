#include "cross/crosscalls.h"
#include "noncstd/stdiofile.h"
#include "stdio.h"
#include "stddef.h"
#include "stdint.h"
#include "string.h"

int __advance_buffer_read(FILE* f){
    f->read_ptr = f->buff_start;
    int64_t br = cross_read(f->fd, f->buff_start, f->buff_end - f->buff_start);
    if(br == -1){
        f->buff_ptr = f->buff_start;
        f->err |= __FILE_ERR_EOF;
        return EOF;
    }
    f->buff_ptr = f->buff_start + br;
    return 0;
}

size_t fread(void* mem, size_t size, size_t count, FILE* stream){
    if(!mem || !stream || size == 0 || count == 0) return 0;

    if(ferror(stream)) return 0;

    if(!(stream->open_mode & __FILE_MODE_READ)){
        return 0;
    }

    if(stream->lastop == __FILE_LAST_OP_WRITE){
        return 0;
    }

    cross_lock(&stream->lock);

    stream->lastop = __FILE_LAST_OP_READ;

    if(stream->buff_start == NULL){
        int64_t br = cross_read(stream->fd, mem, size * count);
        cross_unlock(&stream->lock);
        if(br <= 0){
            return 0;
        }
        return br / size;
    }

    char* dest = (char*)mem;
    size_t bytesread = 0;

    if(stream->buff_ptr == stream->buff_start){
        __advance_buffer_read(stream);
        if(stream->buff_ptr == stream->buff_start){
            cross_unlock(&stream->lock);
            return 0;
        }
    }

    size_t total = size * count;

    while(bytesread < total){
        // If buffer is empty, refill
        if(stream->read_ptr == stream->buff_ptr){
            if(__advance_buffer_read(stream) == EOF) break;
            if(stream->read_ptr == stream->buff_ptr) break; // EOF
        }

        size_t buf_left = stream->buff_ptr - stream->read_ptr;
        size_t to_copy = total - bytesread;

        if(to_copy > buf_left)
            to_copy = buf_left;

        memcpy(dest + bytesread, stream->read_ptr, to_copy);
        stream->read_ptr += to_copy;
        bytesread += to_copy;
    }
    cross_unlock(&stream->lock);
    return bytesread / size;
}