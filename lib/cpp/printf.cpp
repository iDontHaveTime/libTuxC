#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "stddef.h"

// %[flags][width][.precision][length]specifier
enum FMTSTATE : uint8_t{
    FLAGS = 1, WIDTH = 2, PRECISION = 3, LENGTH = 4, SPECIFIER = 5
};

enum FMTFLAG : int{
    LEFT_JUSTIFY = 0x1, FORCE_SIGN = 0x2, NO_SIGN_SPACE = 0x4, HASHTAG = 0x8, PAD_ZERO = 0x10
};

enum FMTLENGTH : uint8_t{
    l_normal = 0, l_hh = 1, l_h = 2, l_l = 3, l_ll = 4, l_j = 5, l_z = 6, l_t = 7, l_L = 8
};

struct FMTOUT{
    char* buffcpy;
    char* allocbuff = nullptr;
    size_t alloc_size = BUFSIZ;
    size_t buffsize = 0;
    size_t l = 0;
    bool err = false;

    inline bool addstr(const char* str){
        while(*str){
            if(addchar(*str)){
                return true;
            }
            str++;
        }
        return false;
    }

    inline bool addchar_noindex(char c){
        if(l < buffsize){
            buffcpy[l] = c;
        }
        else{
            if(!allocbuff){
                allocbuff = (char*)malloc(alloc_size);
                if(!allocbuff){
                    err = true;
                    return true;
                }
            }
            if((l - buffsize) >= alloc_size){
                alloc_size <<= 1;
                void* rall = realloc(allocbuff, alloc_size);
                if(!rall){
                    err = true;
                    return true;
                }
                allocbuff = (char*)rall;
            }
            allocbuff[(l - buffsize)] = c;
        }
        return false;
    }

    inline bool addchar(char c){
        bool f = addchar_noindex(c);
        l++;
        return f;
    }

    operator bool() const{
        return !err;
    }
};

struct FMTSTR{
    FMTOUT* out;
    int flags = 0;
    int width = 0;
    int precision = -1;
    uint8_t state = FLAGS;
    bool done = false;
    FMTLENGTH length = l_normal;

    inline bool processchar(char c, va_list ls){
        if(state == FLAGS){
            switch(c){
                case '-': flags |= LEFT_JUSTIFY; break;
                case '+': flags |= FORCE_SIGN; break;
                case ' ': flags |= NO_SIGN_SPACE; break;
                case '#': flags |= HASHTAG; break;
                case '0': flags |= PAD_ZERO; break;
                default:
                    state = WIDTH;
                    break;
            }
        }
        if(state == WIDTH){
            switch(c){
                case '*':
                    width = va_arg(ls, int);
                    [[fallthrough]];
                case '.':
                    state = PRECISION;
                    return false;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    width *= 10;
                    width += c - '0';
                    break;
                default:
                    state = PRECISION;
                    break;
            }
        }
        
        if(state == PRECISION){
            switch(c){
                case '*':
                    precision = va_arg(ls, int);
                    state = LENGTH;
                    break;
                case '.':
                    return false;
                case '0':
                    [[fallthrough]];
                case '1':
                    [[fallthrough]];
                case '2':
                    [[fallthrough]];
                case '3':
                    [[fallthrough]];
                case '4':
                    [[fallthrough]];
                case '5':
                    [[fallthrough]];
                case '6':
                    [[fallthrough]];
                case '7':
                    [[fallthrough]];
                case '8':
                    [[fallthrough]];
                case '9':
                    if(precision < 0){
                        precision = 0;
                    }
                    precision *= 10;
                    precision += c - '0';
                    break;
                default:
                    state = LENGTH;
                    break;
            }
        }
        
        if(state == LENGTH){
            switch(c){
                case 'h':
                    if(length == l_normal){
                        length = l_h;
                    }
                    else if(length == l_h){
                        length = l_hh;
                        state = SPECIFIER;
                    }
                    else{
                        state = SPECIFIER;
                    }
                    return false;
                case 'l':
                    if(length == l_normal)
                        length = l_l;
                    else if(length == l_l){
                        length = l_ll;
                        state = SPECIFIER;
                    }
                    else
                        state = SPECIFIER;
                    return false;
                case 'L':
                    length = l_L;
                    state = SPECIFIER;
                    return false;
                case 'j':
                    length = l_j;
                    state = SPECIFIER;
                    return false;
                case 'z':
                    length = l_z;
                    state = SPECIFIER;
                    return false;
                case 't':
                    length = l_t;
                    state = SPECIFIER;
                    return false;
                default:
                    state = SPECIFIER;
                    break;
            }

        }
        if(state == SPECIFIER){
            done = true;
            return handle_specifier(c, ls);
        }
        return false;
    }

