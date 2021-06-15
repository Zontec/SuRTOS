#include "light_stack.h"

void light_stack_init(light_stack_t *light_stack)
{
    light_stack->size = 0;
}

void light_stack_free(light_stack_t *light_stack)
{
    light_stack->size = 0;
}

void light_stack_push_back(light_stack_t *light_stack, lstack_data data)
{
    uint32_t nxt_pos = light_stack->size;
    memcpy(&light_stack->ptrs[nxt_pos], &data, sizeof(lstack_data));
    ++light_stack->size;
}

lstack_data light_stack_top(const light_stack_t *light_stack)
{
    if (light_stack->size == 0)
    {
        return (lstack_data)0;
    }
    return *(light_stack->ptrs + light_stack->size - 1);
}

lstack_data light_stack_pop(light_stack_t *light_stack)
{
    if (light_stack->size == 0)
    {
        return (lstack_data)0;
    }

    lstack_data buf;

    memcpy(&buf, &light_stack->ptrs[light_stack->size - 1], sizeof(lstack_data));

    light_stack->ptrs[light_stack->size - 1] = (lstack_data)0;
    --light_stack->size;

    return buf;
}

uint32_t light_stack_size(const light_stack_t *light_stack)
{
    return light_stack->size;
}
