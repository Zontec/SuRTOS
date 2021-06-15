#ifndef __SHA512_H__
#define __SHA512_H__

#include "security.h"

#define SHA512_HASH_SIZE            (64)
#define SHA512_BUFFER_SIZE          (128)

typedef struct sha512_t
{
    uint64_t h0;
    uint64_t h1;
    uint64_t h2;
    uint64_t h3;
    uint64_t h4;
    uint64_t h5;
    uint64_t h6;
    uint64_t h7;
    uint32_t total[2];
    uint8_t buffer[SHA512_BUFFER_SIZE];
} sha512_t;


security_status_e sha512_init(sha512_t *ctx);

security_status_e sha512_update(sha512_t *ctx, const uint8_t *data, uint32_t data_len);

security_status_e sha512_finish(sha512_t *ctx, uint8_t *out);

security_status_e sha512(const uint8_t *data, uint32_t data_len, uint8_t *out);

#endif /*__SHA512_H__*/
