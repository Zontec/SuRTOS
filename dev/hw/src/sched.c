#include "sched.h"

thread_t *sched_run(void)
{
/*
    thread_t *active_thread = thread_get_active();
    thread_t *previous_thread = active_thread;

    if (!IS_USED(MODULE_CORE_IDLE_THREAD) && !runqueue_bitcache) {
        if (active_thread) {
            _unschedule(active_thread);
            active_thread = NULL;
        }

        do {
            sched_arch_idle();
        } while (!runqueue_bitcache);
    }

    sched_context_switch_request = 0;

    unsigned nextrq = _get_prio_queue_from_runqueue();
    thread_t *next_thread = container_of(sched_runqueues[nextrq].next->next,
                                         thread_t, rq_entry);

    DEBUG(
        "sched_run: active thread: %" PRIkernel_pid ", next thread: %" PRIkernel_pid "\n",
        (kernel_pid_t)((active_thread == NULL)
                       ? KERNEL_PID_UNDEF
                       : active_thread->pid),
        next_thread->pid);

    next_thread->status = STATUS_RUNNING;

    if (previous_thread == next_thread) {
#ifdef MODULE_SCHED_CB
       
        if (sched_cb && !active_thread) {
            sched_cb(KERNEL_PID_UNDEF, next_thread->pid);
        }
#endif
        DEBUG("sched_run: done, sched_active_thread was not changed.\n");
    }
    else {
        if (active_thread) {
            _unschedule(active_thread);
        }

        sched_active_pid = next_thread->pid;
        sched_active_thread = next_thread;

#ifdef MODULE_SCHED_CB
        if (sched_cb) {
            sched_cb(KERNEL_PID_UNDEF, next_thread->pid);
        }
#endif

#ifdef PICOLIBC_TLS
        _set_tls(next_thread->tls);
#endif

#ifdef MODULE_MPU_STACK_GUARD
        mpu_configure(
            2,                                              
            (uintptr_t)next_thread->stack_start + 31,       
            MPU_ATTR(1, AP_RO_RO, 0, 1, 0, 1, MPU_SIZE_32B) 
            );
#endif
        DEBUG("sched_run: done, changed sched_active_thread.\n");
    }

    return next_thread;
*/
}