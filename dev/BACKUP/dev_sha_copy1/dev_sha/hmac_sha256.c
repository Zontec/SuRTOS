#include "hmac_sha256.h"

#define IPAD_BYTE   ((uint8_t)(0x36))
#define OPAD_BYTE   ((uint8_t)(0x5C))

void hmac_sha256_init(hmac_sha256_t *ctx, const uint8_t *key, uint32_t key_len)
{
    uint8_t ipad_xor_arr[HMAC_SHA256_BLOCK_SIZE];

    memset(ipad_xor_arr, IPAD_BYTE, sizeof(ipad_xor_arr));
    memset(ctx, 0x00, sizeof(hmac_sha256_t));

    sha256_init(&ctx->sha_ctx);

    if (key_len > HMAC_SHA256_BLOCK_SIZE)
    {
        sha256(key, key_len, ctx->key);
    }
    else 
    {
        memcpy(ctx->key, key, key_len);
    }

    for (uint32_t i = 0; i < HMAC_SHA256_BLOCK_SIZE; ++i)
    {
        ipad_xor_arr[i] = (uint8_t)(ipad_xor_arr[i] ^ ctx->key[i]);
    }

    sha256_update(&ctx->sha_ctx, ipad_xor_arr, HMAC_SHA256_BLOCK_SIZE);
}

void hmac_sha256_update(hmac_sha256_t *ctx, const uint8_t *data, uint32_t data_len)
{
    sha256_update(&ctx->sha_ctx, data, data_len);
}

void hmac_sha256_final(hmac_sha256_t *ctx, uint8_t *out)
{
    uint8_t opad_xor_arr[HMAC_SHA256_BLOCK_SIZE];

    memset(opad_xor_arr, OPAD_BYTE, sizeof(opad_xor_arr));

    for (uint32_t i = 0; i < HMAC_SHA256_BLOCK_SIZE; ++i)
    {
        opad_xor_arr[i] = (uint8_t)(opad_xor_arr[i] ^ ctx->key[i]);
    }

    sha256_finish(&ctx->sha_ctx, out);

    sha256_init(&ctx->sha_ctx);
    sha256_update(&ctx->sha_ctx, opad_xor_arr, HMAC_SHA256_BLOCK_SIZE);
    sha256_update(&ctx->sha_ctx, out, HMAC_SHA256_HASH_SIZE);
    sha256_finish(&ctx->sha_ctx, out);
}

void hmac_sha256(const uint8_t *key, uint32_t key_len, 
                const uint8_t *data, uint32_t data_len, uint8_t *out)
{
    hmac_sha256_t ctx;

    hmac_sha256_init(&ctx, key, key_len);
    hmac_sha256_update(&ctx, data, data_len);
    hmac_sha256_final(&ctx, out);
}