#include "hmac.h"

uint32_t _hmac_sha1(const uint8_t *key, const uint32_t key_len, 
                const uint8_t *text, const uint32_t text_len,
                uint8_t *key_out, uint32_t *key_out_len){
    if (HMAC(EVP_sha1(), key, key_len, text, text_len, key_out, key_out_len) == NULL) {
        return HMAC_FAIL;
    } else {
        return HMAC_OK;
    }
}

uint32_t _hmac_sha256(const uint8_t *key, const uint32_t key_len, 
                const uint8_t *text, const uint32_t text_len,
                uint8_t *key_out, uint32_t *key_out_len){
    if (HMAC(EVP_sha256(), key, key_len, text, text_len, key_out, key_out_len) == NULL) {
        return HMAC_FAIL;
    } else {
        return HMAC_OK;
    }
}

uint32_t _hmac_sha384(const uint8_t *key, const uint32_t key_len, 
                const uint8_t *text, const uint32_t text_len,
                uint8_t *key_out, uint32_t *key_out_len){
    if (HMAC(EVP_sha384(), key, key_len, text, text_len, key_out, key_out_len) == NULL) {
        return HMAC_FAIL;
    } else {
        return HMAC_OK;
    }
}

uint32_t _hmac_sha512(const uint8_t *key, const uint32_t key_len, 
                const uint8_t *text, const uint32_t text_len,
                uint8_t *key_out, uint32_t *key_out_len){
    if (HMAC(EVP_sha512(), key, key_len, text, text_len, key_out, key_out_len) == NULL) {
        return HMAC_FAIL;
    } else {
        return HMAC_OK;
    }
}


uint32_t hmac(hmac_sha_type_e hmac_sha, const uint8_t *key, const uint32_t key_len, 
                const uint8_t *text, const uint32_t text_len,
                uint8_t *key_out, uint32_t *key_out_len) {
    uint32_t status = HMAC_FAIL;
    if (!key || !key_len || !text ||
        !text_len || !key_out || !key_out_len) {
        return HMAC_FAIL;
    }
    switch (hmac_sha)
    {
    case HMAC_SHA1:
        status = _hmac_sha1(key, key_len, text, text_len, key_out, key_out_len);
        break;
    case HMAC_SHA256:
        status = _hmac_sha256(key, key_len, text, text_len, key_out, key_out_len);
        break;
    case HMAC_SHA384:
        status = _hmac_sha384(key, key_len, text, text_len, key_out, key_out_len);
        break;
    case HMAC_SHA512:
        status = _hmac_sha512(key, key_len, text, text_len, key_out, key_out_len);
        break;
    default:
        status = HMAC_UNKNOWN_HASH_TYPE;
        break;
    }
    return status;
}