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


int main(void)
{
    initPeriph();
    char str[50];
    int a;
    printf("\n\r");
    printf("\n\r");
    while (1)
    {
        printf("cmd/>");
        scanf("%s", str);
        /*
        printf("cmd accepted: %s\n\r", str);
        printf("%d\n\r", str[strlen(str)-1]);
        */
        cli_handler(str);
        if (!strcmp(str, "led_on 1"))
        {
            GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
        }
        if (!strcmp(str, "led_on 2"))
        {
            GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
        }
        if (!strcmp(str, "led_on 3"))
        {
            GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
        }
        if (!strcmp(str, "led_on 4"))
        {
            GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
        }
        memset(str, 0x00, 50);

       /* 
        GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
        Delay(20000000);

        GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
        Delay(20000000);
        */
    }
}
