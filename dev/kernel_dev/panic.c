#include "panic.h"
#include "hw.h"

void kernel_panic(const char *msg)
{

#ifdef PANIC_ARCH
    PANIC_ARCH();
#else
    #warning "PANIC_ARCH not implemented!"
#endif /*PANIC_ARCH*/

#ifdef CORE_DUMP_SAVE
    CORE_DUMP_SAVE();
#endif /*CORE_DUMP_SAVE*/

#ifdef OS_REG_CR
    OS_REG_CR |= OS_REG_CR_KP;
    OS_REG_CR |= OS_REG_CR_LOCK;
#endif /*OS_REG_CR*/

#ifdef ERROR_MEM_ADRESS
    FLASH_WRITE(ERROR_MEM_ADRESS, msg, strlen(msg));
#endif /*ERROR_MEM_ADRESS*/
    
#if SECURE_LEVEL == 2
    SECURE_CLEAR();
#endif /*SECURE_LEVEL*/

#ifdef KERNEL_STOP
    KERNEL_STOP();
#else
    #warning "KERNEL_STOP not implemented!"
#endif /*KERNEL_STOP*/

#ifdef INF_LOOP
    INF_LOOP;
#else
    #error "INF_LOOP macro should be implemented!"
#endif /*INF_LOOP*/
}
