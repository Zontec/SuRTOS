#ifndef __RAND_H__
#define __RAND_H__

#include "common_security.h"

typedef enum rand_status_e {
    RAND_OK = 0,
    RAND_FAIL = 1,
} rand_status_e;

uint32_t rand_bytes(uint8_t *buf, uint32_t buf_len);

#endif /*__RAND_H__*/