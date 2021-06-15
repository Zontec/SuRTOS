#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "defines.h"

#include "defines.h"
#include "macros.h"
#include "memmap.h"


#define NOP_ROAD{           \
        ASM("nop    \n");   \
        ASM("nop    \n");   \
        ASM("nop    \n");   \
        ASM("nop    \n");   \
        ASM("nop    \n");   \
        ASM("nop    \n");   \
        ASM("nop    \n");   \
        ASM("nop    \n");   \
        ASM("nop    \n");   \
        ASM("nop    \n");   \
    }while (0)              \

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

void inf()
{
    while (1)
    {
        NOP_ROAD;
    }
}

void idle()
{
    while (1)
    {
        NOP_ROAD;
    }
}

void kmal_init()
{
    timer_init();
    timer_int_init();
}

void enable_irq(void)
{
    ASM("cpsie i" : : : "memory");
}

 void disable_irq(void)
{
    ASM("cpsid i" : : : "memory");
}

void hw_smp_init()
{
    /* NO SMP */
}

void hw_sw_ctx()
{
    ICSR_REG |= 1 << 27;
}

void hw_req_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
}

void hw_security_req_init()
{
    /* NO SPECIFIC SECURITY */
}

uint8_t *hw_build_stack(uint8_t *sp, uint32_t stack_size, 
                        mem_address_t task, mem_address_t task_arg)
{
    stack_frame_t sf;

    memset(&sf, 0, sizeof(stack_frame_t));
    memset(sp, STACK_MAGIC_BYTE, stack_size);

    sf.pc = task & 0xFFFFFFFE;
    sf.psr = 0x21000000;
    sf.lr = 0x00;
    sp = sp + stack_size - sizeof(stack_frame_t);
    memcpy(sp, &sf, sizeof(stack_frame_t));

    return sp;
}

void timer_init()
{
    TIM_TimeBaseInitTypeDef TIMER_InitStructure;
 	TIM_TimeBaseStructInit(&TIMER_InitStructure);
    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIMER_InitStructure.TIM_Prescaler = 8399;
    TIMER_InitStructure.TIM_Period = TIMER_SPEED;
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
