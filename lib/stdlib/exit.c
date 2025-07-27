#include "cross/crosscalls.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "runtime.h"

static cross_mutex exit_lock = {0};

typedef void (*void_func_ptr)(void);

static cxa_atexit_entry_t cxa_atexit_funcs[MAX_CXA_ATEXIT_FUNCS];
static size_t cxa_atexit_count = 0;

void __cxa_finalize(void *dso_handle){
    cross_lock(&exit_lock);
    for(size_t i = 0; i < cxa_atexit_count;){
        if(cxa_atexit_funcs[i].dso_handle == dso_handle || dso_handle == NULL){
            cxa_atexit_funcs[i].func(cxa_atexit_funcs[i].arg);

            cxa_atexit_funcs[i] = cxa_atexit_funcs[cxa_atexit_count - 1];
            cxa_atexit_count--;
        } 
        else{
            i++;
        }
    }
    cross_unlock(&exit_lock);
}


int __cxa_atexit(void (*func)(void*), void* arg, void* dso_handle){
    cross_lock(&exit_lock);
    if(cxa_atexit_count >= MAX_CXA_ATEXIT_FUNCS){
        cross_unlock(&exit_lock);
        return EOF;
    }
    cxa_atexit_funcs[cxa_atexit_count].func = func;
    cxa_atexit_funcs[cxa_atexit_count].arg = arg;
    cxa_atexit_funcs[cxa_atexit_count].dso_handle = dso_handle;
    cxa_atexit_count++;
    cross_unlock(&exit_lock);
    return 0;
}

void_func_ptr fptrs[ATEXIT_MAX];
size_t fptr_cur = 0;

void call_atexit_funcs(){
    if(fptr_cur == 0) return;
    // C standard: LIFO
    for(int i = fptr_cur; i > 0; i--){
        fptrs[i-1]();
    }
    fptr_cur = 0;
}

int atexit(void (*f)(void)){
    cross_lock(&exit_lock);
    if(fptr_cur >= ATEXIT_MAX){
        cross_unlock(&exit_lock);
        return EOF;
    }
    fptrs[fptr_cur++] = f;
    cross_unlock(&exit_lock);
    return 0;
}

void run_destructors(void){
    if(!__fini_array_start || !__fini_array_end) return;
    for(ctor_t* dtor = __fini_array_end; dtor > __fini_array_start; dtor--){
        (*(dtor - 1))();
    }
}

noreturn void exit(int code){
    __cxa_finalize(NULL);
    //run_cxa_atexit_destructors();
    call_atexit_funcs();
    run_destructors();

    if(stdout){
        if(fflush(stdout) == EOF){
            fputs("Error: fflush EOF encountered\n", stderr); // not buffered unless set
        }
        if(stdout->last_flushed != '\n'){
            char nl = '\n';
            cross_write(stdout->fd, &nl, 1);
        }
    }


    if(stderr) fflush(stderr); // if flushable

    fcloseall();

    _exit(code);
}

noreturn void _exit(int code){
    cross_exit(code);
}

noreturn void abort(void){
    cross_kill(cross_getpid(), CROSS_ABORT);
    _exit(1);
}