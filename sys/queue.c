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

#include "queue.h"

static void *_resize_queue(queue_t *queue)
{
    void *tmp_buf = realloc(queue->ptrs, queue->capacity * sizeof(queue_t) * 2);
    
    if (tmp_buf != queue->ptrs)
    {
        memcpy(tmp_buf, queue->ptrs, queue->size);
        free(queue->ptrs);
    }

    queue->ptrs = tmp_buf;

    return queue;
}

void queue_init(queue_t *queue)
{
    queue->capacity = DEFAULT_QUEUE_CAPACITY;
    queue->size = 0;
    queue->front = 0;
    queue->ptrs = malloc(DEFAULT_QUEUE_CAPACITY * sizeof(void*));

    if (!queue->ptrs)
    {
        /*KERNEL PANIC*/
    }
}

void queue_free(queue_t *queue)
{
    free(queue->ptrs);
}

void queue_push_back(queue_t *queue, const void *data)
{
    uint32_t nxt_pos;

    if (queue->size == queue->capacity)
    {
        queue = _resize_queue(queue);
    }

    nxt_pos = (queue->front + queue->size) & ~queue->capacity;
    queue->ptrs[nxt_pos] = data;
    ++queue->size;
}

void *queue_front(const queue_t *queue)
{
    if (queue->size == 0)
    {
        return NULL;
    }

    return queue->ptrs + queue->front;
}

void *queue_pop(queue_t *queue)
{
    void *buf;

    if (queue->size == 0)
    {
        return NULL;
    }

    buf = queue->ptrs[queue->front];
    queue->ptrs[queue->front] = NULL;
    ++queue->front;
    --queue->size;
    
    return buf;
}

uint32_t queue_size(const queue_t *queue)
{
    return queue->size;
}

uint32_t queue_capacity(const queue_t *queue)
{
    return queue->capacity;
}