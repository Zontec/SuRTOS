#ifndef __AES_H__
#define __AES_H__

#include "security.h"

#define AES_BLOCK_SIZE              (16)

typedef enum aes_type_e
{
    AES128 = 0,
    AES192 = 1,
    AES256 = 2,
} aes_type_e;

typedef enum aes_key_expansion_hash_type_e
{
    AES_KEY_EXPANSION_SHA1 =            0,
    AES_KEY_EXPANSION_SHA256 =          1,
    AES_KEY_EXPANSION_SHA512 =          2,
    AES_KEY_EXPANSION_NOT_REQUIRED =    3,
} aes_key_expansion_hash_type_e;

typedef struct aes_key_t
{
    uint8_t *w;
    aes_type_e aes_type;
} aes_key_t;


security_status_e aes_key_init(aes_type_e aes_type, aes_key_t *aes_key);

security_status_e aes_key_expand(aes_key_expansion_hash_type_e key_exp_hash_type,
                                    const uint8_t *key_in, uint32_t key_in_len, aes_key_t *key_out);

void aes_key_free(aes_key_t *aes_key);


security_status_e aes_encrypt_block(aes_key_t *key, const uint8_t in[AES_BLOCK_SIZE], uint8_t out[AES_BLOCK_SIZE]);

security_status_e aes_cbc_encrypt_block(aes_key_t *key, const uint8_t in[AES_BLOCK_SIZE], 
                                        const uint8_t iv[AES_BLOCK_SIZE], uint8_t out[AES_BLOCK_SIZE]);

security_status_e aes_decrypt_block(aes_key_t *key, const uint8_t in[AES_BLOCK_SIZE], uint8_t out[AES_BLOCK_SIZE]);


security_status_e aes_encrypt(const uint8_t *data, uint32_t data_len, aes_key_t *key, uint8_t *out);

security_status_e aes_encrypt(const uint8_t *data, uint32_t data_len, aes_key_t *key, uint8_t *out);

security_status_e aes_decrypt(const uint8_t *c_data, uint32_t c_data_len, aes_key_t *key, uint8_t *out);


security_status_e aes_encrypt_ex(aes_type_e aes_type, aes_key_expansion_hash_type_e key_exp_hash_type, 
                                const uint8_t *data, uint32_t data_len, 
                                uint8_t *key, uint32_t key_len, uint8_t *out);

security_status_e aes_decrypt_ex(aes_type_e aes_type, aes_key_expansion_hash_type_e key_exp_hash_type, 
                                const uint8_t *c_data, uint32_t c_data_len, 
                                uint8_t *key, uint32_t key_len, uint8_t *out);

#endif /* __AES_H__ */