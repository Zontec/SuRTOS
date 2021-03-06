/****************************INFORMATION***********************************
*
*
*
*
**************************************************************************/

/*!
*   @file
*   @brief SHA algorithm header file
*	@author Zontec
*	@version 1.0
*	@date 2021.04.04
*/

#include "stack.h"

static void *_resize_stack(stack_t *stack)
{
    void *tmp_buf = realloc(stack->ptrs, stack->capacity * sizeof(stack_t) * 2);
    
    if (tmp_buf != stack->ptrs)
    {
        memcpy(tmp_buf, stack->ptrs, stack->size);
        free(stack->ptrs);
    }

    stack->ptrs = tmp_buf;

    return stack;
}

void stack_init(stack_t *stack)
{
    stack->capacity = DEFAULT_STACK_CAPACITY;
    stack->size = 0;
    stack->ptrs = malloc(DEFAULT_STACK_CAPACITY * sizeof(void*));
    if (!stack->ptrs)
    {
        /*KERNEL PANIC*/
    }
}

void stack_free(stack_t *stack)
{
    free(stack->ptrs);
}

void stack_push_back(stack_t *stack, const void *data)
{
    uint32_t nxt_pos;

    if (stack->size == stack->capacity)
    {
        stack = _resize_stack(stack);
    }

    nxt_pos = stack->size;
    stack->ptrs[nxt_pos] = data;
    ++stack->size;
}

void *stack_top(const stack_t *stack)
{
    if (stack->size == 0)
    {
        return NULL;
    }
    return stack->ptrs + stack->size - 1;
}

void *stack_pop(stack_t *stack)
{
    void *buf;

    if (stack->size == 0)
    {
        return NULL;
    }

    buf = stack->ptrs[stack->size - 1];
    stack->ptrs[stack->size - 1] = NULL;
    --stack->size;

    return buf;
}

uint32_t stack_size(const stack_t *stack)
{
    return stack->size;
}

uint32_t stack_capacity(const stack_t *stack)
{
    return stack->capacity;
}