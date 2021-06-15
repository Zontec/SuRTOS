#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "types.h"
#include "platform_specific.h"

#ifndef DEFAULT_QUEUE_CAPACITY
#define DEFAULT_QUEUE_CAPACITY (32)
#endif

typedef struct queue_t 
{
    void **ptrs;
    uint32_t front;
    uint32_t size;
    uint32_t capacity;
} queue_t;

void queue_init(queue_t *queue);

void queue_push_back(queue_t *queue, const void *data);

void *queue_front(const queue_t *queue);

void *queue_pop(queue_t *queue);

uint32_t queue_size(const queue_t *queue);

uint32_t queue_capacity(const queue_t *queue);


#endif /*__QUEUE_H__*/