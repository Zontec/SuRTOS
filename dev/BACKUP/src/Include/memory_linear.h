#ifndef __MEMORY_LINEAR_H__
#define __MEMORY_LINEAR_H__

#include "types.h"

void linear_init();

void *linear_memory_alloc(uint32_t size);

void *linear_memory_realloc(void *ptr, uint32_t size);

void linear_memory_free(void *ptr);

void linear_memory_force_free();

#endif /*__MEMORY_LINEAR_H__*/
