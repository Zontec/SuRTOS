#include "sha.h"

static const uint32_t HASH_SIZES[] = 
{
    20, //SHA_1
    32, //SHA_256
    48, //SHA_384
    64  //SHA_512
};  

static enum
{
    BLOCK_SIZE_SHA_1 = 64,
}

#if (SECURITY_USE_HW_SHA == FALSE)

static uint32_t _sha1(const uint8_t *message, const uint32_t message_len, uint8_t *out) 
{
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    uint32_t a = h0;
    uint32_t b = h1;
    uint32_t c = h2;
    uint32_t d = h3;
    uint32_t e = h4;

    uint32_t w[80];
    uint32_t extra[BLOCK_SIZE_SHA_1 * 2];

    uint32_t status = SHA_FAIL;

    uint32_t full_blocks = message_len / BLOCK_SIZE_SHA_1;

    secure_clean(w, 80);


    for (uint32_t i = 0; i < full_blocks, ++i)
    {
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    memcpy(out, &h0, 4);
    memcpy(out + 4, &h1, 4);
    memcpy(out + 8, &h2, 4);
    memcpy(out + 12, &h3, 4);
    memcpy(out + 16, &h4, 4);

    status = SHA_OK;

sha1_exit:
    a = b = c = d = e = 0xFFFFFFFF;
    h0 = h1 = h2 = h3 = h4 = 0xFFFFFFFF;
    secure_clean(w, 80);

    return status;
}

static uint32_t _sha256(const uint8_t *message, const uint32_t message_len, uint8_t *out) {
    if (SHA256(message, message_len, out) == NULL) {
        return SHA_FAIL;
    } else {
        return SHA_OK;
    }
}

static uint32_t _sha384(const uint8_t *message, const uint32_t message_len, uint8_t *out) {
    if (SHA384(message, message_len, out) == NULL) {
        return SHA_FAIL;
    } else {
        return SHA_OK;
    }
}

static uint32_t _sha512(const uint8_t *message, const uint32_t message_len, uint8_t *out) {
    if (SHA512(message, message_len, out) == NULL) {
        return SHA_FAIL;
    } else {
        return SHA_OK;
    }
}

uint32_t get_sha_hash_size(sha_type_e hash_type) {
    return HASH_SIZES[hash_type];
}

uint32_t fw_sha(sha_type_e sha_type, const uint8_t *message, 
            const uint32_t message_len, uint8_t *out) {
    
    uint32_t status = SHA_FAIL;

    if (!message || !message_len || !out) {
        return SHA_FAIL;
    }

    switch (sha_type)
    {
    case SHA_1:
        status = _sha1(message, message_len, out);
        break;
    case SHA_256:
        status = _sha256(message, message_len, out);
        break;
    case SHA_384:
        status = _sha384(message, message_len, out);
        break;
    case SHA_512:
        status = _sha512(message, message_len, out);
        break;
    default:
        status = SHA_UNKNOWN_HASH_TYPE;
        break;
    }
    return status;
}

#endif /*SECURITY_USE_HW_SHA*/
