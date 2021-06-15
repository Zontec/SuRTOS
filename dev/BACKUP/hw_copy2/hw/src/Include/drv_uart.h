#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#include "types.h"
#include "memmap.h"
#include "macros.h"

typedef enum
{
    DRV_UART_PARITY_EVEN =  0,
    DRV_UART_PARITY_ODD =   1
};

typedef enum
{
    DRV_UART_DATA_8 = 0,
    DRV_UART_DATA_9 = 1
};

typedef enum
{
    DRV_UART_STOP_1 =   0,
    DRV_UART_STOP_1_5 = 1,
    DRV_UART_STOP_2 =   2
};

typedef enum
{
    DRV_UART_FLOW_CONTROL_NO =  0,
    DRV_UART_FLOW_CONTROL_CTS = 1,
    DRV_UART_FLOW_CONTROL_RST = 2,
};

typedef enum drv_uart_dev_e 
{
    DRV_UART_1 = 0,
    DRV_UART_2 = 1,
    DRV_UART_3 = 2,
    DRV_UART_4 = 3,
    DRV_UART_5 = 4,
    DRV_UART_6 = 5,
} drv_uart_dev_e;

typedef struct drv_uart_settings_t
{
    uint32_t baud;
    union
    {
        struct
        {
            uint8_t parity : 1;
            uint8_t data_9 : 1;
            uint8_t stop : 2;
            uint8_t flow : 2;
            uint8_t reserved : 2;
        };
        uint8_t bt;
    };
} drv_uart_settings_t;

void drv_uart_init(drv_uart_dev_e uart_dev, drv_uart_settings_t *uart_settings);

void drv_uart_write_byte(drv_uart_dev_e uart_dev, uint8_t data);

uint8_t drv_uart_read_byte(drv_uart_dev_e uart_dev);

void drv_uart_write(drv_uart_dev_e uart_dev, const uint8_t *data, uint32_t len);

void drv_uart_read(drv_uart_dev_e uart_dev, uint8_t *data, uint32_t len);

#endif /*__DRV_UART_H__*/