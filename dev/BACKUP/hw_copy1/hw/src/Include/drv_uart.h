#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#include "types.h"

typedef struct drv_uart_settings_t
{
    uint32_t baud;
    union
    {
        struct
        {
            uint8_t even : 1;
            uint8_t data_9 : 1;
            uint8_t stop : 2;
            uint8_t flow : 1;
            uint8_t reserved : 3;
        };
        uint8_t bt;
    };
} drv_uart_settings_t;

void drv_uart_init(uint8_t uart_dev, drv_uart_settings_t uart_settings);

void drv_uart_write_byte(uint8_t data);

uint8_t drv_uart_read_byte();

void drv_uart_write(const uint8_t *data, uint32_t len);

void drv_uart_read(uint8_t *data, uint32_t len);

#endif /*__DRV_UART_H__*/