#ifndef __SHA1_H__
#define __SHA1_H__

#include <stdint.h>
#include <stdlib.h>

#define SHA1_HASH_SIZE              (20)
#define SHA1_BUFFER_SIZE            (64)

typedef struct sha1_t
{
    uint32_t h0;
    uint32_t h1;
    uint32_t h2;
    uint32_t h3;
    uint32_t h4;
    uint32_t total[2];
    uint8_t buffer[SHA1_BUFFER_SIZE];
} sha1_t;


void sha1_init(sha1_t *ctx);

void sha1_update(sha1_t *ctx, const uint8_t *data, uint32_t data_len);

void sha1_finish(sha1_t *ctx, uint8_t *out);

void sha1(const uint8_t *data, uint32_t data_len, uint8_t *out);

#endif /* __SHA1_H__ */
