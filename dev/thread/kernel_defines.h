#ifndef __KERNEL_DEFINES_H__
#define __KERNEL_DEFINES_H__

#define ALIGN(x, a)              __ALIGN_MASK((x), (a) - 1)
#define __ALIGN_MASK(x, mask)    (((x) + (mask)) & ~(mask))  


#endif /*__KERNEL_DEFINES_H__*/