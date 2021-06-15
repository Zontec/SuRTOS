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

#define wr_psp(var) __asm__ volatile("mrs %0, psp\n" : "=r" (var));
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

uint32_t *tsp;
int cnt =0;
uint32_t arr[128];

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

    while (nCount-->0) 
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
    rd_psp(tsp);
    arr[cnt++] = (uint32_t)tsp;
    while (1)
    {
        __disable_irq();
        GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
        __enable_irq();
        Delay(1000);
    }
}
void led2()
{    
    rd_psp(tsp);
    arr[cnt++] = (uint32_t)tsp;
    while (1)
    {
        __disable_irq();
        GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
        __enable_irq();
        Delay(1000);
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

    __asm__ volatile("push {r0-r7, lr}\n");
    
    rd_psp(tsp);
    arr[cnt] = (uint32_t)tsp;

    nxt_task_ind = (cur_task_ind + 1) & 1;


    if (is_os_started)
    {
        __asm__ volatile("mrs %0, psp\n" : "=r" (tasks[cur_task_ind].sp));
    }
    
    cur_task_ind = nxt_task_ind;

    is_os_started = 1;

    __asm__ volatile("msr psp, %0\n"::"r"(tasks[cur_task_ind].sp));

    rd_psp(tsp);
    arr[cnt+1] = (uint32_t)tsp;
    cnt += 2;

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

    sys_set_control(USE_PSP_IN_THREAD_MODE);

    //wr_psp(0x200010000)
    //wr_msp(0x200040000)

    thread_init((uint32_t*)(((uint32_t)(stack1) & ~7)+8), led1);
    thread_init((uint32_t*)(((uint32_t)(stack2) & ~7)+8), led2);
    //thread_init(stack3, led3);
    //thread_init(stack4, led4);
    printf("%p\n\r", tasks[0].sp);
    printf("%p\n\r", tasks[1].sp);

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
