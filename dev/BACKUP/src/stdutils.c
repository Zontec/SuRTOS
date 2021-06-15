#include "stdutils.h"


char to_lower(char x)
{
    if (x >= 'A' && x<= 'Z')
    {
        return x + 32;
    }
    return x;
}

char to_upper(char x)
{
    if (x >= 'a' && x<= 'z')
    {
        return x - 32;
    }
    return x;
}

char *lower_string(char *str)
{
    char *p = str;
    while (*str)
    {
        to_lower(*str);
        str++;
    }
    return p;
}

char *upper_string(char *str)
{
    char *p = str;
    while (*str)
    {
        to_upper(*str);
        str++;
    }
    return p;
}

void mem_xor(void *dst, const void *src, uint32_t len)
{
    for (uint32_t i = 0; i < len; ++i)
    {
        *UPTR8(dst + i) ^= *UPTR8(src + i);
    }
}

void mem_swap(void *buf1, void *buf2, uint32_t len)
{
    mem_xor(buf1, buf2, len);
    mem_xor(buf2, buf1, len);
    mem_xor(buf1, buf2, len);
}

void swapU8(uint8_t buf1, uint8_t buf2)
{
    buf1 ^= buf2;
    buf2 ^= buf1;
    buf1 ^= buf2;
}

void swapU16(uint16_t buf1, uint16_t buf2)
{
    buf1 ^= buf2;
    buf2 ^= buf1;
    buf1 ^= buf2;
}

void swapU32(uint32_t buf1, uint32_t buf2)
{
    buf1 ^= buf2;
    buf2 ^= buf1;
    buf1 ^= buf2;
}

void swapU64(uint64_t buf1, uint64_t buf2)
{
    buf1 ^= buf2;
    buf2 ^= buf1;
    buf1 ^= buf2;
}