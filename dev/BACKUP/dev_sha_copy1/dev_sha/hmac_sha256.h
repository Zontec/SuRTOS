#ifndef __HMAC_SHA256_H__
#define __HMAC_SHA256_H__

#include "sha256.h"

#define HMAC_SHA256_BLOCK_SIZE            (64)
#define HMAC_SHA256_HASH_SIZE             (SHA256_HASH_SIZE)


typedef struct hmac_sha256_t
{
    sha256_t sha_ctx;
    uint8_t key[HMAC_SHA256_BLOCK_SIZE];
} hmac_sha256_t;


void hmac_sha256(const uint8_t *key, uint32_t key_len, 
                const uint8_t *data, uint32_t data_len, uint8_t *out);

void hmac_sha256_init(hmac_sha256_t *ctx, const uint8_t *key, uint32_t key_len);

void hmac_sha256_update(hmac_sha256_t *ctx, const uint8_t *data, uint32_t data_len);

void hmac_sha256_final(hmac_sha256_t *ctx, uint8_t *out);

#endif /* __HMAC_SHA256_H__ */