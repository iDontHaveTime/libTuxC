#include "stdlib.h"
#include "stdbool.h"

char* itoa(int num, char* buffer, int base){
    return lltoa(num, buffer, base);
}

char* ltoa(long num, char* buffer, int base){
    return lltoa(num, buffer, base);
}

char* lltoa(long long num, char* buffer, int base){
    if(base < 2 || base > 36){
        buffer[0] = '\0';
        return buffer;
    }

    bool negative = false;
    unsigned long long n;

    if(num == 0){
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    if(base == 10 && num < 0){
        negative = true;
        n = (unsigned long long)(-num);
    } 
    else{
        n = (unsigned long long)num;
    }

    long long i = 0;
    while(n > 0){
        long long rem = n % base;
        buffer[i++] = (rem < 10) ? ('0' + rem) : ('A' + rem - 10);
        n /= base;
    }

    if(negative){
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    for(size_t j = 0, k = i - 1; j < k; j++, k--){
        char temp = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = temp;
    }

    return buffer;
}

char* utoa(unsigned int num, char* buffer, int base){
    return llutoa(num, buffer, base);
}

char* lutoa(unsigned long num, char* buffer, int base){
    return llutoa(num, buffer, base);
}

char* llutoa(unsigned long long num, char* buffer, int base){
    if(base < 2 || base > 36){
        buffer[0] = '\0';
        return buffer;
    }

    if(num == 0){
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    unsigned long long i = 0;
    while(num > 0){
        unsigned long long rem = num % base;
        buffer[i++] = (rem < 10) ? ('0' + rem) : ('A' + rem - 10);
        num /= base;
    }

    buffer[i] = '\0';

    for(size_t j = 0, k = i - 1; j < k; j++, k--){
        char temp = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = temp;
    }

    return buffer;
}
