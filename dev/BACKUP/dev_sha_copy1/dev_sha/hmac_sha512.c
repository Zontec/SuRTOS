#include "hmac_sha512.h"

#define IPAD_BYTE   ((uint8_t)(0x36))
#define OPAD_BYTE   ((uint8_t)(0x5C))

void hmac_sha512_init(hmac_sha512_t *ctx, const uint8_t *key, uint32_t key_len)
{
    uint8_t ipad_xor_arr[HMAC_SHA512_BLOCK_SIZE];

    memset(ipad_xor_arr, IPAD_BYTE, sizeof(ipad_xor_arr));
    memset(ctx, 0x00, sizeof(hmac_sha512_t));

    sha512_init(&ctx->sha_ctx);

    if (key_len > HMAC_SHA512_BLOCK_SIZE)
    {
        sha512(key, key_len, ctx->key);
    }
    else 
    {
        memcpy(ctx->key, key, key_len);
    }

    for (uint32_t i = 0; i < HMAC_SHA512_BLOCK_SIZE; ++i)
    {
        ipad_xor_arr[i] = (uint8_t)(ipad_xor_arr[i] ^ ctx->key[i]);
    }

    sha512_update(&ctx->sha_ctx, ipad_xor_arr, HMAC_SHA512_BLOCK_SIZE);
}

void hmac_sha512_update(hmac_sha512_t *ctx, const uint8_t *data, uint32_t data_len)
{
    sha512_update(&ctx->sha_ctx, data, data_len);
}

void hmac_sha512_final(hmac_sha512_t *ctx, uint8_t *out)
{
    uint8_t opad_xor_arr[HMAC_SHA512_BLOCK_SIZE];

    memset(opad_xor_arr, OPAD_BYTE, sizeof(opad_xor_arr));

    for (uint32_t i = 0; i < HMAC_SHA512_BLOCK_SIZE; ++i)
    {
        opad_xor_arr[i] = (uint8_t)(opad_xor_arr[i] ^ ctx->key[i]);
    }

    sha512_finish(&ctx->sha_ctx, out);

    sha512_init(&ctx->sha_ctx);
    sha512_update(&ctx->sha_ctx, opad_xor_arr, HMAC_SHA512_BLOCK_SIZE);
    sha512_update(&ctx->sha_ctx, out, HMAC_SHA512_HASH_SIZE);
    sha512_finish(&ctx->sha_ctx, out);
}

void hmac_sha512(const uint8_t *key, uint32_t key_len, 
                const uint8_t *data, uint32_t data_len, uint8_t *out)
{
    hmac_sha512_t ctx;

    hmac_sha512_init(&ctx, key, key_len);
    hmac_sha512_update(&ctx, data, data_len);
    hmac_sha512_final(&ctx, out);
}