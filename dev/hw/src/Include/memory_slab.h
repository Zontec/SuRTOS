#ifndef __MEMORY_SLAB_H__
#define __MEMORY_SLAB_H__

#include "types.h"
#include "memmap.h"

void mem_manager_init();

void *mem_manager_alloc(uint32_t size);

void mem_manager_free(void *ptr);

void *mem_manager_realloc(void *ptr, uint32_t size);

#endif /* __MEMORY_SLAB_H__ */
