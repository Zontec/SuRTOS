#include "assert.h"

void assert(uint8_t state, const char *msg, const char *file, uint32_t line)
{
    if (!state)
    {
        PRINT_ERROR("%s:%ld => ASSERT FAIL\n", file, line); 
        PRINT_ERROR("Msg: %s\n", msg); 
        kernel_panic(msg); 
    }
}
