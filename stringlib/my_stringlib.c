#include <stddef.h>

size_t my_strlen(const char *str){
    size_t total = 0;
    while(*str++){
        total++;
    }
    return total;
}

int my_strcmp(const char *str1, const char *str2){
    while(*str1 && (*str1==*str2)){
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

void *my_memcpy(void *dest, const void *src, size_t n){
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (unsigned char*)src;
    for(size_t i = 0; i < n; i++){
        d[i] = s[i];
    }
    return dest;
}