#include "cross/crosscalls.h"
#include "stddef.h"
#include "stdio.h"
#include "noncstd/stdiofile.h"
#include "string.h"
#include "stdint.h"

int fputs(const char* str, FILE* fs){
    if(!str || !fs) return EOF;
    if(fs->lastop == __FILE_LAST_OP_READ){
        // libc compliance
        return EOF;
    }
    if(!(fs->open_mode & __FILE_MODE_WRITE)) return EOF;

    size_t slen = strlen(str);

    fs->lastop = __FILE_LAST_OP_WRITE;

    if(fs->buff_start == NULL){
        int64_t res = cross_write(fs->fd, str, slen);
        if(res != slen) return EOF;
        return 0;
    }

    char ffl = 0;
    char fnl = 0;

    size_t buflen = fs->buff_end - fs->buff_start;
    size_t bufused = fs->buff_ptr - fs->buff_start;
    size_t bufavail = buflen - bufused;

    if(slen > bufavail){
        if(bufavail > 0){
            size_t copy_amount = bufavail;

            if(fs->open_mode & __FILE_FLUSH_LINE){
                fs->buff_ptr = mempcpy_c(fs->buff_ptr, str, copy_amount, '\n', &fnl);
            }
            else{
                fs->buff_ptr = mempcpy(fs->buff_ptr, str, copy_amount);
            }

            if(fflush(fs) == EOF) return EOF;

            str += copy_amount;
            slen -= copy_amount;
        }

        while(slen >= buflen){
            if(fs->open_mode & __FILE_FLUSH_LINE){
                fs->buff_ptr = mempcpy_c(fs->buff_start, str, buflen, '\n', &fnl);
            } 
            else{
                fs->buff_ptr = mempcpy(fs->buff_start, str, buflen);
            }
            if(fflush(fs) == EOF) return EOF;

            str += buflen;
            slen -= buflen;
        }

        if(slen > 0){
            if(fs->open_mode & __FILE_FLUSH_LINE){
                fs->buff_ptr = mempcpy_c(fs->buff_start, str, slen, '\n', &fnl);
            }
            else{
                fs->buff_ptr = mempcpy(fs->buff_start, str, slen);
            }
        }
    }
    else{
        fs->buff_ptr = mempcpy_c(fs->buff_ptr, str, slen, '\n', &fnl);
    }

    if(fs->buff_ptr == fs->buff_end){
        ffl = 1;
    }
    if(fnl && (fs->open_mode & __FILE_FLUSH_LINE)){
        ffl = 1;
    }

    if(ffl){
        if(fflush(fs) == EOF) return EOF;
    }

    return 0;
}

int puts(const char* str){
    return (fputs(str, stdout) != EOF ? (putchar('\n') != EOF ? 0 : EOF) : EOF);
}