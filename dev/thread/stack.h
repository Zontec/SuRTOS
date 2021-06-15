#ifndef __STACK_H__
#define __STACK_H__

#include "types.h"
#include "product_specific.h"

#ifndef DEFAULT_STACK_CAPACITY
#define DEFAULT_STACK_CAPACITY (32)
#endif

typedef struct stack_t 
{
    void **ptrs;
    uint32_t size;
    uint32_t capacity;
} stack_t;

void stack_init(stack_t *stack);

void stack_push(stack_t *stack, const void *data);

void *stack_top(const stack_t *stack);

void *stack_pop(stack_t *stack);

uint32_t stack_size(const stack_t *stack);

uint32_t stack_capacity(const stack_t *stack);


#endif /*__STACK_H__*/