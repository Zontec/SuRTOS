#ifndef __THREAD_H__
#define __THREAD_H__

#include "types.h"

#ifndef THREAD_STACKSIZE_DEFAULT
#define THREAD_STACKSIZE_DEFAULT    (512)
#endif 

#ifndef THREAD_STACKSIZE_MAIN
#define THREAD_STACKSIZE_MAIN      (THREAD_STACKSIZE_DEFAULT + \
                                    THREAD_EXTRA_STACKSIZE_PRINTF)
#endif

#ifndef THREAD_STACKSIZE_LARGE
#define THREAD_STACKSIZE_LARGE (THREAD_STACKSIZE_MEDIUM * 2)
#endif

#ifndef THREAD_STACKSIZE_MEDIUM
#define THREAD_STACKSIZE_MEDIUM THREAD_STACKSIZE_DEFAULT
#endif

#ifndef THREAD_STACKSIZE_SMALL
#define THREAD_STACKSIZE_SMALL (THREAD_STACKSIZE_MEDIUM / 2)
#endif

#ifndef THREAD_STACKSIZE_TINY
#define THREAD_STACKSIZE_TINY (THREAD_STACKSIZE_MEDIUM / 4)
#endif

#ifndef THREAD_STACKSIZE_MINIMUM
#define THREAD_STACKSIZE_MINIMUM  (sizeof(thread_t))
#endif

typedef void *(*task_t)(void *arg);

typedef enum thread_status_e
{
    THREAD_STATUS_WORKING =  0,
    THREAD_STATUS_STOPED =   1,
    THREAD_STATUS_KILLED =   2,
    THREAD_STATUS_WAITING =  3,
} thread_status_e;

typedef enum thread_priority_e
{
    THREAD_PRIORITY_IDLE =       0,
    THREAD_PRIORITY_ORDINARY =   1,
    THREAD_PRIORITY_HIGH =       2,
    THREAD_PRIORITY_EXTRA =      3,
} thread_priority_e;

typedef struct thread_t 
{
    uint8_t *sp;
    uint8_t *sp_start;
    uint32_t stack_size;
    thread_priority_e priority;
    thread_status_e status;
    uint32_t thread_id;
} thread_t;

uint32_t thread_create(uint8_t *stack,
                           uint32_t stacksize,
                           thread_priority_e priority,
                           uint32_t flags,
                           task_t callback,
                           void *arg);


void thread_sleep();

void thread_yield();

void thread_yield_higher();

void thread_zombify();

uint32_t thread_kill_zombie(uint32_t thread_id);

uint32_t thread_wakeup(uint32_t thread_id);


uint8_t *thread_stack_init(task_t callback, void *arg,
                        void *stack_start, uint32_t stack_size);

static inline thread_t *thread_get_active(void)
{
    extern volatile thread_t *sched_active_thread;

    return (thread_t *)sched_active_thread;
}

#endif /* __THREAD_H__ */