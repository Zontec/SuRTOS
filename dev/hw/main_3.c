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

#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"


uint32_t __heap_base = RAM_BEGIN_ADRESS;
uint32_t __heap_size = 200;

#define ICSR (*(volatile uint32_t *) 0xE000ED04)

static volatile uint32_t *ret;

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

void timer_init()
{
    TIM_TimeBaseInitTypeDef TIMER_InitStructure;
 	TIM_TimeBaseStructInit(&TIMER_InitStructure);
    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIMER_InitStructure.TIM_Prescaler = 8399;
    TIMER_InitStructure.TIM_Period = 100000;
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

#define rd_r0(result) asm volatile ("MOV %0, r0 \n\t": "=r" (result))
#define rd_r1(result) asm volatile ("MOV %0, r1 \n\t": "=r" (result))
#define rd_r2(result) asm volatile ("MOV %0, r2 \n\t": "=r" (result))
#define rd_r3(result) asm volatile ("MOV %0, r3 \n\t": "=r" (result))
#define rd_r4(result) asm volatile ("MOV %0, r4 \n\t": "=r" (result))
#define rd_r5(result) asm volatile ("MOV %0, r5 \n\t": "=r" (result))

#define rd_sp(result) asm volatile ("MOV %0, r13 \n\t": "=r" (result))
#define rd_lr(result) asm volatile ("MOV %0, lr \n\t": "=r" (result))
#define rd_ip(result) asm volatile ("MOV %0, r15 \n\t": "=r" (result))
#define rd_msp(result) asm volatile ("MRS %0, msp \n\t": "=r" (result))
#define rd_psp(result) asm volatile ("MRS %0, psp \n\t": "=r" (result))
#define rd_psr(result) asm volatile ("MRS %0, psr \n\t": "=r" (result))

#define wr_sp(var) asm volatile ("MOV r13, %0\n\t"::"r" (var))
#define wr_lr(var) asm volatile ("MOV r14, #2\n\t"::"r" (var))
#define wr_ip(var) asm volatile ("MOV r15, %0\n\t"::"r" (var))

char stack1[512];
uint32_t *tmp_sp;

void led()
{
    rd_sp(tmp_sp);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
    *(tmp_sp + 1) = ret;
}

void led1()
{
    GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
}
 
void led2()
{
    GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
}
typedef volatile uint32_t* vu;

void _sched_run(void)
{
    led1();
}

void __attribute((naked)) _sched_exit(void)
{
    led2();
    __enable_irq();
    __asm__ volatile("add  sp,sp,#(8*4)");
    __asm__ volatile("bx lr");

}

static volatile uint32_t *tsp;
static volatile int c;
static volatile int d;
static volatile int *tlr;

typedef struct stack_frame_t
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
} stack_frame_t;
static stack_frame_t *sf;

void __attribute__((interrupt("IRQ"))) PendSV_Handler(void)
{
    ICSR |= 1<<27;
    led2();
    rd_msp(tsp);
    sf = tsp + 4;
    printf("R0: %p\n\r", sf->r0);
    printf("R1: %p\n\r", sf->r1);
    printf("R2: %p\n\r", sf->r2);
    printf("R3: %p\n\r", sf->r3);
    printf("R12: %p\n\r", sf->r12);

    printf("LR: %p\n\r", sf->lr);
    printf("PC: %p\n\r", sf->pc);
    printf("PSR: %p\n\r", sf->psr);
 
/*
    volatile int a;
    volatile int b;
    a++;
    __asm__ volatile ("TST   lr, 0x4");//       ; Is bit 2 of LR clear?
    __asm__ volatile ("ITE   eq");
    __asm__ volatile ("MRSEQ r3, MSP      ");//e
    __asm__ volatile ("MRSNE r3, PSP      ");//n use
    __asm__ volatile ("LDR   r2, [r3, #28]");//into r0
    __asm__ volatile ("LDR   r0, [r3, #24]");//into r0
    __asm__ volatile ("LDR   r1, [r3, #20]");//into r1
    __asm__ volatile ("MOV   %0, r1":"=r"(a));//into r1
    __asm__ volatile ("MOV   %0, r0":"=r"(b));//into r1
    __asm__ volatile ("MOV   %0, r2":"=r"(c));//into r1
    printf("lr: %p\n\r", a);
    printf("pc: %p\n\r", b);
    printf("psr: %p\n\r", c);
    printf("psr: %p\n\r", d);
*/

/*
    __asm__ volatile("ldr r3, =0xE000ED04\n\t");
    __asm__ volatile("dr r1, =1<<27\n\t");
    __asm__ volatile("str r1, [r3]\n\t");
    __asm__ volatile("ldr r3, =1<<24\n\t");
    __asm__ volatile("ldr r2, =_sched_run\n\t");
    __asm__ volatile("ldr r1, =_sched_exit\n\t");
    __asm__ volatile("sub sp, sp, #8*4\n\t");
    __asm__ volatile("add r0, sp, #5*4\n\t");
    __asm__ volatile("stm r0!, {r1-r3}\n\t");
    __asm__ volatile("ldr r0, =0xFFFFFFF9\n\t");
    __asm__ volatile("bx r0\n\t");
    */
}


void TIM7_IRQHandler(void)
{
    __disable_irq();
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        ICSR |= (1 << 28);
    }
    __enable_irq();

}
volatile uint32_t *sp1;

void __attribute__((used)) empty_call() { __asm__ volatile("nop\n");}

void func()
{
    volatile uint32_t a = 0xABABABAB;
    volatile uint32_t c = 0xCCCCFFFF;
    rd_sp(sp1);
    
    empty_call();
  
    ret = *(sp1 + 3);

    *(sp1 + 3) = led1;
}

#define MARKER  0x0FAFAFAF0
#define BRK     uint32_t mark3233 = MARKER
uint32_t *tmp;



int main(void)
{

    SystemInit();
    SystemCoreClockUpdate();
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    __disable_irq();

    initPeriph();
    pinsInit();
    timer_init();
    timer_int_init();


    __enable_irq();



    while (1)
    {
    __disable_irq();

        rd_psr(tmp);
        printf("__PSR: %p\n\r", *tmp);
    __enable_irq();
    Delay(10000000);

/*
        func();
        led2();
        Delay(10000000);
*/
    }


}
