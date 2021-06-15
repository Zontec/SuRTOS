#include "plain.h"

uint8_t arr[2048];


void *__heap_base = arr;
uint32_t __heap_size = 2048;


int main()
{
    memory_alloc(10);
    memory_alloc(43);
    memory_alloc(54);
    memory_alloc(123);
    memory_alloc(413);
    memory_force_free();
    memory_alloc(2000);

    return 0;
}