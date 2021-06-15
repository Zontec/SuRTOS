#ifndef __HMAC_SHA512_H__
#define __HMAC_SHA512_H__

#include "sha512.h"

#define HMAC_SHA512_BLOCK_SIZE            (128)
#define HMAC_SHA512_HASH_SIZE             (SHA512_HASH_SIZE)


typedef struct hmac_sha512_t
{
    sha512_t sha_ctx;
    uint8_t key[HMAC_SHA512_BLOCK_SIZE];
} hmac_sha512_t;


void hmac_sha512(const uint8_t *key, uint32_t key_len, 
                const uint8_t *data, uint32_t data_len, uint8_t *out);

void hmac_sha512_init(hmac_sha512_t *ctx, const uint8_t *key, uint32_t key_len);

void hmac_sha512_update(hmac_sha512_t *ctx, const uint8_t *data, uint32_t data_len);

void hmac_sha512_final(hmac_sha512_t *ctx, uint8_t *out);

#endif /* __HMAC_SHA512_H__ */