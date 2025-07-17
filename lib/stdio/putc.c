#include "cross/crosscalls.h"
#include "stddef.h"
#include "stdio.h"
#include "noncstd/stdiofile.h"
#include <stdint.h>

int putc(int ch, FILE* f){
    if(!f) return EOF;
    if(f->lastop == __FILE_LAST_OP_READ) return EOF;
    if(!(f->open_mode & __FILE_MODE_WRITE)) return EOF;

    f->lastop = __FILE_LAST_OP_WRITE;

    if(f->buff_start == NULL){
        int64_t res = cross_write(f->fd, (const char*)&ch, 1);
        if(res != 1){
            return EOF;
        }
        return ch;
    }

    if(f->buff_ptr == f->buff_end){
        fflush(f);
    }
    *f->buff_ptr = ch;
    f->buff_ptr++;
    
    if(f->open_mode & __FILE_FLUSH_LINE && ch == '\n'){
        fflush(f);
    }

    return ch;
}