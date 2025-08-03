#ifndef _CROSSCALLS_H
#define _CROSSCALLS_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stddef.h"
#include "stdint.h"
#include "stdnoreturn.h"

typedef struct{
    int val; // 0 unlocked, 1 locked
} cross_mutex;

int64_t cross_write(unsigned int fd, const char* buff, size_t n); 
int64_t cross_read(unsigned int fd, char* buff, size_t n);
int64_t cross_seek(unsigned int fd, ssize_t offset, int whence);
noreturn void cross_exit(int code);

void* cross_alloc_big(size_t size, size_t* allocated);
void cross_free_big(void* ptr, size_t size);

void* cross_alloc_small(size_t size, size_t* got);
void cross_free_small(void* ptr, size_t size);

void* cross_alloc(size_t size, size_t* allocated);
void cross_free(void* ptr, size_t size);

#define CROSS_OPEN_WRITE 0x1
#define CROSS_OPEN_READ 0x2
#define CROSS_OPEN_READXWRITE 0x4
#define CROSS_OPEN_APPEND 0x8

int64_t cross_open(const char* name, int open_type);
void cross_close(int64_t fd);

void cross_lock(cross_mutex* tx);
void cross_unlock(cross_mutex* tx);

int cross_kill(int pid, int sig);
int cross_getpid();

extern const int CROSS_ABORT;

#define MINIMUM_BIG_ALLOC 4096
#define MINIMUM_BIG_ALLOC_THRESHOLD (MINIMUM_BIG_ALLOC * 3 / 4)

#ifdef __cplusplus
}
#endif

#endif // CROSSCALLS_H
