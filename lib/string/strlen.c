#include "string.h"
#include "stdint.h"

uint16_t alignmentLookup[8] = {
    8, 1, 2, 1, 4, 1, 2, 1
};

uint16_t get_alignment(void* ptr){
    return alignmentLookup[(uintptr_t)ptr & 0x7];
}

size_t strlen(const char* str){
    if(!str) return 0;
    const char* original_str = str;
    uintptr_t addr = (uintptr_t)str;
    uint16_t alignment = alignmentLookup[addr & 0x7];

    switch(alignment){
        case 4:
        case 8:
        case 1:
            while(*str != '\0') str++;
            break;
        case 2:
            while(1){
                uint16_t val = *(const uint16_t*)str;
                uint16_t b2 = ((val - 0x0101) & ~val & 0x8080);

                if(b2 != 0){
                    if(b2 & 0x0080){
                        #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
                        str++;
                        #endif
                        break;
                    }
                    else{
                        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                        str++;
                        #endif
                        break;
                    }
                }
                str += 2;
            }
            break;
    }

    return str - original_str;
}