#include "thread.h"

static thread_t threads[10];
static uint32_t threads_num;

static uint32_t cur_thread_ind = 0;
static uint32_t nxt_thread_ind = 0;

void *cur_thread;
void *nxt_thread;


void sched_add_thread(thread_t thread)
{
    threads[threads_num] = thread;
    ++threads_num;
}

void sched_run()
{
    nxt_thread_ind = (cur_thread_ind + 1) % threads_num;
    cur_thread = &threads[cur_thread_ind];
    nxt_thread = &threads[nxt_thread_ind];
    cur_thread_ind = nxt_thread_ind;
}

void sched_init()
{
    threads_num = 0;
}