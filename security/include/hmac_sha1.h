#ifndef __HMAC_SHA1_H__
#define __HMAC_SHA1_H__

#include "security.h"
#include "sha1.h"

#define HMAC_SHA1_BLOCK_SIZE            (64)
#define HMAC_SHA1_HASH_SIZE             (SHA1_HASH_SIZE)

typedef struct hmac_sha1_t
{
    sha1_t sha_ctx;
    uint8_t key[HMAC_SHA1_BLOCK_SIZE];
} hmac_sha1_t;

security_status_e hmac_sha1(const uint8_t *key, uint32_t key_len, 
                const uint8_t *data, uint32_t data_len, uint8_t *out);

security_status_e hmac_sha1_init(hmac_sha1_t *ctx, const uint8_t *key, uint32_t key_len);

security_status_e hmac_sha1_update(hmac_sha1_t *ctx, const uint8_t *data, uint32_t data_len);

security_status_e hmac_sha1_finish(hmac_sha1_t *ctx, uint8_t *out);

#endif /* __HMAC_SHA1_H__ */