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

extern void main();

void os_load()
{
    /* Init rest hw */

    /* Init serial */

    /* Init stuff */

    /* Init memory planer */

    /* Init SMP */

    /* Init scheduler */

    /* create idler thread */

    /* create thread with main*/
    mem_manager_init();
    
    main();
}