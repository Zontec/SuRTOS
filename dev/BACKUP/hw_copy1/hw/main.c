#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include <memory.h>

#include "memmap.h"
#include "macros.h"

#include<stdarg.h>  
uint32_t __heap_base = RAM_BEGIN_ADRESS;
uint32_t __heap_size = 200;
USART_InitTypeDef uart;
USART_ClockInitTypeDef uart_clk;

void initPeriph()
{
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //Включаем тактирование порта A
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //Включаем тактирование порта USART2
 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //Подключаем PA3 к TX USART2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //Подключаем PA2 к RX USART2
 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
}

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

void lower_string(char *str)
{
    while (*str)
    {
        to_lower(*str);
        str++;
    }
}

void upper_string(char *str)
{
    while (*str)
    {
        to_upper(*str);
        str++;
    }
}

void pinsInit() {



    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOD, &gpio);

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
    
    
}
void send_to_uart(uint8_t data)
{
 while(!(USART2->SR & USART_SR_TC));
 USART2->DR=data;
}
void Delay(__IO uint32_t nCount)
{
    while (nCount--) {
        __ASM( "nop" );
    }
}
char *convert(unsigned int num, int base) 
{ 
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[32]; 
    char *ptr; 

    ptr = &buffer[31]; 
    *ptr = '\0'; 

    do 
    { 
        *--ptr = Representation[num % base]; 
        num /= base; 
    }while(num != 0); 

    return(ptr); 
}
void initUART()
{
    USART_StructInit(&uart);
    USART_ClockStructInit(&uart_clk);
    USART_Init(USART2, &uart);
    //USART_ClockInit(USART2, &uart_clk);
    USART2->BRR = 562 << 3;

    USART_Cmd(USART2, ENABLE);
}

void uart_write_byte(char byte)
{
        USART2->DR = byte;
        while(!(USART2->SR & USART_SR_TXE));//What until finished
}

void print_str(const char *str)
{
    while (*str)
    {
        uart_write_byte(*str);
        str++;
    }
    
}

void print_num(int32_t num)
{
    char str[10] = {0};
    int i = 0;
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
    for (int j = 0; j < i / 2; j++)
    {
        char tmp = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = tmp;
    }
    print_str(str);
}

#define PUTC(x) putc(x)
#define PUTS(x) print_str(x)
#define PUTN(x) print_num(x)

int putc(char symb)
{
    uart_write_byte(symb);
}

int printf(const char* format, ...) 
{ 

    char *traverse;
    char *s; 
    unsigned int i; 

    va_list arg; 
    va_start(arg, format); 

    for (traverse = format; *traverse != '\0'; traverse++) 
    { 
        while (*traverse && *traverse != '%') 
        { 
            PUTC(*traverse);
            traverse++; 
        } 
        if (!traverse)
        {
            break;
        }

        traverse++; 

        switch (*traverse) 
        { 
            case 'c': 
                i = va_arg(arg, char);
                PUTC(i);
                break; 

            case 'd': 
                i = va_arg(arg, int32_t);
                PUTN(i);
                break; 

            case 'o': 
                i = va_arg(arg, uint32_t);
                PUTC(convert(i, 8));
                break; 

            case 's': 
                s = va_arg(arg, char *);
                PUTS(s); 
                break; 

            case 'x':
                i = va_arg(arg, uint32_t);
                PUTC(convert(i, 16));
                break; 
        }   
    }
    va_end(arg); 
} 



int main(void)
{
    /*
    uint32_t status = 1;
    RCC_HSEConfig(RCC_HSE_ON);
    RCC_WaitForHSEStartUp();
    RCC->CFGR &= ~((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4));
    RCC->CFGR &= ~((1 << 10) | (1 << 11) | (1 << 12));
*/
    RCC_ClocksTypeDef def;
    RCC_GetClocksFreq(&def);

    uint32_t sysclk = def.SYSCLK_Frequency;
    uint32_t hclk = def.HCLK_Frequency;
    uint32_t pclk = def.PCLK1_Frequency;
    uint32_t pclk2 = def.PCLK2_Frequency;

    initPeriph();
    pinsInit();
    initUART();
    

    while (1)
    {

        printf("Hello: -1\r\n\0");


        GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
        Delay(20000000);

        GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
        Delay(20000000);
    }
}
