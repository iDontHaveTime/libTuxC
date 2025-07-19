#include "noncstd/stdiofile.h"
#include "stdio.h"

void clearerr(FILE* fs){
    fs->err = 0;
}

int ferror(FILE* fs){
    return fs->err > 0 ? 1 : 0;
}

int feof(FILE* fs){
    return (fs->err & __FILE_ERR_EOF) ? 1 : 0;
}