    inline void wchar_to_str(const wchar_t* wstr, size_t len){
        while(len--){
            wchar_t wc = *wstr++;

            if(sizeof(wchar_t) == 2){
                if(wc >= (wchar_t)0xD800 && wc <= (wchar_t)0xDBFF){
                    wchar_t low = *wstr++;
                    if(low >= (wchar_t)0xDC00 && low <= (wchar_t)0xDFFF){

                        uint32_t codepoint = 0x10000 + (((wc - 0xD800) << 10) | (low - 0xDC00));

                        out->addchar((char)(0xF0 | ((codepoint >> 18) & 0x07)));
                        out->addchar((char)(0x80 | ((codepoint >> 12) & 0x3F)));
                        out->addchar((char)(0x80 | ((codepoint >> 6) & 0x3F)));
                        out->addchar((char)(0x80 | (codepoint & 0x3F)));

                        continue;
                    } 
                    else{

                        out->addchar('?');
                        wstr--;
                        continue;
                    }
                } 
                else if(wc >= (wchar_t)0xDC00 && wc <= (wchar_t)0xDFFF){
                    out->addchar('?');
                    continue;
                }
            }

            if(wc <= (wchar_t)0x7F){
                out->addchar((char)wc);
            } 
            else if(wc <= (wchar_t)0x7FF){
                out->addchar((char)(0xC0 | (wc >> 6)));
                out->addchar((char)(0x80 | (wc & 0x3F)));
            } 
            else{
                out->addchar((char)(0xE0 | (wc >> 12)));
                out->addchar((char)(0x80 | ((wc >> 6) & 0x3F)));
                out->addchar((char)(0x80 | (wc & 0x3F)));
            }
        }
    }

    inline bool addpadding(char p, size_t pad){
        while(pad--){
            if(out->addchar(p)) return true;
        }
        return false;
    }

    inline bool handle_str(va_list ls){
        size_t len = 0;
        const void* s = nullptr;
        if(length == l_l){
            const wchar_t* str = va_arg(ls, const wchar_t*);
            if(!str){
                str = L"(null)";
            }
            s = str;
            while(str[len] != L'\0'){
                len++;
            }
        }
        else{
            const char* str = va_arg(ls, const char*);
            
            if(!str){
                str = "(null)";
                len = sizeof("(null)") - 1;
            }
            else{
                len = strlen(str);
            }
            s = str;
        }

        if(!s) return true;
        
        if(precision != -1){
            if(precision < len){
                len = precision;
            }
        }

        if(len < width && !(flags & LEFT_JUSTIFY)){
            if(addpadding(' ', width - len)) return true;
        }

        if(length == l_l){
            wchar_to_str((const wchar_t*)s, len);
        }
        else{
            const char* ptr = (char*)s;
            const char* end = ptr + len;
            while(ptr != end){
                if(out->addchar(*ptr)){
                    return true;
                }
                ptr++;
            }
        }

        if(len < width && (flags & LEFT_JUSTIFY)){
            if(addpadding(' ', width - len)) return true;
        }

        return false;
    }

    inline bool handle_char(va_list ls){

        if(width > 1 && !(flags & LEFT_JUSTIFY)){
            if(addpadding(' ', width - 1)) return true;
        }

        int c = va_arg(ls, int);

        if(length == l_l){
            wchar_to_str((const wchar_t*)&c, 1);
        }
        else{
            if(out->addchar(c)){
                return true;
            }
        }

        if(width > 1 && (flags & LEFT_JUSTIFY)){
            if(addpadding(' ', width - 1)) return true;
        }
        return false;
    }

    inline bool handle_n(va_list ls){
        #ifndef __PRINTF_CPP_N_FMT_ENABLE__
        
        return false;
        #else
        switch(length){
            case l_normal:
                *(int*)va_arg(ls, int*) = out->l;
                return false;
            case l_hh:
                *(signed char*)va_arg(ls, signed char*) = out->l;
                return false;
            case l_h:
                *(short*)va_arg(ls, short*) = out->l;
                return false;
            case l_l:
                *(long*)va_arg(ls, long*) = out->l;
                return false;
            case l_j:
                [[fallthrough]];
            case l_ll:
                *(long long*)va_arg(ls, long long*) = out->l;
                return false;
            case l_t:
                *(ptrdiff_t*)va_arg(ls, ptrdiff_t*) = out->l;
                return false;
            case l_z:
                *(ssize_t*)va_arg(ls, ssize_t*) = out->l;
                return false;
            default:
                return true;
        }
        return false;
        #endif
    }

