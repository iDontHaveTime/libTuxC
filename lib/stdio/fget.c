#include "stddef.h"
#include "stdio.h"

int fgetc(FILE* fs){
    char b[1];
    return fread(b, 1, 1, fs) == 1 ? b[0] : EOF;
}

int getchar(void){
    return fgetc(stdin);
}

char* fgets(char* buff, size_t n, FILE* fs){
    if(!buff || n == 0) return NULL;

    size_t pos = 0;
    while(pos < n - 1){
        size_t ns = fread(buff + pos, 1, 1, fs);
        if(ns == 0){
            if(pos == 0) return NULL;
            break;
        }
        if(buff[pos] == '\n'){
            pos++;
            break;
        }
        pos++;
    }
    buff[pos] = '\0';
    return buff;
}

char* gets(char* buff){
    size_t pos = 0;
    int ch;

    while((ch = getchar()) != EOF && ch != '\n'){
        buff[pos++] = (char)ch;
    }

    if(ch == EOF && pos == 0) return NULL;

    buff[pos] = '\0';
    return buff;
}

int ungetc(int c, FILE* fs){
    if(fs->buff_ptr != fs->buff_end){
        *fs->buff_ptr = (unsigned char)c;
        return (unsigned char)c;
    }
    return EOF;
}