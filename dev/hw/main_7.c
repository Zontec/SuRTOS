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
//#include "thread.h"

#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"

static uint32_t __heap_base = RAM_BEGIN_ADRESS;
static uint32_t __heap_size = 200;

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
#define wr_lr(var) asm volatile ("MOV r14, %0\n\t"::"r" (var))
#define wr_pc(var) asm volatile ("MOV r15, %0\n\t"::"r" (var))


#define ICSR (*(volatile uint32_t *) 0xE000ED04)
#define USE_PSP_IN_THREAD_MODE      (1 << 1)

#define STACK_SIZE  (4096)

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];
char stack3[STACK_SIZE];
char stack4[STACK_SIZE];

typedef struct stack_frame_t
{
    uint32_t o[8];
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

static task_t tasks[5]; 
static int is_os_started = 0;

static uint32_t INT_EXIT = 0xFFFFFFFC;
static uint32_t num_tasks = 0;

static uint32_t cur_task_ind = 0;
static uint32_t nxt_task_ind = 0;

static task_t *cur_task;
static task_t *nxt_task;

void initPeriph()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
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
void inf()
{
    while (1)
    {
        GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
    }
}
void sys_set_control(uint32_t val)
{
    asm volatile ("msr control, %0\n\t" : : "r" (val));
}   


#define DELAY_TIME  5000
void led1()
{
    while (1)
    {
        __disable_irq();
        GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
        __enable_irq();
    }
}
void led2()
{
    while (1)
    {
        __disable_irq();
        GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
        __enable_irq();
    }
}
void led3()
{
    while (1)
    {
        __disable_irq();
        GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
        __enable_irq();
        

    }
}
void led4()
{
    while (1)
    {
        __disable_irq();
        GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
        __enable_irq();
    }
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

void __attribute__((naked)) PendSV_Handler() 
{
    __disable_irq();

    ICSR |= 1 << 27;

    __asm__ volatile("push {r0-r7, lr}\n");
    
    nxt_task_ind = (cur_task_ind + 1) % num_tasks;
    cur_task = &tasks[cur_task_ind];
    nxt_task = &tasks[nxt_task_ind];
    cur_task_ind = nxt_task_ind;

    __asm__ volatile
    (
        " LDR     R3,=cur_task     \n"   // R3 = cur_task
        " LDR     R1,[R3]          \n"   // R1 = *cur_task === cur_task->sp
        " LDR     R2,=nxt_task     \n"   // R2 = nxt_task
        " LDR     R2,[R2]          \n"   // R2 = *nxt_task === nxt_task->sp
        " MRS     R0, PSP          \n"  
    );
    if (is_os_started)
    {
        __asm__ volatile
        (
        " SUB     R0,R0,#32        \n"
        " STR     R0,[R1]          \n"    // *R1 = R0 === cur_task->sp
        " STMIA   R0!,{R4-R7}      \n"    // Save old context (R4-R7)
        " MOV     R4,R8            \n"
        " MOV     R5,R9            \n"
        " MOV     R6,R10           \n"
        " MOV     R7,R11           \n"
        " STMIA   R0!,{R4-R7}      \n"    // Save old context (R8-R11)
        );
    }
    __asm__ volatile
    (
    " popStk:                  \n" 
    " LDR     R0, [R2]         \n"    // Get SP of task that be switch into.
    " ADD     R0,R0,#16        \n"
    " LDMIA   R0!,{R4-R7}      \n"    // Restore new Context (R8-R11)
    " MOV     R8,R4            \n"
    " MOV     R9,R5            \n"
    " MOV     R10,R6           \n"
    " MOV     R11,R7           \n"
    " SUB     R0,R0,#32        \n"
    " LDMIA   R0!,{R4-R7}      \n"    // Restore new Context (R4-R7)
    " ADD     R0,R0,#16        \n"
    " MSR     PSP, R0          \n"    // Mov new stack point to PSP
    );

    is_os_started = 1;

    __asm__ volatile("pop {r0-r7, lr}\n");
    __enable_irq();
    __asm__ volatile("bx lr\n");
}


void thread_init(char *sp, void f(void))
{
    stack_frame_t tsf;
    task_t *task;

    task = &tasks[num_tasks];
    num_tasks++;
    
    task->sp = sp;

    memset(&tsf, 0, sizeof(stack_frame_t));
    memset(task->sp, 0, STACK_SIZE);

    tsf.pc = ((uint32_t)f) & 0xFFFFFFFE;
    tsf.psr = 0x21000000;
    tsf.lr = inf;
    task->sp = task->sp + (STACK_SIZE - sizeof(stack_frame_t) * 8);
    memcpy(task->sp, &tsf, sizeof(stack_frame_t));
}


int main(void)
{
    thread_init(stack1, led1);
    thread_init(stack2, led2);
    thread_init(stack3, led3);
    thread_init(stack4, led4);
    sys_set_control(USE_PSP_IN_THREAD_MODE);

    __disable_irq();

    SystemInit();
    SystemCoreClockUpdate();

    initPeriph();
    pinsInit();
    timer_init();
    timer_int_init();

    __enable_irq();

    while (1)
    {
        __asm__ volatile("nop\n");
        __asm__ volatile("nop\n");
        __asm__ volatile("nop\n");
        __asm__ volatile("nop\n");
    }
}
