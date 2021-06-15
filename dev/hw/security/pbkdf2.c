#include "pbkdf2.h"
#include "hmac_sha1.h"
#include "hmac_sha256.h"
#include "hmac_sha512.h"

#define PBKDF2_HMAC_SHA1_BUFFER_SIZE            (32)
#define PBKDF2_HMAC_SHA256_BUFFER_SIZE          (64)
#define PBKDF2_HMAC_SHA512_BUFFER_SIZE          (128)

security_status_e pbkdf2_hmac_sha1(const uint8_t *password, uint32_t pass_len,
                                    const uint8_t *salt, uint32_t salt_len, uint32_t iters, 
                                    uint8_t *out, uint32_t out_len)
{
    _SECURITY_FUNCTION_BEGIN;

    _SECURITY_CHECK_VALID_NOT_NULL(password);
    _SECURITY_CHECK_VALID_NOT_NULL(salt);
    _SECURITY_CHECK_VALID_NOT_NULL(out);

    int32_t j;
    uint32_t i;
    uint32_t use_len;
    uint8_t md1[PBKDF2_HMAC_SHA1_BUFFER_SIZE];
    uint8_t work[PBKDF2_HMAC_SHA1_BUFFER_SIZE];
    uint8_t md_size = HMAC_SHA1_HASH_SIZE;
    uint8_t *out_p = out;
    uint8_t counter[4];
    uint32_t ctr_len = sizeof(counter) / sizeof(uint8_t);

    hmac_sha1_t hmac;

    _SECURITY_CHECK_VALID_NOT_NULL(memset(counter, 0x00, ctr_len));

    counter[3] = 1;

    _SECURITY_VALID_RES(hmac_sha1_init(&hmac, password, pass_len));

    while (out_len != 0)
    {
        _SECURITY_VALID_RES(hmac_sha1_update(&hmac, salt, salt_len));
        _SECURITY_VALID_RES(hmac_sha1_update(&hmac, counter, ctr_len));
        _SECURITY_VALID_RES(hmac_sha1_finish(&hmac, work));
        
        _SECURITY_VALID_RES(hmac_sha1_init(&hmac, password, pass_len));
    
        _SECURITY_CHECK_VALID_NOT_NULL(memcpy(md1, work, md_size));

        for(i = 1; i < iters; ++i)
        {
            _SECURITY_VALID_RES(hmac_sha1_update(&hmac, md1, md_size));
            _SECURITY_VALID_RES(hmac_sha1_finish(&hmac, md1));
            _SECURITY_VALID_RES(hmac_sha1_init(&hmac, password, pass_len));
            // U1 xor U2
            for(j = 0; j < md_size; ++j)
            {
                work[j] ^= md1[j];
            }
        }

        use_len = (out_len < md_size) ? out_len : md_size;

        _SECURITY_CHECK_VALID_NOT_NULL(memcpy(out_p, work, use_len));

        out_len -= U32(use_len);
        out_p += use_len;

        for(i = 4; i > 0; --i)
        {
            if(++counter[i - 1] != 0)
            {
                break;
            }
        }
    }

_SECURITY_EXIT:
    _SECURITY_FUNCTION_END;
    /* Zeroise buffers to clear sensitive data from memory. */
}

