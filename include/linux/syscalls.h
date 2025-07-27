#ifndef SYSCALLS_H
#define SYSCALLS_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stddef.h"
#include "stdint.h"
#include "stdnoreturn.h"

static inline int64_t syscall0(uint64_t n){
    int64_t ret;
    register uint64_t rax __asm__("rax") = n;
    __asm__ volatile(
        "syscall"
        : "=a"(ret)
        : "a"(rax)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t syscall1(uint64_t n, uint64_t a1){
    int64_t ret;
    register uint64_t rax __asm__("rax") = n;
    register uint64_t rdi __asm__("rdi") = a1;
    __asm__ volatile(
        "syscall"
        : "=a"(ret)
        : "a"(rax), "D"(rdi)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t syscall2(uint64_t n, uint64_t a1, uint64_t a2){
    int64_t ret;
    register uint64_t rax __asm__("rax") = n;
    register uint64_t rdi __asm__("rdi") = a1;
    register uint64_t rsi __asm__("rsi") = a2;
    __asm__ volatile(
        "syscall"
        : "=a"(ret)
        : "a"(rax), "D"(rdi), "S"(rsi)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t syscall3(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3){
    int64_t ret;
    register uint64_t rax __asm__("rax") = n;
    register uint64_t rdi __asm__("rdi") = a1;
    register uint64_t rsi __asm__("rsi") = a2;
    register uint64_t rdx __asm__("rdx") = a3;
    __asm__ volatile(
        "syscall"
        : "=a"(ret)
        : "a"(rax), "D"(rdi), "S"(rsi), "d"(rdx)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t syscall4(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4){
    int64_t ret;
    register uint64_t rax __asm__("rax") = n;
    register uint64_t rdi __asm__("rdi") = a1;
    register uint64_t rsi __asm__("rsi") = a2;
    register uint64_t rdx __asm__("rdx") = a3;
    register uint64_t r10 __asm__("r10") = a4;
    __asm__ volatile(
        "syscall"
        : "=a"(ret)
        : "a"(rax), "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t syscall5(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5){
    int64_t ret;
    register uint64_t rax __asm__("rax") = n;
    register uint64_t rdi __asm__("rdi") = a1;
    register uint64_t rsi __asm__("rsi") = a2;
    register uint64_t rdx __asm__("rdx") = a3;
    register uint64_t r10 __asm__("r10") = a4;
    register uint64_t r8  __asm__("r8")  = a5;
    __asm__ volatile(
        "syscall"
        : "=a"(ret)
        : "a"(rax), "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10), "r"(r8)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t syscall6(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6){
    int64_t ret;
    register uint64_t rax __asm__("rax") = n;
    register uint64_t rdi __asm__("rdi") = a1;
    register uint64_t rsi __asm__("rsi") = a2;
    register uint64_t rdx __asm__("rdx") = a3;
    register uint64_t r10 __asm__("r10") = a4;
    register uint64_t r8  __asm__("r8")  = a5;
    register uint64_t r9  __asm__("r9")  = a6;
    __asm__ volatile(
        "syscall"
        : "=a"(ret)
        : "a"(rax), "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10), "r"(r8), "r"(r9)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t dynamicSyscall(uint64_t sysnum, int argc,
    uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6){
    switch (argc){
        case 0: return syscall0(sysnum);
        case 1: return syscall1(sysnum, a1);
        case 2: return syscall2(sysnum, a1, a2);
        case 3: return syscall3(sysnum, a1, a2, a3);
        case 4: return syscall4(sysnum, a1, a2, a3, a4);
        case 5: return syscall5(sysnum, a1, a2, a3, a4, a5);
        case 6: return syscall6(sysnum, a1, a2, a3, a4, a5, a6);
        default: return -1; // invalid number of arguments
    }
}

#define EPERM 1 // Operation not permitted
#define ENOENT 2 // No such file or directory
#define ESRCH 3 // No such process
#define EINTR 4 // Interrupted system call
#define EIO 5 // I/O error
#define ENXIO 6 // No such device or address
#define E2BIG 7 // Argument list too long
#define ENOEXEC 8 // Exec format error
#define EBADF 9 // Bad file number
#define ECHILD 10 // No child processes
#define EAGAIN 11 // Try again
#define ENOMEM 12 // Out of memory
#define EACCES 13 // Permission denied
#define EFAULT 14 // Bad address
#define ENOTBLK 15 // Block device required
#define EBUSY 16 // Device or resource busy
#define EEXIST 17 // File exists
#define EXDEV 18 // Cross-device link
#define ENODEV 19 // No such device
#define ENOTDIR 20 // Not a directory
#define EISDIR 21 // Is a directory
#define EINVAL 22 // Invalid argument
#define ENFILE 23 // File table overflow
#define EMFILE 24 // Too many open files
#define ENOTTY 25 // Not a typewriter
#define ETXTBSY 26 // Text file busy
#define EFBIG 27 // File too large
#define ENOSPC 28 // No space left on device
#define ESPIPE 29 // Illegal seek
#define EROFS 30 // Read-only file system
#define EMLINK 31 // Too many links
#define EPIPE 32 // Broken pipe
#define EDOM 33 // Math argument out of domain of func
#define ERANGE 34 // Math result not representable

noreturn static inline void sys_exit(long long code){
    syscall1(60, code);
    __builtin_unreachable();
}

static inline int64_t sys_write(unsigned long long fd, const char* buff, size_t count){
    return syscall3(1, fd, (uint64_t)buff, count);
}

static inline int64_t sys_read(unsigned long long fd, char* buff, size_t count){
    return syscall3(0, fd, (uint64_t)buff, count);
}

static inline int64_t sys_lseek(unsigned long long fd, int64_t offset, uint64_t origin){
    return syscall3(8, fd, offset, origin);
}


// Access Modes (mutually exclusive)
#define OPEN_FLAG_RDONLY 00000000 // Open for reading only
#define OPEN_FLAG_WRONLY 00000001 // Open for writing only
#define OPEN_FLAG_RDWR 00000002 // Open for reading and writing

// Creation/Status Flags (can be OR'd with access modes)
#define OPEN_FLAG_CREAT 00000100 // Create file if it does not exist
#define OPEN_FLAG_EXCL 00000200 // Error if OPEN_FLAG_CREAT and file exists
#define OPEN_FLAG_NOCTTY 00000400 // Don't assign controlling terminal
#define OPEN_FLAG_TRUNC 00001000 // Truncate file to 0 length if it exists
#define OPEN_FLAG_APPEND 00002000 // Append mode - writes to end of file
#define OPEN_FLAG_NONBLOCK 00004000 // Non-blocking I/O
#define OPEN_FLAG_DSYNC 00010000 // Write operations are synchronized to the data on disk
#define OPEN_FLAG_SYNC 00040000 // Write operations are synchronized to the file data and metadata on disk
#define OPEN_FLAG_ASYNC 00020000 // Enable signal-driven I/O

// Linux specific flags
#define OPEN_FLAG_DIRECTORY 00200000 // Must be a directory
#define OPEN_FLAG_NOFOLLOW 00400000 // Don't follow symlinks
#define OPEN_FLAG_CLOEXEC 02000000 // Close-on-exec (important for security/correctness)
#define OPEN_FLAG_TMPFILE 04000000 // Create an unnamed temporary file

// File permission bits
#define OPEN_MODE_IRWXU 0000700 // User: read, write, execute
#define OPEN_MODE_IRUSR 0000400 // User: read permission
#define OPEN_MODE_IWUSR 0000200 // User: write permission
#define OPEN_MODE_IXUSR 0000100 // User: execute permission

#define OPEN_MODE_IRWXG 0000070 // Group: read, write, execute
#define OPEN_MODE_IRGRP 0000040 // Group: read permission
#define OPEN_MODE_IWGRP 0000020 // Group: write permission
#define OPEN_MODE_IXGRP 0000010 // Group: execute permission

#define OPEN_MODE_IRWXO 0000007 // Others: read, write, execute
#define OPEN_MODE_IROTH 0000004 // Others: read permission
#define OPEN_MODE_IWOTH 0000002 // Others: write permission
#define OPEN_MODE_IXOTH 0000001 // Others: execute permission

// Other special permissions
#define OPEN_MODE_ISUID 0004000 // Set user ID on execution
#define OPEN_MODE_ISGID 0002000 // Set group ID on execution
#define OPEN_MODE_ISVTX 0001000 // Sticky bit

static inline int64_t sys_open(const char* fileName, long long flags, long long mode){
    return syscall3(2, (uint64_t)fileName, flags, mode);
}

static inline int64_t sys_close(long long fd){
    return syscall1(3, fd);
}

#define MMAP_PROT_NONE 0x0 // Pages cannot be accessed
#define MMAP_PROT_READ 0x1 // Pages can be read
#define MMAP_PROT_WRITE 0x2 // Pages can be written
#define MMAP_PROT_EXEC 0x4 // Pages can be executed

#define MMAP_FLAG_SHARED 0x01 // Share changes with other processes/file
#define MMAP_FLAG_PRIVATE 0x02 // Changes are private (copy-on-write)
#define MMAP_FLAG_FIXED 0x10 // Use exact address (addr must be page-aligned)
#define MMAP_FLAG_ANONYMOUS 0x20 // Not backed by a file (for anonymous memory/heap)
#define MMAP_FLAG_GROWSDOWN 0x0100 // Stack segment (grows downwards)
#define MMAP_FLAG_LOCKED 0x2000 // Lock pages into RAM
#define MMAP_FLAG_NORESERVE 0x4000 // Do not reserve swap space
#define MMAP_FLAG_POPULATE 0x8000 // Populate (fault in) page tables for a mapping
#define MMAP_FLAG_NONBLOCK 0x10000 // Only effective with MAP_POPULATE
#define MMAP_FLAG_STACK 0x20000 // Used for the process stack

#define SIGABRT 6
#define MMAP_SIZE_MINIMUM 4096

static inline int64_t sys_mmap(uintptr_t addr /* void* */, size_t size, uint64_t prot, uint64_t flags, int64_t fd, uint64_t off){
    return syscall6(9, addr, size, prot, flags, fd, off);
}

static inline int64_t sys_mprotect(uintptr_t addr /* void* */, size_t size, uint64_t prot){
    return syscall3(10, addr, size, prot);
}

static inline int64_t sys_munmap(uintptr_t addr /* void* */, size_t size){
    return syscall2(11, addr, size);
}

static inline int64_t sys_brk(uintptr_t addr){
    return syscall1(12, addr);
}

static inline int64_t sys_kill(uint64_t pid, uint64_t sig){
    return syscall2(62, pid, sig);
}

static inline int64_t sys_pid(){
    return syscall0(39);
}

#define FUTEX_WAIT 0
#define FUTEX_WAKE 1
#define FUTEX_FD 2
#define FUTEX_REQUEUE 3
#define FUTEX_CMP_REQUEUE 4
#define FUTEX_WAKE_OP 5

struct timespec{
    long tv_sec;
    long tv_nsec;
};

static inline int64_t sys_futex(int *uaddr, int futex_op, int val, const struct timespec *timeout, int *uaddr2, int val3){
    return syscall6(202, (uint64_t)uaddr, (uint64_t)futex_op, (uint64_t)val,
                    (uint64_t)timeout, (uint64_t)uaddr2, (uint64_t)val3);
}

#ifdef __cplusplus
}
#endif

#endif // SYSCALLS_H
