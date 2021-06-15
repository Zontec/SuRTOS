#ifndef __LIGHT_STACK_H__
#define __LIGHT_STACK_H__

#include "types.h"

#ifndef LIGHT_STACK_SIZE
#define LIGHT_STACK_SIZE        (16)
#endif

typedef MEMORY_ADRESS_DATA_TYPE lstack_data;

typedef struct light_stack_t 
{
    lstack_data ptrs[LIGHT_STACK_SIZE];
    uint32_t size;
} light_stack_t;

void light_stack_init(light_stack_t *light_stack);

void light_stack_free(light_stack_t *light_stack);

void light_stack_push_back(light_stack_t *light_stack, lstack_data data);

lstack_data light_stack_top(const light_stack_t *light_stack);

lstack_data light_stack_pop(light_stack_t *light_stack);

uint32_t light_stack_size(const light_stack_t *light_stack);

#endif /*__LIGHT_STACK_H__*/