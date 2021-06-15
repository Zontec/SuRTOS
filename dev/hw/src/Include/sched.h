#ifndef __SCHED_H__
#define __SCHED_H__

#include "thread.h"
#include "types.h"

thread_t *sched_run(void);

void sched_switch(uint16_t other_prio);

#endif /* __SCHED_H__ */