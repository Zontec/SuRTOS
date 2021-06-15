#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

#include "types.h"

void pool_init();

void *pool_memory_alloc(uint32_t size);

void *pool_memory_realloc(void *ptr, uint32_t size);

void pool_memory_free(void *ptr);

void pool_memory_force_free();

#endif /*__MEMORY_POOL_H__*/
