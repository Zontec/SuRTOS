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

#include "light_stack.h"
#include "memory_pool.h"

#ifndef HEAP_BASE
#error "HEAP_BASE not defined!"
#endif

#ifndef HEAP_SIZE
#error "HEAP_SIZE not defined!"
#endif

#if (POOL0_SIZE + POOL1_SIZE + POOL2_SIZE + POOL3_SIZE + POOL4_SIZE) > HEAP_SIZE
    #error memory size not enough!
#endif

typedef struct pool_t
{
    light_stack_t pool_stack;
    uint32_t block_size;
    uint32_t blocks_num;
} pool_t;

static pool_t _pools[5];

void mem_manager_init()
{
    uint8_t *p = (uint8_t*)HEAP_BASE;

    _pools[0].block_size = POLL0_BLOCK_SIZE;    
    _pools[0].blocks_num = POLL0_BLOCKS_NUM;  

    _pools[1].block_size = POLL1_BLOCK_SIZE;    
    _pools[1].blocks_num = POLL1_BLOCKS_NUM;  

    _pools[2].block_size = POLL2_BLOCK_SIZE;    
    _pools[2].blocks_num = POLL2_BLOCKS_NUM;  

    _pools[3].block_size = POLL3_BLOCK_SIZE;    
    _pools[3].blocks_num = POLL3_BLOCKS_NUM;  

    _pools[4].block_size = POLL4_BLOCK_SIZE;    
    _pools[4].blocks_num = POLL4_BLOCKS_NUM;  

    for (uint32_t i = 0; i < 5; ++i)
    {
        pool_t *pool = &_pools[i];

        light_stack_init(&pool->pool_stack);

        for (uint32_t j = 0; j < pool->blocks_num; ++j, p += pool->block_size)
        {
            light_stack_push_back(&pool->pool_stack, (lstack_data)p);
        }
    }
}

void *mem_manager_alloc(uint32_t size)
{
    void *p = NULL;

    for (uint32_t i = 0; i < 5; ++i)
    {
        if (_pools[i].block_size >= size && light_stack_size(&_pools[i].pool_stack) != 0)
        {
            p = (void*)light_stack_pop(&_pools[i].pool_stack);
            break;
        }
    }
    return p;
}

void mem_manager_free(void *ptr)
{
    uint8_t *p = (uint8_t*)HEAP_BASE;

    for (uint32_t i = 0; i < 5; ++i)
    {
        pool_t *pool = &_pools[i];

        for (uint32_t j = 0; j < pool->blocks_num; ++j, p += pool->block_size)
        {
            if (p == ptr)
            {
                light_stack_push_back(&pool->pool_stack, (lstack_data)p);
                return;
            }
        }      
    }
}

void *mem_manager_realloc(void *ptr, uint32_t size)
{
    uint8_t *p = (uint8_t*)HEAP_BASE;

    for (uint32_t i = 0; i < 5; ++i)
    {
        pool_t *pool = &_pools[i];

        for (uint32_t j = 0; j < pool->blocks_num; ++j, p += pool->block_size)
        {
            if (p == ptr)
            {
                if (pool->block_size > size)
                {
                    return p;
                }
                else 
                {
                    uint8_t *tmp_ptr = mem_manager_alloc(size);
                    if (!tmp_ptr)
                    {
                        return NULL;
                    }

                    memcpy(tmp_ptr, p, pool->block_size);
                    /*free(ptr)*/
                    light_stack_push_back(&pool->pool_stack, (lstack_data)p);

                    return tmp_ptr;
                }
            }
        }      
    }
    return NULL;
}
