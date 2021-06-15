#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

#include "thread.h"
#include "board.h"

#define STACK_SIZE  (4096)

char stack1[STACK_SIZE] __attribute__ ((aligned (8)));
char stack2[STACK_SIZE] __attribute__ ((aligned (8)));
char stack3[STACK_SIZE] __attribute__ ((aligned (8)));
char stack4[STACK_SIZE] __attribute__ ((aligned (8)));


void initPeriph()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
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
void Delay(int32_t nCount)
{
    while (nCount--) 
    {
        __asm__ volatile("nop\n");
    }
}

#define DELAY_TIME  600000

void led1()
{
    while (1)
    {
        __disable_irq();
        led_toggle(LED1);
        __enable_irq();
        Delay(DELAY_TIME);
    }
}
void led2()
{
    while (1)
    {
        __disable_irq();
        led_toggle(LED2);
        __enable_irq();
        Delay(DELAY_TIME);
    }
}
void led3()
{
    while (1)
    {
        __disable_irq();
        GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
        __enable_irq();
        Delay(DELAY_TIME);
    }
}
void led4()
{
    while (1)
    {
        GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
        Delay(DELAY_TIME);
    }
}

int main(void)
{

    __disable_irq();
    board_init();
    initPeriph();
    pinsInit();
    /*bootloader block*/

    thread_create(stack1, 4096, 0, 0, led1, 0, "lal");
    thread_create(stack2, 4096, 0, 0, led2, 0, "lal");
    
    thread_create(stack3, 4096, 0, 0, led3, 0, "lal");
    thread_create(stack4, 4096, 0, 0, led4, 0, "lal");

    __enable_irq();        
    return 0;
}
