#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"


#include "types.h"
#include "memmap.h"
#include "stdio.h"
#include "panic.h"
#include "assert.h"

uint32_t __heap_base = RAM_BEGIN_ADRESS;
uint32_t __heap_size = 200;

void initPeriph()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
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
    


    //print("Binary int: %X\r\n", 153);
   //print("Binary int: %x\r\n", 153);
   // print("Binary int: %o\r\n", 153);
    //print("Binary int: %O\r\n", 153);
    //print("Binary int: %p\r\n", 153);
    //print("Binary int: %P\r\n", 153);

    /*
    RCC_ClocksTypeDef def;
    RCC_GetClocksFreq(&def);

    uint32_t sysclk = def.SYSCLK_Frequency;
    uint32_t hclk = def.HCLK_Frequency;
    uint32_t pclk = def.PCLK1_Frequency;
    uint32_t pclk2 = def.PCLK2_Frequency;
*/
    initPeriph();
    pinsInit();
    
    uint32_t data[5];
    data[0] = *VUPTR32(FLASH_BEGIN_ADRESS);
    data[1] = *(VUPTR32(FLASH_BEGIN_ADRESS) + 1);
    data[2] = *(VUPTR32(FLASH_BEGIN_ADRESS) + 2);
    data[3] = *(VUPTR32(FLASH_BEGIN_ADRESS) + 3);
    *(VUPTR32(FLASH_BEGIN_ADRESS) + 4) = 0x1A1B1C1D;
    data[4] = *(VUPTR32(FLASH_BEGIN_ADRESS) + 4);

    printf("1%p ", data[0]);
    printf("%p ", data[1]);
    printf("%p ", data[2]);
    printf("%p ", data[3]);
    printf("%p ", data[4]);

    printf("\n\r");
    while (1)
    {

        
        GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
        Delay(20000000);

        GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
        Delay(20000000);
    }
}
