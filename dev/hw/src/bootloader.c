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

#include "types.h"
#include "platform_specific.h"

extern void main();

static void inline __attribute__((naked)) ___jmp_main()
{
    __asm__ volatile("bl main");
    while (1)
    {

    }
    
}

#define PRINT_INFO (void) 
void os_load()
{
    /* Init rest hw */
    /* Init serial */

    PRINT_INFO("\r\n\r\n");
    PRINT_INFO("BOOTLOADER started!\n\r");

    /* Init memory manager */

    PRINT_INFO("Memory manager initialization...\n\r");

    mem_manager_init();


    /* Init security */

    PRINT_INFO("Security initialization...\n\r");

    /* Init SMP */

    PRINT_INFO("SMP initialization...\n\r");

    /* Init scheduler */

    PRINT_INFO("Scheduler initialization...\n\r");

    /* create idler thread */

    PRINT_INFO("Starting IDLE thread...\n\r");

    /* create thread with main*/

    PRINT_INFO("Starting MAIN thread...\n\r");

    PRINT_INFO("CLI ready!\n\r");

    ___jmp_main();
}