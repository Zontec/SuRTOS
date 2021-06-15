#ifndef __ASSERT_H__
#define __ASSERT_H__

#include "types.h"
#include "kernel_config.h"
#include "platform_defines.h"


#ifndef __FILE__
#define __FILE__    "unknown!"
#endif

#ifndef __LINE__
#define __LINE__    (0)
#endif

#if (ASSERT_ENABLED == TRUE)
#define ASSERT(state, msg) assert(state, msg, __FILE__, __LINE__)
#else
#define ASSERT(state)
#endif

void assert(uint8_t state, const char *msg, const char *file, uint32_t line);

#endif /*__ASSERT_H__*/