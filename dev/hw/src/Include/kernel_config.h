#ifndef __KERNEL_CONFIG_H__
#define __KERNEL_CONFIG_H__

#include "defines.h"

/*
*
*
*
*
*/
#ifndef ASSERT_ENABLED
#define ASSERT_ENABLED                      (TRUE)
#endif /* ASSERT_ENABLED */

/*
*
*
*
*
*/
#ifndef SECURITY_LEVEL
#define SECURITY_LEVEL                      (SECURITY_LEVEL_NORMAL)
#endif /* SECURITY_LEVEL */


/*
*
*
*
*
*/
#ifndef PRINT_ENABLE
#define PRINT_ENABLE                        (TRUE)
#endif /* PRINT_ENABLE */


/*
*
*
*
*
*/
#ifdef MEMORY_MANAGER_POOL

#define POLL0_BLOCK_SIZE                    (4)
#define POLL0_BLOCKS_NUM                    (8)
#define POOL0_SIZE                          (POLL0_BLOCK_SIZE * POLL0_BLOCKS_NUM)

#define POLL1_BLOCK_SIZE                    (32)
#define POLL1_BLOCKS_NUM                    (8)
#define POOL1_SIZE                          (POLL1_BLOCK_SIZE * POLL1_BLOCKS_NUM)

#define POLL2_BLOCK_SIZE                    (64)
#define POLL2_BLOCKS_NUM                    (8)
#define POOL2_SIZE                          (POLL2_BLOCK_SIZE * POLL2_BLOCKS_NUM)

#define POLL3_BLOCK_SIZE                    (128)
#define POLL3_BLOCKS_NUM                    (4)
#define POOL3_SIZE                          (POLL3_BLOCK_SIZE * POLL3_BLOCKS_NUM)

#define POLL4_BLOCK_SIZE                    (256)
#define POLL4_BLOCKS_NUM                    (4)
#define POOL4_SIZE                          (POLL4_BLOCK_SIZE * POLL4_BLOCKS_NUM)

#endif /* MEMORY_MANAGER_POOL */

#endif /* __KERNEL_CONFIG_H__ */