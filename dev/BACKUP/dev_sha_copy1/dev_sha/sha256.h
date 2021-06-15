#include <stdint.h>
#include <stdlib.h>

#define SHA256_HASH_SIZE    (32)

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
    uint8_t buffer[64];
} sha256_t;


void sha256_init(sha256_t *ctx);

void sha256_update(sha256_t *ctx, const uint8_t *data, uint32_t data_len);

void sha256_finish(sha256_t *ctx, uint8_t *out);

void sha256(const uint8_t *data, uint32_t data_len, uint8_t *out);
