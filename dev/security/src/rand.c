#include "rand.h"


static uint32_t _rand_bytes(uint8_t *buf, uint32_t buf_len) {
    return RAND_bytes(buf, buf_len);
}

uint32_t rand_bytes(uint8_t *buf, uint32_t buf_len) {
    if (!buf || !buf_len) {
        return RAND_FAIL;
    }
    return _rand_bytes(buf, buf_len);
}