    inline bool handle_int(va_list ls){
        intmax_t i = 0;

        char buff[128];
        char* buffptr = buff;

        switch(length){
            case l_hh:
                [[fallthrough]];
            case l_h:
                [[fallthrough]];
            case l_normal:
                i = va_arg(ls, int);
                break;
            case l_l:
                i = va_arg(ls, long);
                break;
            case l_ll:
                i = va_arg(ls, long long);
                break;
            case l_j:
                i = va_arg(ls, intmax_t);
                break;
            case l_z:
                i = va_arg(ls, ssize_t);
                break;
            case l_t:
                i = va_arg(ls, ptrdiff_t);
                break;
            case l_L:
                [[fallthrough]];
            default: return true;
        }

        bool sign = i < 0;
        if(sign) i = -i;
        bool signcount = false;

        if(flags & FORCE_SIGN){
            signcount = true;
            *buffptr++ = sign ? '-' : '+';
        }
        else if(flags & NO_SIGN_SPACE){
            signcount = true;
            *buffptr++ = sign ? '-' : ' ';
        }
        else{
            if(sign){
                signcount = true;
                *buffptr++ = '-';
            }
        }
        llutoa(i, buffptr, 10);
         
        size_t num_len = strlen(buffptr);
        size_t total_len = signcount + num_len;
        size_t zero_pad = 0;
        size_t space_pad = 0;

        if(precision != -1){
            if(num_len < (size_t)precision){
                zero_pad = precision - num_len;
            }
        }
        else if((flags & PAD_ZERO) && !(flags & LEFT_JUSTIFY)){
            if(width > total_len){
                zero_pad = width - total_len;
            }
        }

        total_len += zero_pad;

        if(width > total_len){
            space_pad = width - total_len;
        }

        if(!(flags & LEFT_JUSTIFY)){
            if(addpadding(' ', space_pad)) return true;
        }

        if(signcount){
            if(out->addchar(buff[0])) return true;
        }

        for(size_t z = 0; z < zero_pad; z++){
            if(out->addchar('0')) return true;
        }

        for(size_t d = 0; d < num_len; d++){
            if(out->addchar(buffptr[d])) return true;
        }

        if(flags & LEFT_JUSTIFY){
            if(addpadding(' ', space_pad)) return true;
        }
        return false;
    }

    inline bool handle_uint(va_list ls){
        uintmax_t u = 0;
        char buff[128];
        char* buffptr = buff;

        switch(length){
            case l_hh: u = (unsigned char)va_arg(ls, unsigned int); break;
            case l_h:  u = (unsigned short)va_arg(ls, unsigned int); break;
            case l_normal: u = va_arg(ls, unsigned int); break;
            case l_l:  u = va_arg(ls, unsigned long); break;
            case l_ll: u = va_arg(ls, unsigned long long); break;
            case l_j:  u = va_arg(ls, uintmax_t); break;
            case l_z:  u = va_arg(ls, size_t); break;
            case l_t:  u = (uintmax_t)va_arg(ls, ptrdiff_t); break;
            default: return true; // unsupported
        }

        llutoa(u, buffptr, 10);

        size_t num_len = strlen(buffptr);
        size_t zero_pad = 0;
        size_t space_pad = 0;

        if(precision != -1){
            if(num_len < (size_t)precision){
                zero_pad = precision - num_len;
            }
        }
        else if((flags & PAD_ZERO) && !(flags & LEFT_JUSTIFY)){
            if(width > num_len){
                zero_pad = width - num_len;
            }
        }

        size_t total_len = num_len + zero_pad;

        if(width > total_len){
            space_pad = width - total_len;
        }

        if(!(flags & LEFT_JUSTIFY)){
            if(addpadding(' ', space_pad)) return true;
        }

        for(size_t z = 0; z < zero_pad; z++){
            if(out->addchar('0')) return true;
        }

        for(size_t d = 0; d < num_len; d++){
            if(out->addchar(buffptr[d])) return true;
        }

        if(flags & LEFT_JUSTIFY){
            if(addpadding(' ', space_pad)) return true;
        }

        return false;
    }