security_status_e pbkdf2_hmac_sha256(const uint8_t *password, uint32_t pass_len,
                                    const uint8_t *salt, uint32_t salt_len, uint32_t iters, 
                                    uint8_t *out, uint32_t out_len)
{
    _SECURITY_FUNCTION_BEGIN;

    _SECURITY_CHECK_VALID_NOT_NULL(password);
    _SECURITY_CHECK_VALID_NOT_NULL(salt);
    _SECURITY_CHECK_VALID_NOT_NULL(out);

    int32_t j;
    uint32_t i;
    uint32_t use_len;
    uint8_t md256[PBKDF2_HMAC_SHA256_BUFFER_SIZE];
    uint8_t work[PBKDF2_HMAC_SHA256_BUFFER_SIZE];
    uint8_t md_size = HMAC_SHA256_HASH_SIZE;
    uint8_t *out_p = out;
    uint8_t counter[4];
    uint32_t ctr_len = sizeof(counter) / sizeof(uint8_t);

    hmac_sha256_t hmac;

    _SECURITY_CHECK_VALID_NOT_NULL(memset(counter, 0x00, ctr_len));

    counter[3] = 1;

    _SECURITY_VALID_RES(hmac_sha256_init(&hmac, password, pass_len));

    while (out_len != 0)
    {
        _SECURITY_VALID_RES(hmac_sha256_update(&hmac, salt, salt_len));
        _SECURITY_VALID_RES(hmac_sha256_update(&hmac, counter, ctr_len));
        _SECURITY_VALID_RES(hmac_sha256_finish(&hmac, work));
        
        _SECURITY_VALID_RES(hmac_sha256_init(&hmac, password, pass_len));
    
        _SECURITY_CHECK_VALID_NOT_NULL(memcpy(md256, work, md_size));

        for(i = 1; i < iters; ++i)
        {
            _SECURITY_VALID_RES(hmac_sha256_update(&hmac, md256, md_size));
            _SECURITY_VALID_RES(hmac_sha256_finish(&hmac, md256));
            _SECURITY_VALID_RES(hmac_sha256_init(&hmac, password, pass_len));
            // U256 xor U2
            for(j = 0; j < md_size; ++j)
            {
                work[j] ^= md256[j];
            }
        }

        use_len = (out_len < md_size) ? out_len : md_size;

        _SECURITY_CHECK_VALID_NOT_NULL(memcpy(out_p, work, use_len));

        out_len -= U32(use_len);
        out_p += use_len;

        for(i = 4; i > 0; --i)
        {
            if(++counter[i - 1] != 0)
            {
                break;
            }
        }
    }

_SECURITY_EXIT:
    _SECURITY_FUNCTION_END;
    /* Zeroise buffers to clear sensitive data from memory. */
}



security_status_e pbkdf2_hmac_sha512(const uint8_t *password, uint32_t pass_len,
                                    const uint8_t *salt, uint32_t salt_len, uint32_t iters, 
                                    uint8_t *out, uint32_t out_len)
{
    _SECURITY_FUNCTION_BEGIN;

    _SECURITY_CHECK_VALID_NOT_NULL(password);
    _SECURITY_CHECK_VALID_NOT_NULL(salt);
    _SECURITY_CHECK_VALID_NOT_NULL(out);

    int32_t j;
    uint32_t i;
    uint32_t use_len;
    uint8_t md512[PBKDF2_HMAC_SHA512_BUFFER_SIZE];
    uint8_t work[PBKDF2_HMAC_SHA512_BUFFER_SIZE];
    uint8_t md_size = HMAC_SHA512_HASH_SIZE;
    uint8_t *out_p = out;
    uint8_t counter[4];
    uint32_t ctr_len = sizeof(counter) / sizeof(uint8_t);

    hmac_sha512_t hmac;

    _SECURITY_CHECK_VALID_NOT_NULL(memset(counter, 0x00, ctr_len));

    counter[3] = 1;

    _SECURITY_VALID_RES(hmac_sha512_init(&hmac, password, pass_len));

    while (out_len != 0)
    {
        _SECURITY_VALID_RES(hmac_sha512_update(&hmac, salt, salt_len));
        _SECURITY_VALID_RES(hmac_sha512_update(&hmac, counter, ctr_len));
        _SECURITY_VALID_RES(hmac_sha512_finish(&hmac, work));
        
        _SECURITY_VALID_RES(hmac_sha512_init(&hmac, password, pass_len));
    
        _SECURITY_CHECK_VALID_NOT_NULL(memcpy(md512, work, md_size));

        for(i = 1; i < iters; ++i)
        {
            _SECURITY_VALID_RES(hmac_sha512_update(&hmac, md512, md_size));
            _SECURITY_VALID_RES(hmac_sha512_finish(&hmac, md512));
            _SECURITY_VALID_RES(hmac_sha512_init(&hmac, password, pass_len));
            // U512 xor U2
            for(j = 0; j < md_size; ++j)
            {
                work[j] ^= md512[j];
            }
        }

        use_len = (out_len < md_size) ? out_len : md_size;

        _SECURITY_CHECK_VALID_NOT_NULL(memcpy(out_p, work, use_len));

        out_len -= U32(use_len);
        out_p += use_len;

        for(i = 4; i > 0; --i)
        {
            if(++counter[i - 1] != 0)
            {
                break;
            }
        }
    }

_SECURITY_EXIT:
    _SECURITY_FUNCTION_END;
    /* Zeroise buffers to clear sensitive data from memory. */
}




