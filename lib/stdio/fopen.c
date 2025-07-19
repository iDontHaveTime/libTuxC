#include "cross/crosscalls.h"
#include "stdbool.h"
#include "stdio.h"
#include "noncstd/stdiofile.h"
#include "stddef.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

FILE** farray = NULL;
size_t farray_size = 0;
size_t farray_cur = 0;

int __farray_pushback(FILE* f){
    // THREAD LOCK
    if(!farray){
        farray_size = 16;
        farray = malloc(sizeof(FILE*) * farray_size);
        if(!farray) return EOF;
    }
    if(farray_cur >= farray_size){
        farray_size <<= 1;

        void* ftemp = realloc(farray, sizeof(FILE*) * farray_size);;
        if(!ftemp){
            return EOF;
        }
        farray = ftemp;
    }
    farray[farray_cur++] = f;
    return 0;
    // THREAD UNLOCK
}

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

    FILE* f = malloc(sizeof(FILE) + BUFSIZ);
    if(!f){
        return NULL;
    }

    if(__farray_pushback(f) == EOF){
        fclose(f);
        return NULL;
    }
    f->buff_start = (char*)((FILE*)f + 1);
    f->buff_ptr = f->buff_start;
    f->buff_end = f->buff_start + (BUFSIZ-1);
    f->lastop = __FILE_LAST_OP_NONE;
    f->open_mode = fopenmode;
    f->last_flushed = 0;
    f->flushed = 0;
    f->fd = fd;
    f->err = 0;
    f->read_ptr = f->buff_start;
    f->arrpos = farray_cur - 1;

    return f;
}

int fclose(FILE* fst){
    if(!fst) return EOF;
    bool err = false;
    if(fst->open_mode & __FILE_MODE_WRITE){
        if(fflush(fst) == EOF){
            err = true;
        }
    }

    for(size_t j = fst->arrpos; j + 1 < farray_cur; j++){
        farray[j] = farray[j + 1];
        farray[j]->arrpos = j;
    }
    farray_cur--;

    cross_close(fst->fd);
    if(!(fst->open_mode & __FILE_MODE_STACK_ALLOC)){
        free(fst);
    }

    return err ? EOF : 0;
}

int __fflush_all(void){
    if(farray_cur == 0) return 0;
    int err = 0;
    for(size_t i = 0; i < farray_cur; i++){
        if(fflush(farray[i]) == EOF){
            err = 1;
        }
    }
    return err ? EOF : 0;
}

int fcloseall(void){
    if(farray_cur == 0) return 0;
    int err = 0;
    while(farray_cur){
        if(fclose(farray[0]) == EOF){
            err = 1;
        }
    }
    free(farray);
    farray = NULL;
    farray_size = 0;
    return err ? EOF : 0;
}