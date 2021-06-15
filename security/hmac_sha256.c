#include "hmac_sha256.h"

#define IPAD_BYTE   (U8(0x36))
#define OPAD_BYTE   (U8(0x5C))

security_status_e hmac_sha256_init(hmac_sha256_t *ctx, const uint8_t *key, uint32_t key_len)
{
    _SECURITY_FUNCTION_BEGIN;

    uint8_t ipad_xor_arr[HMAC_SHA256_BLOCK_SIZE];

    _SECURITY_CHECK_VALID_NOT_NULL(ctx);
    _SECURITY_CHECK_VALID_NOT_NULL(key);

    _SECURITY_CHECK_VALID_NOT_NULL(memset(ipad_xor_arr, IPAD_BYTE, sizeof(ipad_xor_arr)));
    _SECURITY_CHECK_VALID_NOT_NULL(memset(ctx, 0x00, sizeof(hmac_sha256_t)));

    _SECURITY_VALID_RES(sha256_init(&ctx->sha_ctx));

    if (key_len > HMAC_SHA256_BLOCK_SIZE)
    {
        _SECURITY_VALID_RES(sha256(key, key_len, ctx->key));
    }
    else 
    {
        _SECURITY_CHECK_VALID_NOT_NULL(memcpy(ctx->key, key, key_len));
    }

    for (uint32_t i = 0; i < HMAC_SHA256_BLOCK_SIZE; ++i)
    {
        ipad_xor_arr[i] = (uint8_t)(ipad_xor_arr[i] ^ ctx->key[i]);
    }

    _SECURITY_VALID_RES(sha256_update(&ctx->sha_ctx, ipad_xor_arr, HMAC_SHA256_BLOCK_SIZE));

_SECURITY_EXIT:
    _SECURITY_FUNCTION_END;
}

security_status_e hmac_sha256_update(hmac_sha256_t *ctx, const uint8_t *data, uint32_t data_len)
{
    _SECURITY_FUNCTION_BEGIN;

    _SECURITY_CHECK_VALID_NOT_NULL(ctx);
    _SECURITY_CHECK_VALID_NOT_NULL(data);

    if (data_len == 0)
    {
        goto _SECURITY_EXIT;
    }

    _SECURITY_VALID_RES(sha256_update(&ctx->sha_ctx, data, data_len));

_SECURITY_EXIT:
    _SECURITY_FUNCTION_END;
}

security_status_e hmac_sha256_finish(hmac_sha256_t *ctx, uint8_t *out)
{
    _SECURITY_FUNCTION_BEGIN;

    uint8_t opad_xor_arr[HMAC_SHA256_BLOCK_SIZE];

    _SECURITY_CHECK_VALID_NOT_NULL(ctx);
    _SECURITY_CHECK_VALID_NOT_NULL(out);

    _SECURITY_CHECK_VALID_NOT_NULL(memset(opad_xor_arr, OPAD_BYTE, sizeof(opad_xor_arr)));

    for (uint32_t i = 0; i < HMAC_SHA256_BLOCK_SIZE; ++i)
    {
        opad_xor_arr[i] = U8(opad_xor_arr[i] ^ ctx->key[i]);
    }

    _SECURITY_VALID_RES(sha256_finish(&ctx->sha_ctx, out));

    _SECURITY_VALID_RES(sha256_init(&ctx->sha_ctx));
    _SECURITY_VALID_RES(sha256_update(&ctx->sha_ctx, opad_xor_arr, HMAC_SHA256_BLOCK_SIZE));
    _SECURITY_VALID_RES(sha256_update(&ctx->sha_ctx, out, HMAC_SHA256_HASH_SIZE));
    _SECURITY_VALID_RES(sha256_finish(&ctx->sha_ctx, out));

_SECURITY_EXIT:
    _SECURITY_FUNCTION_END;
}

security_status_e hmac_sha256(const uint8_t *key, uint32_t key_len, 
                const uint8_t *data, uint32_t data_len, uint8_t *out)
{
    _SECURITY_FUNCTION_BEGIN;

    hmac_sha256_t ctx;

    _SECURITY_VALID_RES(hmac_sha256_init(&ctx, key, key_len));
    _SECURITY_VALID_RES(hmac_sha256_update(&ctx, data, data_len));
    _SECURITY_VALID_RES(hmac_sha256_finish(&ctx, out));

_SECURITY_EXIT:
    _SECURITY_FUNCTION_END;
}