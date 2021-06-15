#include <stdint.h>

#define THREAD_MIN_STACK_SIZE                 (128)
#define THREAD_MID_STACK_SIZE                 (512)
#define THREAD_BIG_STACK_SIZE                 (1024)
#define THREAD_LARGE_STACK_SIZE               (2048)
#define THREAD_EXTRA_LARGE_STACK_SIZE         (4096)

#define THREAD_MAIN_STACK_SIZE                THREAD_MID_STACK_SIZE
#define THREAD_IDLE_STACK_SIZE                THREAD_MIN_STACK_SIZE

typedef struct thread_stat_t
{
    union
    {
        struct 
        {
            uint32_t special    : 1;
            uint32_t state      : 4;
            uint32_t owner_id   : 5;
            uint32_t exit_code  : 8;
            uint32_t reserved   : 14;

            uint32_t calls;
        };
        struct 
        {
            uint32_t w[2];
        };
    };
} thread_stat_t;

typedef uint32_t thread_flags_t;
typedef uint32_t thread_id_t;

typedef enum thread_prior_e
{
    MAX_PRIORITY =    0,
    MEDIUM_PRIORITY = 1,
    LOW_PRIORITY =    2,
    IDLE_PRIORITY =   3,
} thread_prior_e;

enum
{
    THREAD_IDLE =    0,
    THREAD_STOPED = 1,
    THREAD_RUNNING =    2,
    THREAD_KILLED =   3,
};

typedef void(*task_t)(void*);

typedef struct thread_t 
{
    uint8_t *sp;
    uint8_t *sp_base;
    uint32_t stack_size;

    task_t task;
    void *task_arg;

    thread_stat_t status;
    thread_prior_e prior;
    thread_id_t id;
    thread_flags_t flags;

    void *mbox_queue;
    const char *thread_name;
} thread_t __attribute__((aligned(4)));



/**
 * @brief 
 * 
 * @param sp 
 * @param stack_size 
 * @param prior 
 * @param flags 
 * @param task 
 * @param task_arg 
 * @param thread_name 
 * @return thread_id_t 
 */
thread_id_t thread_create(uint8_t *sp, uint32_t stack_size, 
                            thread_prior_e prior, thread_flags_t flags, 
                            task_t task, void *task_arg, const char *thread_name);

void thread_yield();