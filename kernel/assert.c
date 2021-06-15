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

#include "assert.h"

void assert(uint8_t state, const char *msg, const char *file, uint32_t line)
{
    if (!state)
    {
        PRINT_ERROR("%s:%d => ASSERT FAIL\r\n", file, line); 
        PRINT_ERROR("Msg: %s\r\n", msg); 
        kernel_panic(msg); 
    }
}
