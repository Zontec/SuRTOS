#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include "types.h"
#include "kernel.h"
#include "kernel_config.h"

extern uint32_t __heap_top;
extern uint32_t __heap_bottom;

#ifndef MEMORY_ALIGNMENT
#define MEMORY_MANAGER_MEMORY_ALIGNMENT              (4)
#else
#define MEMORY_MANAGER_MEMORY_ALIGNMENT              MEMORY_ALIGNMENT
#endif

#ifndef SECURITY_LEVEL
#define MEMORY_MANAGER_SECURITY_LEVEL                (1)
#else
#define MEMORY_MANAGER_SECURITY_LEVEL                SECURITY_LEVEL
#endif


#define MEMORY_ALLOCATOR_TYPE MEMORY_ALLOCATOR_LINEAR
#if (MEMORY_ALLOCATOR_TYPE == MEMORY_ALLOCATOR_SLAB)
#define MEMORY_MANAGER_MEMORY_ALLOCATION_IS_SLAB
#elif (MEMORY_ALLOCATOR_TYPE == MEMORY_ALLOCATOR_SLOB)
#define MEMORY_MANAGER_MEMORY_ALLOCATION_IS_SLOB
#elif (MEMORY_ALLOCATOR_TYPE == MEMORY_ALLOCATOR_SLUB)
#define MEMORY_MANAGER_MEMORY_ALLOCATION_IS_SLUB
#elif (MEMORY_ALLOCATOR_TYPE == MEMORY_ALLOCATOR_LINEAR)
#define MEMORY_MANAGER_MEMORY_ALLOCATION_IS_LINEAR
#elif (MEMORY_ALLOCATOR_TYPE == MEMORY_ALLOCATOR_BLOCK)
#define MEMORY_MANAGER_MEMORY_ALLOCATION_IS_BLOCK
#elif (MEMORY_ALLOCATOR_TYPE == MEMORY_ALLOCATOR_BUDDY)
#define MEMORY_MANAGER_MEMORY_ALLOCATION_IS_BUDDY
#endif

void memory_manager_init();

void *memory_manager_alloc_mem(const uint32_t len);

void *memory_manager_realloc_mem(const uint8_t *ptr, const uint32_t len);

void memory_manager_free_mem(const uint32_t len);

#endif /*__MEMORY_MANAGER_H__*/