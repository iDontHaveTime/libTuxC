#include "string.h"
#include "stdint.h"
#include "stdio.h"

short __tuxc_align_lookup8[8] = {
    8, 1, 2, 1, 4, 1, 2, 1
};

size_t strlen(const char* str){
    return strnlen(str, UINTPTR_MAX);
}

size_t strnlen(const char* str, size_t max){
    if(!str) return 0;
    const char* original_str = str;
    size_t c = 0;
    short alignment = __tuxc_getalignment8(str);

    alignment = alignment > sizeof(void*) ? sizeof(void*) : alignment;

    #if defined(__TUXC_16_BYTES_ALLOWED__)
    // soon
    #endif

    switch(alignment){
        case 1:
            while(*str){
                str++;
                c++;
                if(c >= max){
                    return max;
                }
            }
            break;
        case 2:
            while(1){
                uint16_t val = *(const uint16_t*)str;
                uint16_t b2 = ((val - 0x0101) & ~val & 0x8080);

                if(b2 != 0){
                    if(b2 & 0x0080){
                        #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
                        str++;
                        c++;
                        #endif
                        break;
                    }
                    else{
                        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                        str++;
                        c++;
                        #endif
                        break;
                    }
                }
                str += 2;
                c += 2;
                if(c >= max){
                    return max;
                }
            }
            break;
        case 4:
            while(1){
                uint32_t val = *(const uint32_t*)str;
                uint32_t b2 = ((val - 0x01010101) & ~val & 0x80808080);

                if(b2 != 0){
                    if(b2 & 0x00000080){
                        #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
                        str += 3;
                        c += 3;
                        #endif
                        break;
                    }
                    else if(b2 & 0x00008000){
                        #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
                        str += 2;
                        c += 2;
                        #else
                        str++;
                        c++;
                        #endif
                        break;
                    }
                    else if(b2 & 0x00800000){
                        #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
                        str++;
                        c++;
                        #else
                        str += 2;
                        c += 2;
                        #endif
                        break;
                    }
                    else{
                        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                        str += 3;
                        c += 3;
                        #endif
                        break;
                    }
                }
                str += 4;
                c += 4;
                if(c >= max){
                    return max;
                }
            }
            break;
        case 8:
            while(1){
                uint64_t val = *(const uint64_t*)str;
                uint64_t b2 = ((val - 0x0101010101010101ULL) & ~val & 0x8080808080808080ULL);

                if(b2 != 0){
                    if(b2 & 0x0000000000000080){
                        #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
                        str += 7;
                        c += 7;
                        #endif
                        break;
                    }
                    else if(b2 & 0x0000000000008000){
                        #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
                        str += 6;
                        c += 6;
                        #else
                        str++;
                        c++;
                        #endif
                        break;
                    }
                    else if(b2 & 0x0000000000800000){
                        #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
                        str += 5;
                        c += 5;
                        #else
                        str += 2;
                        c += 2;
                        #endif
                        break;
                    }
                    else if(b2 & 0x0000000080000000){
                        #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
                        str += 4;
                        c += 4;
                        #else
                        str += 3;
                        c += 3;
                        #endif
                        break;
                    }
                    else if(b2 & 0x0000008000000000){
                        #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
                        str += 3;
                        c += 3;
                        #else
                        str += 4;
                        c += 4;
                        #endif
                        break;
                    }
                    else if(b2 & 0x0000800000000000){
                        #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
                        str += 2;
                        c += 2;
                        #else
                        str += 5;
                        c += 5;
                        #endif
                        break;
                    }
                    else if(b2 & 0x0080000000000000){
                        #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
                        str++;
                        c++;
                        #else
                        str += 6;
                        c += 6;
                        #endif
                        break;
                    }
                    else{
                        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                        str += 7;
                        c += 7;
                        #endif
                        break;
                    }
                }
                str += 8;
                c += 8;
                if(c >= max){
                    return max;
                }
            }
            break;
    }

    return str - original_str;
}