#include "stdio.h"

#include <stdarg.h>

static const char base_rep[] = {"0123456789abcdef"};

int printf(const char* format, ...) 
{ 
    char *traverse;
    char *s; 
    uint32_t i; 
    uint32_t cnt = 0;

    va_list arg; 
    va_start(arg, format); 

    if (!format)
    {
        return -1;
    }

    SERIAL_ENABLE();

    for (traverse = format; *traverse != '\0'; traverse++) 
    { 
        while (*traverse && *traverse != '%') 
        { 
            PUTC(*traverse);
            traverse++; 
        } 
        
        if (!*traverse)
        {
            break;
        }

        traverse++; 

        switch (*traverse) 
        { 
            case 'c': 
                i = va_arg(arg, char);
                PUTC(i);
                ++cnt;
                break; 
            case 'd': 
                i = va_arg(arg, int32_t);
                PUTN(i);
                ++cnt;
                break; 
            case 'b': 
                i = va_arg(arg, uint32_t);
                for (int32_t j = 31; j >= 0; --j)
                {
                    PUTC(GET_BIT_U32(i, j) + '0');
                }
                ++cnt;
                break;
            case 's': 
                s = va_arg(arg, char *);
                PUTS(s); 
                ++cnt;
                break; 
            case 'x':
                i = va_arg(arg, uint32_t);
                PUTC(base_rep[i >> 4]);
                PUTC(base_rep[i & 0x0F]);
                ++cnt;
                break; 
            case 'X':
                i = va_arg(arg, uint32_t);
                PUTC(to_upper(base_rep[i >> 4]));
                PUTC(to_upper(base_rep[i & 0x0F]));
                ++cnt;
                break;
            case 'p':
                i = va_arg(arg, uint32_t);
                for(int32_t j = 0; j < 4; j++)
                {
                    uint8_t x = ((uint8_t)(i >> (8 * (3 - j))) & 0xFF); 
                    PUTC(to_upper(base_rep[x >> 4]));
                    PUTC(to_upper(base_rep[x & 0x0F]));
                }
                ++cnt;
                break;
        }   
    }
    va_end(arg); 
    SERIAL_DISABLE();

    return cnt;
}

int scanf(const char* format, ...)
{

}