    inline bool handle_specifier(char c, va_list ls){
        switch(c){
            case '%':
                out->addchar('%');
                return false;
            case 's':
                return handle_str(ls);
            case 'c':
                return handle_char(ls);
            case 'n':
                return handle_n(ls);
            case 'i':
                [[fallthrough]];
            case 'd':
                return handle_int(ls);
            case 'u':
                return handle_uint(ls);
            default:
                return true;
        }
    }
};

FMTOUT vsprintf_engine(char* buff, size_t bufsize, const char* fmt, va_list args){
    // formatting, dynamically allocated
    bool fmting = false;
    FMTOUT out;
    out.buffcpy = buff;
    out.buffsize = bufsize;

    while(*fmt != '\0'){
        if(!fmting){
            if(*fmt == '%'){
                fmting = true;
            }
            else{
                out.addchar(*fmt);
            }
        }
        else{
            FMTSTR fv;
            fv.out = &out;

            while(!fv.done){
                if(fv.processchar(*fmt, args)){
                    out.addchar_noindex('\0');
                    return out;
                }
                fmt++;
            }
            
            fmting = false;
            continue;
        }
        fmt++;
    }

    out.addchar_noindex('\0');

    return out;
}

extern "C"{



int vsnprintf(char* str, size_t max, const char* fmt, va_list args){
    char buff[__PRINTF_BUFFER_SIZE__];
    FMTOUT pf = vsprintf_engine(buff, sizeof(buff), fmt, args);
    if(!pf){
        if(pf.allocbuff) free(pf.allocbuff);
        return EOF;
    }

    if(max > 0){
        size_t to_write = (pf.l >= max) ? max - 1 : pf.l;
        if(to_write > pf.buffsize){
            memcpy(str, pf.buffcpy, pf.buffsize);
            to_write -= pf.buffsize;
            memcpy(str + pf.buffsize, pf.allocbuff, to_write);
        }
        else{
            memcpy(str, pf.buffcpy, to_write);
        }
        str[to_write] = '\0';
    }
    if(pf.allocbuff) free(pf.allocbuff);
    return pf.l;
}

int vsprintf(char* str, const char* fmt, va_list args){
    char buff[__PRINTF_BUFFER_SIZE__];
    FMTOUT pf = vsprintf_engine(buff, sizeof(buff), fmt, args);
    if(!pf){
        if(pf.allocbuff) free(pf.allocbuff);
        return EOF;
    }
    if(pf.l > pf.buffsize){
        memcpy(str, pf.buffcpy, pf.buffsize);
        memcpy(str + pf.buffsize, pf.allocbuff, pf.l - pf.buffsize);
    }
    else{
        memcpy(str, pf.buffcpy, pf.l);
    }
    if(pf.allocbuff) free(pf.allocbuff);
    return pf.l;
}

int vfprintf(FILE* fs, const char* fmt, va_list args){
    char buff[__PRINTF_BUFFER_SIZE__];
    FMTOUT pf = vsprintf_engine(buff, sizeof(buff), fmt, args);
    if(!pf){
        if(pf.allocbuff) free(pf.allocbuff);
        return EOF;
    }
    if(pf.l > pf.buffsize){
        if(fwrite(pf.buffcpy, 1, pf.buffsize, fs) != pf.buffsize){
            if(pf.allocbuff) free(pf.allocbuff);
            return EOF;
        }
        if(fwrite(pf.allocbuff, 1, pf.l - pf.buffsize, fs) != pf.l - pf.buffsize){
            if(pf.allocbuff) free(pf.allocbuff);
            return EOF;
        }
    }
    else{
        if(fwrite(pf.buffcpy, 1, pf.l, fs) != pf.l){
            return EOF;
        }
    }
    if(pf.allocbuff) free(pf.allocbuff);
    return pf.l;
}

int vprintf(const char* fmt, va_list args){
    return vfprintf(stdout, fmt, args);
}

int snprintf(char* str, size_t max, const char* fmt, ...){
    va_list ls;
    va_start(ls, fmt);
    int p = vsnprintf(str, max, fmt, ls);
    va_end(ls);
    return p;
}

int printf(const char* fmt, ...){
    va_list ls;
    va_start(ls, fmt);
    int p = vprintf(fmt, ls);
    va_end(ls);
    return p;
}

int sprintf(char* str, const char* fmt, ...){
    va_list ls;
    va_start(ls, fmt);
    int p = vsprintf(str, fmt, ls);
    va_end(ls);
    return p;
}

int fprintf(FILE* fs, const char* fmt, ...){
    va_list ls;
    va_start(ls, fmt);
    int p = vfprintf(fs, fmt, ls);
    va_end(ls);
    return p;
}

}