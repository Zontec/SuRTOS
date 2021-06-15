#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

#include "thread.h"
#include "memmap.h"

/* Import scheduler function */
extern void sched_run();

static uint32_t is_os_started = 0;

extern void *cur_thread;
extern void *nxt_thread;

void TIM7_IRQHandler(void)
{
   __disable_irq();
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        ICSR_REG |= (1 << 28);
    }
    __enable_irq();
}

void __attribute__((naked)) __attribute__((used)) PendSV_Handler() 
{

    ICSR_REG |= 1 << 27;

    __asm__ volatile("push  {lr} \n");
    sched_run();
    __asm__ volatile("pop   {lr} \n");

    if (cur_thread == nxt_thread)
    {
        __asm__ volatile("b sw_ctx_exit \n");
    }

    __asm__ volatile
    (
        "ldr     r3, =cur_thread   \n"   // R3 = cur_thread
        "ldr     r1, [r3]          \n"   // R1 = *cur_thread === cur_thread->sp
        "ldr     r2, =nxt_thread   \n"   // R2 = nxt_thread
        "ldr     r2, [r2]          \n"   // R2 = *nxt_thread === nxt_thread->sp
        "mrs     r0, psp           \n"  
    );
    if (is_os_started)
    {
        /* Save context and sp */
        __asm__ volatile
        (
            "sub     r0, r0, #32       \n"
            "str     r0, [r1]          \n"    // *R1 = R0 === cur_thread->sp
            "stmia   r0!, {r4-r11}     \n"
        );
    }
    /* Load new context */
    __asm__ volatile
    (
        " ldr     r0, [r2]         \n"    // Get SP of thread that be switch into.
        " ldmia   r0!, {r4-r11}	   \n"                
        " msr     psp, r0          \n"    // Mov new stack point to PSP
    );

    is_os_started = 1;

    __asm__ volatile(
        "sw_ctx_exit:     \n"
        "isb              \n"
        "bx lr            \n" 
    );
}