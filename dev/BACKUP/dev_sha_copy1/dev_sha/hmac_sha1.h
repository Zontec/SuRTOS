#ifndef __HMAC_SHA1_H__
#define __HMAC_SHA1_H__

#include "sha1.h"

#define HMAC_SHA1_BLOCK_SIZE            (64)
#define HMAC_SHA1_HASH_SIZE             (SHA1_HASH_SIZE)


typedef struct hmac_sha1_t
{
    sha1_t sha_ctx;
    uint8_t key[HMAC_SHA1_BLOCK_SIZE];
} hmac_sha1_t;


void hmac_sha1(const uint8_t *key, uint32_t key_len, 
                const uint8_t *data, uint32_t data_len, uint8_t *out);

void hmac_sha1_init(hmac_sha1_t *ctx, const uint8_t *key, uint32_t key_len);

void hmac_sha1_update(hmac_sha1_t *ctx, const uint8_t *data, uint32_t data_len);

void hmac_sha1_final(hmac_sha1_t *ctx, uint8_t *out);

#endif /* __HMAC_SHA1_H__ */