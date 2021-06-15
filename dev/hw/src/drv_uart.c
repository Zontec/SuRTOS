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

#include "drv_uart.h"

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

typedef struct _drv_uart_dev_t
{
    uint32_t pin_tx;
    uint32_t pin_rx;
    uint32_t pin_ctl;
    uint32_t port;
} _drv_uart_dev_t;

static _drv_uart_dev_t dev[] = 
{
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

void drv_uart_init(drv_uart_dev_e uart_dev, drv_uart_settings_t *uart_settings)
{
    USART_InitTypeDef uart;

    GPIO_InitTypeDef gpio;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
 
    USART_StructInit(&uart);
    USART_Init(USART2, &uart);

    USART2->BRR = 562 << 3;

    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Pin = GPIO_Pin_2;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpio);

    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Pin = GPIO_Pin_3;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpio);

    USART_Cmd(USART2, ENABLE);
}

void drv_uart_write_byte(drv_uart_dev_e uart_dev, uint8_t data)
{
    USART2->DR=data;

    while(!(USART2->SR & USART_SR_TC));
}

uint8_t drv_uart_read_byte(drv_uart_dev_e uart_dev)
{
    while ((!(USART2->SR & USART_SR_RXNE)));
    return USART2->DR;
}

uint8_t drv_uart_close(drv_uart_dev_e uart_dev)
{
    USART_Cmd(USART2, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);

}
