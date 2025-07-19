#include "cross/crosscalls.h"
#include "stdlib.h"
#include "stdio.h"

typedef void (*void_func_ptr)(void);

void_func_ptr fptrs[ATEXIT_MAX];
size_t fptr_cur = 0;

void call_atexit_funcs(){
    if(fptr_cur == 0) return;
    // C standard: LIFO
    for(int i = fptr_cur; i > 0; i--){
        fptrs[i-1]();
    }
}

int atexit(void (*f)(void)){
    if(fptr_cur >= ATEXIT_MAX){
        return EOF;
    }
    fptrs[fptr_cur++] = f;
    return 0;
}

void exit(int code){
    call_atexit_funcs();

    if(stdout && fflush(stdout) == EOF){
        fputs("Error: fflush EOF encountered\n", stderr); // not buffered unless set
    }

    if(stdout && (stdout->last_flushed != '\n' && stdout->flushed)){
        char nl = '\n';
        cross_write(stdout->fd, &nl, 1);
    }

    if(stderr) fflush(stderr); // if flushable

    fcloseall();

    cross_exit(code);
}

void _exit(int code){
    cross_exit(code);
}

void abort(void){
    cross_kill(cross_getpid(), CROSS_ABORT);
    _exit(1);
}