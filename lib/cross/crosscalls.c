#include "cross/crosscalls.h"
#include "stddef.h"
#include "stdint.h"

#if defined(__linux__) && defined(__x86_64__)
#include "linux/syscalls.h"
#endif

int64_t cross_write(unsigned int fd, const char* buff, size_t n){
    #if defined(__linux__) && defined(__x86_64__)
    return sys_write(fd, buff, n); 
    #endif
}

int64_t cross_read(unsigned int fd, char* buff, size_t n){
    #if defined(__linux__) && defined(__x86_64__)
    return sys_read(fd, buff, n);
    #endif
}

void cross_exit(int code){
    #if defined(__linux__) && defined(__x86_64__)
    sys_exit((long long)code);
    #endif
}

void* cross_alloc_big(size_t size, size_t* allocated){
    #if defined(__linux__) && defined(__x86_64__)
    if(size < MMAP_SIZE_MINIMUM){
        *allocated = MMAP_SIZE_MINIMUM;
    }
    else{
        size_t times = size / MMAP_SIZE_MINIMUM;
        size_t remainder = size % MMAP_SIZE_MINIMUM;
        *allocated = (times * MMAP_SIZE_MINIMUM) + ((remainder != 0) ? MMAP_SIZE_MINIMUM : 0);
    }
    void* ptr = (void*)sys_mmap(0, *allocated, MMAP_PROT_READ | MMAP_PROT_WRITE, MMAP_FLAG_ANONYMOUS | MMAP_FLAG_PRIVATE, -1, 0);
    return ptr == (void*)-1 ? NULL : ptr;
    #endif
    return NULL;
}

void cross_free_big(void* ptr, size_t size){
    #if defined(__linux__) && defined(__x86_64__)
    sys_munmap((uintptr_t)ptr, size);
    #endif
}

#if defined(__linux__) && defined(__x86_64__)
void* brk_addr = NULL;
#endif

void* cross_alloc_small(size_t size){
    #if defined(__linux__) && defined(__x86_64__)
    if(!brk_addr){
        brk_addr = (void*)sys_brk(0);
    }
    void* start = brk_addr;
    void* goal = (char*)brk_addr + size;

    void* got_brk = (void*)sys_brk((uintptr_t)goal);

    if((uintptr_t)got_brk < (uintptr_t)brk_addr){
        return NULL;
    }

    brk_addr = got_brk;
    return start;
    
    #endif
    return NULL;
}

void cross_free_small(void* ptr, size_t size){
    #if defined(__linux__) && defined(__x86_64__)
    // FUTURE
    #endif
}

void* cross_alloc(size_t size, size_t* allocated){
    if(size >= MMAP_SIZE_MINIMUM){
        return cross_alloc_big(size, allocated);
    }
    // assume 1:1 allocation for now
    *allocated = size;
    return cross_alloc_small(size);
}

void cross_free(void* ptr, size_t size){
    if(size >= MMAP_SIZE_MINIMUM){
        cross_free_big(ptr, size);
    }
    else{
        cross_free_small(ptr, size);
    }
}

int64_t cross_open(const char* name, int open_type){
    #if defined(__linux__) && defined(__x86_64__)
    uint64_t openType = OPEN_FLAG_CLOEXEC;
    uint64_t modeType = 0;
    
    if(open_type & CROSS_OPEN_READXWRITE){
        openType |= OPEN_FLAG_RDWR;
        if(open_type & CROSS_OPEN_WRITE){
            openType |= OPEN_FLAG_CREAT | OPEN_FLAG_TRUNC;
        }
        else if(open_type & CROSS_OPEN_APPEND){
            openType |= OPEN_FLAG_CREAT | OPEN_FLAG_APPEND;
        }
    }
    else if(open_type & CROSS_OPEN_READ){
        openType |= OPEN_FLAG_RDONLY;
    }
    else if(open_type & CROSS_OPEN_WRITE){
        openType |= OPEN_FLAG_WRONLY | OPEN_FLAG_CREAT | OPEN_FLAG_TRUNC;
    }
    else if(open_type & CROSS_OPEN_APPEND){
        openType |= OPEN_FLAG_WRONLY | OPEN_FLAG_CREAT | OPEN_FLAG_APPEND;
    }

    if(openType & OPEN_FLAG_CREAT){
        modeType = OPEN_MODE_IRUSR | OPEN_MODE_IWUSR | OPEN_MODE_IRGRP | OPEN_MODE_IROTH;
    }

    return sys_open(name, openType, modeType);
    #endif
    return -1;
}

#if defined(__linux__) && defined(__x86_64__)
const int CROSS_ABORT = 6;
#else
const int CROSS_ABORT = 0;
#endif

void cross_close(int64_t fd){
    #if defined(__linux__) && defined(__x86_64__)

    sys_close(fd);
    #endif
}

int cross_kill(int pid, int sig){
    #if defined(__linux__) && defined(__x86_64__)
    return sys_kill(pid, sig);
    #endif
    return -1;
}

int cross_getpid(){
    #if defined(__linux__) && defined(__x86_64__)
    return sys_pid();
    #endif
    return -1;
}
