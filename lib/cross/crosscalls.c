#include "cross/crosscalls.h"
#include "stddef.h"
#include "stdint.h"
#include "errno.h"

#if defined(__linux__) && defined(__x86_64__)
#include "linux/syscalls.h"
#endif

int64_t cross_write(unsigned int fd, const char* buff, size_t n){
    #if defined(__linux__) && defined(__x86_64__)
    int64_t ret = sys_write(fd, buff, n);
    if(ret < 0){
        errno = (int)(-ret);
        return -1;
    }
    return ret;
    #else
    return -1;
    #endif
}

int64_t cross_read(unsigned int fd, char* buff, size_t n){
    #if defined(__linux__) && defined(__x86_64__)
    int64_t ret = sys_read(fd, buff, n);
    if(ret < 0){
        errno = (int)(-ret);
        return -1;
    }
    return ret;
    #else
    return -1;
    #endif
}

noreturn void cross_exit(int code){
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

typedef struct{
    void* address;
    size_t size;
} ReusableMem;

ReusableMem memArr[128];
size_t reusableMemC = 0;

// ASSUMES CALLER THREAD SAFETY
void* cross_alloc_small(size_t size, size_t* got){
    #if defined(__linux__) && defined(__x86_64__)
    if(!brk_addr){
        brk_addr = (void*)sys_brk(0);
    }

    if(reusableMemC > 0){
        for(size_t i = reusableMemC; i > 0; i--){
            if(memArr[i-1].size > size){
                *got = size;
                void* mem = memArr[i-1].address;

                memArr[i-1].address = (char*)memArr[i-1].address + size;
                memArr[i-1].size -= size;
                return mem;
            }
            else if(memArr[i-1].size == size){
                *got = size;
                void* mem = memArr[i-1].address;
                for(size_t j = i; j + 1 < reusableMemC; j++){
                    memArr[j] = memArr[j + 1];
                }
                reusableMemC--;
                return mem;
            }

        }
    }
    void* start = brk_addr;
    void* goal = (char*)brk_addr + size;

    void* got_brk = (void*)sys_brk((uintptr_t)goal);

    if((uintptr_t)got_brk <= (uintptr_t)brk_addr){
        *got = 0;
        return NULL;
    }
    *got = got_brk - start;
    brk_addr = got_brk;
    return start;
    
    #endif
    *got = 0;
    return NULL;
}

void cross_free_small(void* ptr, size_t size){
    if(size >= MINIMUM_BIG_ALLOC_THRESHOLD){
        return;
    }
    #if defined(__linux__) && defined(__x86_64__)
    if(brk_addr - size == ptr){
        sys_brk((uintptr_t)ptr);
        return;
    }
    if(size < 16) return;
    if(reusableMemC < (sizeof(memArr) / sizeof(ReusableMem))){
        memArr[reusableMemC++] = (ReusableMem){
            .address = ptr,
            .size = size,
        };
    }
    #endif
}

void* cross_alloc(size_t size, size_t* allocated){
    if(size >= MINIMUM_BIG_ALLOC_THRESHOLD){
        return cross_alloc_big(size, allocated);
    }
    return cross_alloc_small(size, allocated);
}

void cross_free(void* ptr, size_t size){
    if(size >= MINIMUM_BIG_ALLOC_THRESHOLD){
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

    int64_t ret = sys_open(name, openType, modeType);
    if(ret < 0){
        errno = (int)(-ret);
        return -1;
    }
    return ret;
    #else
    return -1;
    #endif
}

#if defined(__linux__) && defined(__x86_64__)
const int CROSS_ABORT = 6;
#else
const int CROSS_ABORT = 0;
#endif

void cross_close(int64_t fd){
    #if defined(__linux__) && defined(__x86_64__)
    int64_t ret = sys_close(fd);
    if(ret < 0){
        errno = (int)(-ret);
    }
    #endif
}

int cross_kill(int pid, int sig){
    #if defined(__linux__) && defined(__x86_64__)
    int ret = sys_kill(pid, sig);
    if(ret < 0){
        errno = (int)(-ret);
        return -1;
    }
    return ret;
    #else
    return -1;
    #endif
}

int cross_getpid(){
    #if defined(__linux__) && defined(__x86_64__)
    int ret = sys_pid();
    if(ret < 0){
        errno = (int)(-ret);
        return -1;
    }
    return ret;
    #else
    return -1;
    #endif
}

void cross_unlock(cross_mutex* tx){
    tx->val = 0;
    #if defined(__linux__)

    #if defined(__x86_64__)
    sys_futex(&tx->val, FUTEX_WAKE, 1, NULL, NULL, 0);
    #endif

    #endif
}

void cross_lock(cross_mutex* tx){
    #if defined(__linux__)

    #if defined(__x86_64__)

    while(1){
        int prev;
        int old = 0;
        int new = 1;
        int* ptr = &tx->val;
        __asm__ volatile (
            "lock cmpxchg %2, %1"
            : "=a"(prev), "+m"(*ptr)
            : "r"(new), "0"(old)
            : "memory"
        );

        if(prev == 0){
            return;
        }

        sys_futex(&tx->val, FUTEX_WAIT, 1, NULL, NULL, 0);
    }

    #endif

    #endif
}
