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
#include "memmap.h"
#include "platform_specific.h"



void os_load()
{
    disable_irq();

    PRINTF("\n\r\n\r");
    PRINTF("BOOTLOADER started!\n\r");

    /* HW init */
    PRINTF("HW initialization...\n\r");
    hw_req_init();
    
    /* Init security */
    PRINTF("Security initialization...\n\r");
    hw_security_req_init();

    /* Init memory manager */
    PRINTF("Memory manager initialization...\n\r");
    mem_manager_init();

    /* Init SMP */
    PRINTF("SMP initialization...\n\r");
    hw_smp_init();

    /* Init KMAL */
    PRINTF("KMAL initialization...\n\r");
    kmal_init();

    /* Init scheduler */
    PRINTF("Scheduler initialization...\n\r");
    sched_init();

    /* Create idle thread */
    PRINTF("Starting IDLE thread...\n\r");
    thread_create_idle();

    /* Create main thread*/
    PRINTF("Starting MAIN thread...\n\r");
    thread_create_main();

    PRINTF("CLI init...\n\r");
    cli_init();

    PRINTF("OS ready!\n\r");

    enable_irq();
    
    while (1)
    {

    }
}