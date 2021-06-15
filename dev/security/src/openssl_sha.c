#include "sha.h"

static const uint32_t HASH_SIZES[] = {
    20, //SHA_1
    32, //SHA_256
    48, //SHA_384
    64  //SHA_512
};  

#if (SECURITY_USE_HW_SHA == FALSE)

static uint32_t _sha1(const uint8_t *message, const uint32_t message_len, uint8_t *out) {
    if (SHA1(message, message_len, out) == NULL) {
        return SHA_FAIL;
    } else {
        return SHA_OK;
    }
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
