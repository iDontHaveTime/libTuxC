#ifndef CROSSCALLS_H
#define CROSSCALLS_H

#include "stddef.h"
#include "stdint.h"

int64_t cross_write(unsigned int fd, const char* buff, size_t n); 
int64_t cross_read(unsigned int fd, char* buff, size_t n);
void cross_exit(int code);

void* cross_alloc_big(size_t size, size_t* allocated);
void cross_free_big(void* ptr, size_t size);

void* cross_alloc_small(size_t size);
void cross_free_small(void* ptr, size_t size);

void* cross_alloc(size_t size, size_t* allocated);
void cross_free(void* ptr, size_t size);

#define CROSS_OPEN_WRITE 0x1
#define CROSS_OPEN_READ 0x2
#define CROSS_OPEN_READXWRITE 0x4
#define CROSS_OPEN_APPEND 0x8

int64_t cross_open(const char* name, int open_type);
void cross_close(int64_t fd);

int cross_kill(int pid, int sig);
int cross_getpid();

extern const int CROSS_ABORT;

#endif // CROSSCALLS_H
