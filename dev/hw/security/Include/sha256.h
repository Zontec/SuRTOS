#ifndef __SHA256_H__
#define __SHA256_H__

#include "security.h"

#define SHA256_HASH_SIZE            (32)
#define SHA256_BUFFER_SIZE          (64)

typedef struct sha256_t
{
    uint32_t h0;
    uint32_t h1;
    uint32_t h2;
    uint32_t h3;
    uint32_t h4;
    uint32_t h5;
    uint32_t h6;
    uint32_t h7;
    uint32_t total[2];
    uint8_t buffer[SHA256_BUFFER_SIZE];
} sha256_t;


security_status_e sha256_init(sha256_t *ctx);

security_status_e sha256_update(sha256_t *ctx, const uint8_t *data, uint32_t data_len);

security_status_e sha256_finish(sha256_t *ctx, uint8_t *out);

security_status_e sha256(const uint8_t *data, uint32_t data_len, uint8_t *out);

#endif /* __SHA256_H__ */
