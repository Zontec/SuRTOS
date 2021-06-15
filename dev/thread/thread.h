#ifndef __THREAD_H__
#define __THREAD_H__

typedef struct thread_status_t
{

} thread_status_t;

typedef struct thread_t
{
    uint8_t *SP;
    thread_status_t status;
    uint32_t thread_priority;
    uint32_t core_id;

}

#endif /*__THREAD_H__*/