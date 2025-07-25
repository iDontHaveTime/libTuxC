#include "cross/crosscalls.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

static cross_mutex malloc_lock = {0};

typedef struct{
    size_t size;
    size_t user_request;
} MallocPointer;

void* aligned_malloc(size_t align, size_t size){
    if(size == 0 || align == 0) return NULL;
    cross_lock(&malloc_lock);
    if(align < sizeof(void*)){
        align = sizeof(void*); // little more never hurts anyone
    }
    if((align & (align - 1)) != 0){
        cross_unlock(&malloc_lock);
        return NULL; // align must be power of 2
    }
    size_t request = size + align - 1 + sizeof(MallocPointer);

    size_t got;
    void* raw = cross_alloc(request, &got);

    if(!raw){
        cross_unlock(&malloc_lock);
        return NULL;
    }

    uintptr_t raw_addr = (uintptr_t)raw + sizeof(MallocPointer);
    uintptr_t aligned_addr = (raw_addr + align - 1) & ~(align - 1);

    void* user_ptr = (void*)aligned_addr;
    MallocPointer* header = (MallocPointer*)((char*)user_ptr - sizeof(MallocPointer));
    header->size = got;
    header->user_request = got - ((uintptr_t)user_ptr - (uintptr_t)raw);

    cross_unlock(&malloc_lock);
    return user_ptr;
}

void* malloc(size_t size){
    return aligned_malloc(sizeof(max_align_t), size);
}

MallocPointer* __get_malloc_ptr(void* ptr){
    return (MallocPointer*)((char*)ptr - sizeof(MallocPointer));
}

void* realloc(void* mem, size_t newSize){
    if(!mem) return malloc(newSize);

    cross_lock(&malloc_lock);

    if(newSize == 0){
        cross_unlock(&malloc_lock);
        free(mem);
        return NULL;
    }

    MallocPointer* ptr = __get_malloc_ptr(mem);

    if(newSize <= ptr->user_request){
        cross_unlock(&malloc_lock);
        return mem;
    }

    cross_unlock(&malloc_lock);
    void* newMem = malloc(newSize);
    if(!newMem){
        return NULL;
    }
    
    memcpy(newMem, mem, (newSize < ptr->user_request) ? newSize : ptr->user_request);
    free(mem);
    return newMem;
}

size_t _tuxc_malloc_usersize(void* ptr){
    if(!ptr) return 0;
    return __get_malloc_ptr(ptr)->user_request;
}

size_t _tuxc_malloc_fullsize(void* ptr){
    if(!ptr) return 0;
    return __get_malloc_ptr(ptr)->size;
}

void free(void* ptr){
    if(!ptr) return;
    cross_lock(&malloc_lock);
    MallocPointer* rptr = __get_malloc_ptr(ptr);
    cross_free(rptr, rptr->size);
    cross_unlock(&malloc_lock);
}