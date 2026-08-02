#ifndef MY_STRINGLIB_H
#define MY_STRINGLIB_H
#include <stddef.h>

size_t my_strlen(const char *str);
int my_strcmp(const char *str1, const char *str2);
void *my_memcpy(void *dest, const void *src, size_t n);

#endif