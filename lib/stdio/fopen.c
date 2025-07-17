#include "cross/crosscalls.h"
#include "stdbool.h"
#include "stdio.h"
#include "noncstd/stdiofile.h"
#include "stddef.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#define DEFAULT_FILE_BUFF 512

FILE* fopen(const char* fileName, const char* mode){
    if(!mode || !fileName) return NULL;
    size_t slen = strlen(mode);
    if(slen > 3 || slen < 1){
        return NULL;
    }

    int openType = 0;
    int fopenmode = 0;
    if(mode[0] == 'w'){
        openType |= CROSS_OPEN_WRITE;
        fopenmode |= __FILE_MODE_WRITE;
    }
    else if(mode[0] == 'r'){
        openType |= CROSS_OPEN_READ;
        fopenmode |= __FILE_MODE_READ;
    }
    else if(mode[0] == 'a'){
        openType |= CROSS_OPEN_APPEND;
        fopenmode |= __FILE_MODE_WRITE;
    }
    else{
        return NULL;
    }

    if(slen > 1){
        if(mode[1] == '+'){
            openType |= CROSS_OPEN_READXWRITE;
            fopenmode |= __FILE_MODE_READ | __FILE_MODE_WRITE;
        }
        else if(mode[1] == 'b'){
            fopenmode |= __FILE_MODE_BINARY;
        }
        else{
            return NULL;
        }
        if(slen > 2){
            if(mode[2] == '+'){
                if(openType & CROSS_OPEN_READXWRITE){
                    return NULL;
                }
                openType |= CROSS_OPEN_READXWRITE;
                fopenmode |= __FILE_MODE_READ | __FILE_MODE_WRITE;
            }
            else if(mode[2] == 'b'){
                if(fopenmode & __FILE_MODE_BINARY){
                    return NULL;
                }
                fopenmode |= __FILE_MODE_BINARY;
            }
            else{
                return NULL;
            }
        }
    }

    int64_t fd = cross_open(fileName, openType);

    if(fd < 0){
        return NULL;
    }

    FILE* f = malloc(sizeof(FILE) + DEFAULT_FILE_BUFF);
    if(!f){
        return NULL;
    }
    f->buff_start = (char*)((FILE*)f + 1);
    f->buff_ptr = f->buff_start;
    f->buff_end = f->buff_start + (DEFAULT_FILE_BUFF-1);
    f->lastop = __FILE_LAST_OP_NONE;
    f->open_mode = fopenmode;
    f->last_flushed = 0;
    f->flushed = 0;
    f->fd = fd;

    return f;
}

int fclose(FILE* fst){
    bool err = false;
    if(fst->open_mode & __FILE_MODE_WRITE){
        if(fflush(fst) == EOF){
            err = true;
        }
    }

    cross_close(fst->fd);
    if(!(fst->open_mode & __FILE_MODE_STACK_ALLOC)){
        free(fst);
    }

    return err ? EOF : 0;
}