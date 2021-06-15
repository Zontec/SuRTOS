#ifndef __RAND_H__
#define __RAND_H__

#include "security.h"

#define ASSERT(state, msg) 
#define RAND_FAST


int32_t rand();

void srand(uint32_t seed);

#endif /* __RAND_H__ */