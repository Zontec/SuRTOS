#include "thread.h"

#include "panic.h"
#include "defines.h"


extern void sched_add_thread(thread_t thread);

extern void main();
extern void idle();
extern void inf();

#define BASE_THREAD_ID      0x10

#define IDLE_THREAD_ID      0x11
#define MAIN_THREAD_ID      0x12

uint8_t *idle_stack[THREAD_IDLE_STACK_SIZE] __attribute__((aligned(8)));
uint8_t *main_stack[THREAD_MAIN_STACK_SIZE] __attribute__((aligned(8)));


void thread_yield()
{

}


static thread_id_t thread_nxt_id()
{
    static cur_thread_id = BASE_THREAD_ID;
    ++cur_thread_id;
    
    if (cur_thread_id < BASE_THREAD_ID)
    {
        /* PANIC */
    }

    return cur_thread_id;
}



thread_id_t thread_create(uint8_t *sp, uint32_t stack_size, 
                            thread_prior_e prior, thread_flags_t flags, 
                            task_t task, void *task_arg, const char *thread_name)
{
    uint8_t *tmp_sp = sp; /* Points at the very beginning of the stack */
    thread_t *tmp_thread;

    /* Align sp to 8 bytes */
    sp = (uint8_t*)((((uint32_t)sp) & ~7) + 8);

    /* sp base that points to struct begin */
    tmp_thread = sp;
    tmp_thread->sp_base = sp;

    /* set free bytes to guard bytes*/
    memset(tmp_sp, GUARD_MAGIC_BYTE, sp - tmp_sp);

    /* Offset thread structure*/
    sp += sizeof(thread_t);

    /* Put guard magic on the stack */
    memset(sp, GUARD_MAGIC_BYTE, 8);
    sp += 8;

    /* Build HW dependent part */
    tmp_thread->sp = hw_build_stack(sp, stack_size - (sp - tmp_sp), task, task_arg);

    /* Fill the other fields */
    tmp_thread->stack_size = stack_size;
    tmp_thread->prior = prior;
    tmp_thread->flags = flags;
    tmp_thread->thread_name = thread_name;
    tmp_thread->task = task;
    tmp_thread->task_arg = task_arg;
    tmp_thread->status.w[0] = 0;
    tmp_thread->status.w[1] = 0;
    tmp_thread->id = thread_nxt_id();

    /* Add thread to a scheduler */
    sched_add_thread(*tmp_thread);

    /* Return thread id */
    return tmp_thread->id = thread_nxt_id();
}

void thread_create_idle()
{
    thread_create(idle_stack, THREAD_IDLE_STACK_SIZE, 0, 0, idle, 0, "idle");
}

void thread_create_main()
{
    thread_create(main_stack, THREAD_IDLE_STACK_SIZE, 0, 0, main, 0, "main");
}