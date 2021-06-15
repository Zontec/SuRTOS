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
#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void serial_init()
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

void serial_write_byte(uint8_t data)
{
    USART2->DR=data;

    while(!(USART2->SR & USART_SR_TC));
}

uint8_t serial_read_byte()
{
    while ((!(USART2->SR & USART_SR_RXNE)));
    return USART2->DR;
}

void serial_write_string(const char *str)
{
    while (*str)
    {
        serial_write_byte(*str);
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
        USART_Cmd(USART2, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);

}