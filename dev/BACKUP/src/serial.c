#include "serial.h"

void serial_init()
{
    drv_uart_init(0, 0);
}

void serial_write_byte(uint8_t data)
{
    drv_uart_write_byte(2, data);
}

uint8_t serial_read_byte();

void serial_write_string(const char *str)
{
    while (*str)
    {
        drv_uart_write_byte(0, *str);
        str++;
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
    for (int j = 0; j < (i >> 1); j++)
    {
        char tmp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = tmp;
    }
    serial_write_string(str);
}

int serial_read_string(char *str, uint32_t str_len);

void serial_write(const uint8_t *data, uint32_t data_len);

int serial_read(uint8_t *data, uint32_t data_len);

void serial_close()
{
    return;
}