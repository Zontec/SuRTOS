#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"


#include "types.h"
#include "memmap.h"
#include "platform_specific.h"
#include "stdio.h"
#include "panic.h"
#include "assert.h"
#include "board.h"

#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"

void Delay(uint32_t nCount)
{
    while (nCount--) 
    {
        __asm__ volatile("nop\n");
    }
}


void main(void)
{
    SystemInit();
    SystemCoreClockUpdate();

    board_init();

    while (1)
    {
        PRINTF("Hello world\n\r");
        Delay(3000000);
    }
}
