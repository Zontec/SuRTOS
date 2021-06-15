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


uint32_t __heap_base = RAM_BEGIN_ADRESS;
uint32_t __heap_size = 200;

#define ICSR (*(volatile uint32_t *) 0xE000ED04)

static char stack1[512];
static char stack2[512];

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

typedef struct task_t
{
    char *sp;
} task_t;

task_t tasks[5];
uint32_t num_tasks = 0;

uint32_t cur_task = 0;
uint32_t nxt_task = 0;

static volatile stack_frame_t cur_sf;
static volatile stack_frame_t *tmp_sf;
static volatile uint32_t *tsp;

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

void led1()
{
    GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
}
 
void led2()
{
    GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
}

void led3()
{
    GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
}

void print_stack(volatile uint32_t *_sp)
{
    printf("R0:  %p\n\r",  *(_sp + 0));
    printf("R1:  %p\n\r",  *(_sp + 1));
    printf("R2:  %p\n\r",  *(_sp + 2));
    printf("R3:  %p\n\r",  *(_sp + 3));
    printf("R12: %p\n\r",  *(_sp + 4));
    printf("LR:  %p\n\r",  *(_sp + 5));
    printf("PC:  %p\n\r",  *(_sp + 6));
    printf("PSR: %p\n\r",  *(_sp + 7));
}

void __attribute__((naked)) _sched_run(void)
{
    led1();
    
    rd_sp(tsp);
    __enable_irq();

    __asm__ volatile("bx lr \n");
}

void _sched_exit()
{
    
}

void __attribute__((naked)) PendSV_Handler(void) 
{
    __asm__ volatile("push {r0,r1,r2,r3,r7,lr}\n");
    __asm__ volatile("mov r7, sp\n");
    
    ICSR |= 1 << 24;
    ICSR |= 1 << 27;

    nxt_task = cur_task + 1;
    nxt_task %= num_tasks;


    rd_sp(tsp);
    tmp_sf = (volatile stack_frame_t *)(tsp + 6);
    print_stack(tmp_sf);
    __asm__ volatile("mov sp, r7\n");
    __asm__ volatile("pop {r0,r1,r2,r3,r7,lr}");
    cur_task = nxt_task;
    __asm__ volatile("mov sp, %0\n"::"r"(tasks[cur_task].sp));
    __asm__ volatile("bx lr\n");
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

void inf()
{
    while (1)
    {
        led3();
        Delay(1000000);
    }
}

void thread_init(char *sp, void f(void))
{
    stack_frame_t tsf;
    task_t *task;

    task = &tasks[num_tasks];
    num_tasks++;
    
    task->sp = sp;

    memset(&tsf, 0, sizeof(stack_frame_t));
    memset(task->sp, 0, 512);

    tsf.pc = f;
    tsf.psr = 0x21000000;
    tsf.lr = inf;
    task->sp = task->sp + (512 - sizeof(stack_frame_t) * 2);
    memcpy(task->sp, &tsf, sizeof(stack_frame_t));
}

int main(void)
{
    thread_init(stack1, led1);
    thread_init(stack2, led2);

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
        /* code */
    }
    
}
