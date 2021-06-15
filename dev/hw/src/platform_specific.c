/****************************INFORMATION***********************************
*
*
*
*
**************************************************************************/

/*!
*   @file
*   @brief SHA algorithm header file
*	@author Zontec
*	@version 1.0
*	@date 2021.04.04
*/

#include "platform_specific.h"

void *memset(void *memptr, int32_t val, uint32_t len)
{
    void *base_ptr = memptr;

    for (uint32_t i = 0; i < len; ++i, ++memptr)
    {
        *UPTR8(memptr) = U8(val & 0xFF);
    }
    return base_ptr;
}

void *memcpy(void *dst, const void *src, uint32_t len)
{
    void *base_ptr = dst;

    for (uint32_t i = 0; i < len; ++i, ++dst, ++src)
    {
        *UPTR8(dst) = *UPTR8(src);
    }

    return base_ptr;
}

int32_t memcmp(const void *buf1, const void *buf2, uint32_t len)
{
    for (uint32_t i = 0; i < len; ++i, ++buf1, ++buf2)
    {
        int8_t left = *PTR8(buf1);
        int8_t right = *PTR8(buf2);

        if (left - right)
        {
            return (left > right ? 1 : -1);
        }
    }
    return 0;
}

void *malloc(uint32_t len)
{
    return mem_manager_alloc(len);
}

void *realloc(const void* ptr, uint32_t len)
{
    return mem_manager_realloc(ptr, len);
}

void *calloc(uint32_t len, uint32_t type_size)
{
    uint32_t full_size = len * type_size;
    void *ptr = mem_manager_alloc(full_size);

    memset(ptr, 0x00, full_size);

    return ptr;
}

void free(void *ptr)
{
    mem_manager_free(ptr);
}

uint32_t strlen(const char *str)
{
    uint32_t cnt = 0;

    while (*str++)
    {
        ++cnt;
    }

    return cnt;
}

char *strrev(char *str)
{
    char *l;
    char *r;

    for (l = str, r = str + strlen(str) - 1; l < r; ++l, --r)
    {  
        swapU8(l, r);
    }

    return str;
}

char *itoa(int32_t num, char *str, int radix)
{
    static const char radix_arr[] = "0123456789ABCDEFG";
    char *base_str = str;
    char *num_str = str;


    if (num == 0)
    {
        str[0] = '0';
    }

    if (num < 0)
    {
        if (radix == 10)
        {
            *str = '-';
            ++num_str;
            ++str;
        }
        num *= -1;
    }

    while (num)
    {
        *str = radix_arr[num % radix];
        num /= radix;
        str++;
    }

    str = num_str;
    strrev(str);

    return base_str;
}

int32_t atoi(const char *str)
{
    int32_t minus = 1;
    int32_t num = 0;

    if (*str == '-')
    {
        minus = -1;
        ++str;
    }

    while (*str && *str >= '0' && *str <= '9')
    {
        num = num * 10 + ((*str) - '0');
        ++str;
    }

    return num;
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

char *strtok(char *str, const char *delim)
{
    char *ret = NULL;
    char *p;
    const char *d;
    static char* _buffer = NULL;

    if (str == NULL && _buffer == NULL)
    {
        goto _strtok_exit;
    }

    if (delim == NULL || (_buffer != NULL && _buffer[0] == '\0'))
    {
        goto _strtok_exit;
    }

    if (str != NULL) 
    {
        _buffer = str;
    }

    ret = _buffer;

 
    for (p = _buffer; *p != '\0'; ++p) 
    {
        for (d = delim; *d != '\0'; ++d) 
        {
            if (*p == *d) 
            {
                *p = '\0';
                _buffer = p + 1;
 
                if (p == ret) 
                { 
                    ret++; 
                    continue; 
                }
                goto _strtok_exit;
            }
        }
    }

_strtok_exit:
    return ret;
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
    char *base_ptr = dst;

    while (*src)
    {
        dst[shift++] = *src++;
    }

    return base_ptr;
}