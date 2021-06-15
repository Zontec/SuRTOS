#ifndef __HMAC_SHA512_H__
#define __HMAC_SHA512_H__

#include "security.h"
#include "sha512.h"

#define HMAC_SHA512_BLOCK_SIZE            (128)
#define HMAC_SHA512_HASH_SIZE             (SHA512_HASH_SIZE)

typedef struct hmac_sha512_t
{
    sha512_t sha_ctx;
    uint8_t key[HMAC_SHA512_BLOCK_SIZE];
} hmac_sha512_t;


security_status_e hmac_sha512(const uint8_t *key, uint32_t key_len, 
                const uint8_t *data, uint32_t data_len, uint8_t *out);

security_status_e hmac_sha512_init(hmac_sha512_t *ctx, const uint8_t *key, uint32_t key_len);

security_status_e hmac_sha512_update(hmac_sha512_t *ctx, const uint8_t *data, uint32_t data_len);

security_status_e hmac_sha512_finish(hmac_sha512_t *ctx, uint8_t *out);

#endif /* __HMAC_SHA512_H__ */