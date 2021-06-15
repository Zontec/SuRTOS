#ifndef __HMAC_H__
#define __HMAC_H__

#include "common_security.h"

typedef enum hmac_sha_type_e {
    HMAC_SHA1 = 0,
    HMAC_SHA256 = 1,
    HMAC_SHA384 = 2,
    HMAC_SHA512 = 3,
} hmac_sha_type_e;

typedef enum hmac_status_e {
    HMAC_OK = 0,
    HMAC_FAIL = 1,
    HMAC_UNKNOWN_HASH_TYPE = 2,
} hmac_status_e;

uint32_t hmac(hmac_sha_type_e hmac_sha, const uint8_t *key, const uint32_t key_len, 
                const uint8_t *text, const uint32_t text_len,
                uint8_t *key_out, uint32_t *key_out_len);
#endif /*__HMAC_H__*/