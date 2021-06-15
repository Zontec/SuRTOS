#include <stdint.h>
#include <stdlib.h>

void *memory_alloc(int size);

void memory_free( void *ptr);
void memory_force_free();