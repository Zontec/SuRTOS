#include "hmac_sha1.h"

#define IPAD_BYTE   ((uint8_t)(0x36))
#define OPAD_BYTE   ((uint8_t)(0x5C))

void hmac_sha1_init(hmac_sha1_t *ctx, const uint8_t *key, uint32_t key_len)
{
    uint8_t ipad_xor_arr[HMAC_SHA1_BLOCK_SIZE];

    memset(ipad_xor_arr, IPAD_BYTE, sizeof(ipad_xor_arr));
    memset(ctx, 0x00, sizeof(hmac_sha1_t));

    sha1_init(&ctx->sha_ctx);

    if (key_len > HMAC_SHA1_BLOCK_SIZE)
    {
        sha1(key, key_len, ctx->key);
    }
    else 
    {
        memcpy(ctx->key, key, key_len);
    }

    for (uint32_t i = 0; i < HMAC_SHA1_BLOCK_SIZE; ++i)
    {
        ipad_xor_arr[i] = (uint8_t)(ipad_xor_arr[i] ^ ctx->key[i]);
    }

    sha1_update(&ctx->sha_ctx, ipad_xor_arr, HMAC_SHA1_BLOCK_SIZE);
}

void hmac_sha1_update(hmac_sha1_t *ctx, const uint8_t *data, uint32_t data_len)
{
    sha1_update(&ctx->sha_ctx, data, data_len);
}

void hmac_sha1_final(hmac_sha1_t *ctx, uint8_t *out)
{
    uint8_t opad_xor_arr[HMAC_SHA1_BLOCK_SIZE];

    memset(opad_xor_arr, OPAD_BYTE, sizeof(opad_xor_arr));

    for (uint32_t i = 0; i < HMAC_SHA1_BLOCK_SIZE; ++i)
    {
        opad_xor_arr[i] = (uint8_t)(opad_xor_arr[i] ^ ctx->key[i]);
    }

    sha1_finish(&ctx->sha_ctx, out);

    sha1_init(&ctx->sha_ctx);
    sha1_update(&ctx->sha_ctx, opad_xor_arr, HMAC_SHA1_BLOCK_SIZE);
    sha1_update(&ctx->sha_ctx, out, HMAC_SHA1_HASH_SIZE);
    sha1_finish(&ctx->sha_ctx, out);
}

void hmac_sha1(const uint8_t *key, uint32_t key_len, 
                const uint8_t *data, uint32_t data_len, uint8_t *out)
{
    hmac_sha1_t ctx;

    hmac_sha1_init(&ctx, key, key_len);
    hmac_sha1_update(&ctx, data, data_len);
    hmac_sha1_final(&ctx, out);
}