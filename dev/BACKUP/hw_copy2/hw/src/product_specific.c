#include "product_specific.h"


void *memset(void *memptr, int32_t val, uint32_t len)
{
    void *base_ptr = memptr;

    for (uint32_t i = 0; i < len; ++i)
    {
        *((uint8_t*)memptr++) = (uint8_t)(val & 0xFF);
    }
    return base_ptr;
}

void *memcpy(void *dest, const void *source, uint32_t len)
{
    void *base_ptr = dest;

    for (uint32_t i = 0; i < len; ++i)
    {
        *((uint8_t*)dest++) = ((uint8_t*)(source))[i];
    }
    return base_ptr;
}

int32_t memcmp(const void *buf1, const void *buf2, uint32_t len)
{
    for (uint32_t i = 0; i < len; ++i)
    {
        int8_t left = *(int8_t*)buf1;
        int8_t right = *(int8_t*)buf2;
        if (left - right)
        {
            return (left > right ? 1 : -1);
        }
    }
    return 0;
}
/*
void *malloc(uint32_t len)
{
    return malloc(len);
}

void *realloc(const void* ptr, uint32_t len)
{
    return realloc(ptr, len);
}

void *calloc(uint32_t len, uint32_t type_size)
{
    return calloc(len, type_size);
}

void free(void *ptr)
{
    free(ptr);
}
*/
uint32_t strlen(const char *str)
{
    uint32_t cnt = 0;

    while (*str++)
    {
        ++cnt;
    }
    return cnt;
}

int32_t strcmp(const char *str1, const char *str2)
{
    if (strlen(str1) != strlen(str2))
    {
        return (int32_t)strlen(str1) - (int32_t)strlen(str2);
    }
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
        {
            return str1 - str2;
        }
        str1++;
        str2++;
    }
    return 0;
}

char *strcpy(char *dst, const char *src)
{
    char base_ptr = dst;
    while (*src)
    {
        *dst++ = *src++;
    }
    return base_ptr;
}

char *strcat(char *dst, const char *src)
{
    uint32_t shift = strlen(dst);
    char base_ptr = dst;

    while (*src)
    {
        dst[shift++] = *src++;
    }
    return base_ptr;
}