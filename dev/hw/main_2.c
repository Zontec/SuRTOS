#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"


#include "types.h"
#include "memmap.h"
#include "platform_specific.h"
#include "stdio.h"

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

char tests[][20] =
{
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "11",
    "12",
    "13",
    "14",
    "15",
    "16",
    "17",
};


static inline void * rd_stack_ptr(void)
{
  void *result = NULL;
  asm volatile ("MRS %0, msp\n\t"
      : "=r" (result) );
  return result;
}

static inline void * rd_thread_stack_ptr(void){
    void *result = NULL;
    asm volatile ("MRS %0, psp\n\t" : "=r" (result) );
    return(result);
}

static inline void wr_stack_ptr(void * ptr){
    asm volatile ("MSR msp, %0\n\t" : : "r" (ptr) );
}

static inline void wr_thread_stack_ptr(void * ptr){
    asm volatile ("MSR psp, %0\n\t" : : "r" (ptr) );
}
#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"
void timer_init()
{
    TIM_TimeBaseInitTypeDef TIMER_InitStructure;
 	TIM_TimeBaseStructInit(&TIMER_InitStructure);
    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIMER_InitStructure.TIM_Prescaler = 8399;
    TIMER_InitStructure.TIM_Period = 10000;
    TIM_TimeBaseInit(TIM7, &TIMER_InitStructure);
    TIM_ITConfig(TIM7, TIM_IT_Update|TIM_IT_Trigger|TIM_IT_COM, ENABLE);
    TIM_Cmd(TIM7, ENABLE);
}

void timer_int_init()
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

typedef void (*p)(void);
#define ICSR (*(volatile uint32_t *) 0xE000ED04)
void PendSV_Handler(void)
{
    led2();
}
void TIM7_IRQHandler(void)
{
   
        if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
        {
              ICSR |= (1 << 28);

            __asm__("nop");
            __asm__("nop");
            __asm__("nop");
            __asm__("nop");
        	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        }
}
typedef struct {
  // we explicity push these registers
  struct {
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
  } sw_frame;

  // these registers are pushed by the hardware
  struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    void *lr;
    void *pc;
    uint32_t psr;
  } hw_frame;
} cmcm_stack_frame_t;
static char stack1[512];

static inline void * rd_reg(void){
  void * result=NULL;
  asm volatile ("MRS %0, psp\n\t"
      //"MOV r0, %0 \n\t"
      : "=r" (result) );
  return result;
}

void led1()
{
    GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
}
static const void *st[] = {led1, led1, led1, led1};

void led2()
{
    uint32_t mem = 0;
    GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
    printf("%p\n\r", rd_reg());

    wr_thread_stack_ptr(st);
    printf("ST: %p\n\r", st);
    printf("SP: %p\n\r", ((uint32_t*)rd_reg()));
    printf("SP-1: %p\n\r", *((uint32_t*)rd_reg()-1));
    printf("SP-2: %p\n\r", *((uint32_t*)rd_reg()-2));
}

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    __enable_irq();

    initPeriph();
    pinsInit();
    timer_init();
    timer_int_init();

while (1)
{
    /* code */
}


}
