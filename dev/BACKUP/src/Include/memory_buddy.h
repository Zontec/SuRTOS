#ifndef __MEMORY_BUDDY_H__
#define __MEMORY_BUDDY_H__

#include "types.h"

void buddy_init();

void *buddy_memory_alloc(uint32_t size);

void *buddy_memory_realloc(void *ptr, uint32_t size);

void buddy_memory_free(void *ptr);

void buddy_memory_force_free();

#endif /*__MEMORY_BUDDY_H__*/
