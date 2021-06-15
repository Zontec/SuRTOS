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

#include "serial.h"


void serial_init()
{
    drv_uart_init(0, 0);
}

void serial_write_byte(uint8_t data)
{
    drv_uart_write_byte(2, data);
}

uint8_t serial_read_byte()
{
    return drv_uart_read_byte(2);
}

void serial_write_string(const char *str)
{
    while (*str)
    {
        drv_uart_write_byte(0, *str);
        ++str;
    }
}

void serial_write_number(int32_t num)
{
    char str[16] = {0};
    uint32_t i = 0;
    uint8_t minus = false;

    if (num < 0)
    {
        minus = true;
        num *= -1;
    }
    if (num == 0)
    {
        str[i++] = '0';
    }
    while (num)
    {
        str[i++] = '0' + num % 10;
        num /= 10;
    }

    if(minus)
    {
        str[i++] = '-';
    }
    str[i] = '\0';
    strrev(str);
    
    serial_write_string(str);
}

int32_t serial_read_number()
{
    char str_num[16];
    char *str = str_num;
    char *tmp_str = str_num;

    int32_t minus = 1;
    int32_t num = 0;

    while ((*str = serial_read_byte()) && *str != '\n' && *str != '\r')
    {
        if (*str < '0' || *str > '9')
            break;
        
        if (*str == '-')
        {
            minus = -1;
        }
        else 
        {
            ++str;
        }

        *str = 0;
    }
    strrev(str);
    while (tmp_str != str)
    {
        num = num * 10 + (*tmp_str - '0');
        ++tmp_str;
    }

    return num;
}

int serial_read_string(char *str)
{
    uint32_t cnt = 0;
    while ((*str = serial_read_byte()) && *str != '\n' && *str != '\r')
    {
        ++str;
        ++cnt;
        *str = 0;
    }
    *str = 0;

    return cnt;
}

void serial_write(const uint8_t *data, uint32_t data_len)
{
    for (uint32_t i = 0; i < data_len; ++i)
    {
        serial_write_byte(data[i]);
    }
}

int serial_read(uint8_t *data, uint32_t data_len)
{
    for (uint32_t i = 0; i < data_len; ++i)
    {
        data = serial_read_byte();
    }
}

void serial_close()
{
    drv_uart_close(2);
}