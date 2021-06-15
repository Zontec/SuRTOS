#include <stdint.h>
#include <stdlib.h>


#define SHA512_HASH_SIZE    (64)
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
    uint8_t buffer[128];
} sha512_t;


void sha512_init(sha512_t *ctx);

void sha512_update(sha512_t *ctx, const uint8_t *data, uint32_t data_len);

void sha512_finish(sha512_t *ctx, uint8_t *out);