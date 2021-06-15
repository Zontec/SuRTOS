#ifndef __KERNEL_KONFIG_H__
#define __KERNEL_KONFIG_H__

#include "kernel.h"

/**
*
*   Here is choose of memory allocation type
*
**/

#ifndef MEMORY_ALLOCATOR_TYPE
#define MEMORY_ALLOCATOR_TYPE               MEMORY_ALLOCATOR_LINEAR
#endif

/**
*
*   Here is choose of memory allocation type
*
**/

#ifndef SCHEDULER_TYPE
#define SCHEDULER_TYPE               MEMORY_ALLOCATOR_LINEAR
#endif

/**
*
*   Here is choose of memory allocation type
*
**/

#ifndef ITC_TYPE
#define ITC_TYPE               MEMORY_ALLOCATOR_LINEAR
#endif

/**
*
*   Here is choose of memory allocation type
*
**/



#endif /*__KERNEL_KONFIG_H__*/