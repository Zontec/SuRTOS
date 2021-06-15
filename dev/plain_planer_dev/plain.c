#include "plain.h"


typedef struct _header_t
{
    uint32_t size : 31;
    uint8_t is_free : 1;
} _header_t;

extern void *__heap_base;
extern uint32_t __heap_size;

static uint32_t offset = 0;
static uint32_t blocks_allocated = 0;

static _header_t header;

void *memory_alloc(int size)
{
    if (size > __heap_size - offset || size == 0)
    {
        return NULL;
    }

    uint8_t *ret_ptr = (uint8_t *)__heap_base + offset + sizeof(_header_t);

    offset += size + sizeof(_header_t);

    header.size = size;
    header.is_free = 0; 

    *((uint32_t*)ret_ptr - 1) = *((uint32_t*)&header);

    ++blocks_allocated;

    printf("Size: %d\n", ((_header_t*)ret_ptr - 1)->size);
    printf("Free: %d\n", ((_header_t*)ret_ptr - 1)->is_free);

    return ret_ptr;
}

void memory_free( void *ptr)
{
    _header_t *tmp_header = ((_header_t*)ptr - 1);
    if (blocks_allocated == 0 || tmp_header->is_free || tmp_header->size == 0)
    {
        return;
    }
    tmp_header->is_free = 1;
    tmp_header->size = 0;
    --blocks_allocated;
    if(blocks_allocated == 0)
    {
        memory_force_free();
    }
}

void memory_realloc( void *ptr, int size)
{
    _header_t *tmp_header = ((_header_t*)ptr - 1);
    if (tmp_header->size >= size)
    {
        return;
    }

    return memory_alloc(size);
}

void memory_force_free()
{
    offset = 0;
    blocks_allocated = 0;
}