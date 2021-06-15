#ifndef __HMAC_SHA256_H__
#define __HMAC_SHA256_H__

#include "security.h"
#include "sha256.h"

#define HMAC_SHA256_BLOCK_SIZE            (64)
#define HMAC_SHA256_HASH_SIZE             (SHA256_HASH_SIZE)

typedef struct hmac_sha256_t
{
    sha256_t sha_ctx;
    uint8_t key[HMAC_SHA256_BLOCK_SIZE];
} hmac_sha256_t;

security_status_e hmac_sha256(const uint8_t *key, uint32_t key_len, 
                const uint8_t *data, uint32_t data_len, uint8_t *out);

security_status_e hmac_sha256_init(hmac_sha256_t *ctx, const uint8_t *key, uint32_t key_len);

security_status_e hmac_sha256_update(hmac_sha256_t *ctx, const uint8_t *data, uint32_t data_len);

security_status_e hmac_sha256_finish(hmac_sha256_t *ctx, uint8_t *out);

#endif /* __HMAC_SHA256_H__ */