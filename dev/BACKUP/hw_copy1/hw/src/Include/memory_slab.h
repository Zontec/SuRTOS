#ifndef __MEMORY_SLAB_H__
#define __MEMORY_SLAB_H__

#include "types.h"

void slab_init();

void *slab_memory_alloc(uint32_t size);

void *slab_memory_realloc(void *ptr, uint32_t size);

void slab_memory_free(void *ptr);

void slab_memory_force_free();

#endif /*__MEMORY_SLAB_H__*/
