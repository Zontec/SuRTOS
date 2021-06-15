#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"


#include "types.h"
#include "memmap.h"
#include "platform_specific.h"
#include "stdio.h"
#include "panic.h"
#include "assert.h"
#include "sha1.h"

uint32_t __heap_base = RAM_BEGIN_ADRESS;
uint32_t __heap_size = 200;

void initPeriph()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(FLASH_R_BASE, ENABLE);
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
}

void Delay(__IO uint32_t nCount)
{
    while (nCount--) {
        __ASM( "nop" );
    }
}

static const uint8_t lede[] = 
{
    0x80, 0xb4, 0x00, 0xaf,   
    0x05, 0x4b, 0x1b, 0x68,     
    0x04, 0x4a, 0x43, 0xf4,    
    0x00, 0x53, 0x13, 0x60,   //80 -> 00
    0x00, 0xbf, 0xbd, 0x46,      
    0x5d, 0xf8, 0x04, 0x7b,    
    0x70, 0x47, 0x00, 0xbf,      
    0x18, 0x0c, 0x02, 0x40,   
};

void led_on()
{
    *(volatile uint32_t*)((0x40020C18)) |= (1 << 12);
}

void led_on1()
{
    *(volatile uint32_t*)((0x40020C18)) |= (1 << 13);
}
/*
     static uint8_t buf[400];

    PUT_FUNC_IN_RAM(led_on1, 400, HEAP_BASE);

    CALL(HEAP_BASE, p)();

    memcpy(buf, GET_FUNC_THUMB_ADDRESS(led_on), 128);
    flash_write(buf, 128);

    CALL(FLASH_SECTOR4_BASE, p)();

*/
typedef void (*p)(void);

int main(void)
{
    initPeriph();
    pinsInit();

    
    CALL(lede, p)();
    
    while (1)
    {

    }
}
