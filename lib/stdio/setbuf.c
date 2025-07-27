#include "cross/crosscalls.h"
#include "noncstd/stdiofile.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"

int setvbuf(FILE* fs, char* buff, int mode, size_t size){
    if(!fs) return EOF;
    cross_lock(&fs->lock);
    if(__flush_unlock(fs) == EOF){
        cross_unlock(&fs->lock);
        return EOF;
    }
    if(fs->buff_start != NULL && ((fs->buff_start - sizeof(FILE)) != (char*)fs)){
        if(!(fs->open_mode & __FILE_MODE_STACK_ALLOC)){
            free(fs->buff_start);
        }
    }

    fs->open_mode &= ~(__FILE_FLUSH_LINE);
    fs->open_mode &= ~(__FILE_MODE_STACK_ALLOC);

    if(!buff){
        if(mode == _IONBF){
            fs->buff_start = NULL;
            fs->buff_end = NULL;
            fs->buff_ptr = NULL;
            fs->read_ptr = NULL;
        }
        else{
            char* rell = malloc(((size > 0) ? size : BUFSIZ));
            if(!rell){
                cross_unlock(&fs->lock);
                return EOF;
            }
            fs->buff_start = rell;
            fs->buff_ptr = fs->buff_start;
            fs->buff_end = fs->buff_start + ((size > 0) ? size : BUFSIZ);
            fs->read_ptr = fs->buff_start;
            if(mode == _IOLBF){
                fs->open_mode |= __FILE_FLUSH_LINE;
            }
        }
    }
    else{
        if(mode == _IONBF){
            fs->buff_start = NULL;
            fs->buff_end = NULL;
            fs->buff_ptr = NULL;
            fs->read_ptr = NULL;
        }
        else{
            fs->open_mode |= __FILE_MODE_STACK_ALLOC; // stack alloc not freed
            fs->buff_start = buff;
            fs->buff_end = buff + size;
            fs->buff_ptr = buff;
            fs->read_ptr = buff;

            if(mode == _IOLBF){
                fs->open_mode |= __FILE_FLUSH_LINE;
            }
        }
    }
    cross_unlock(&fs->lock);
    return 0;
}

void setbuf(FILE* fs, char* buff){
    setvbuf(fs, buff, buff ? _IOFBF : _IONBF, BUFSIZ);
}

void setbuffer(FILE* fs, char* buff, size_t size){
    setvbuf(fs, buff, buff ? _IOFBF : _IONBF, size);
}

void setlinebuf(FILE* fs){
    setvbuf(fs, NULL, _IOLBF, 0);
}