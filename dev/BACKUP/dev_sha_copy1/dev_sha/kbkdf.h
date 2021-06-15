
#ifndef __KBKDF_H__
#define __KBKDF_H__

#include <stdint.h>
#include "security.h"

typedef enum kbkdf_hash_type_e
{
    KBKDF_HASH_TYPE_SHA256 = 0,
    KBKDF_HASH_TYPE_SHA384 = 1,
    KBKDF_HASH_TYPE_SHA512 = 2,
} kbkdf_hash_type_e;

typedef uint32_t(*hmac_init)(const uint8_t *key, const uint32_t key_size);
typedef uint32_t(*hmac_update)(const uint8_t *data, const uint32_t data_size);
typedef uint32_t(*hmac_final)(uint8_t *hmac);

typedef struct kbkdf_hmac_callbacks_t
{
    hmac_init hmac_init;
    hmac_update hmac_update;
    hmac_final hmac_final;
}kbkdf_hmac_callbacks_t;

typedef enum kbkdf_mode_e
{
    KBKDF_MODE_COUNTER = 0,
    KBKDF_MODE_FEEDBACK = 1,
    KBKDF_MODE_DOUBLE_PIPELINE = 2,
} kbkdf_mode_e;

typedef struct kbkdf_opts_t
{
    uint32_t ctr_rlen;
    int32_t ctr_rpos;
} kbkdf_opts_t;


#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief This function derives a key based on the provided key and fixed input string using HMAC KBKDF algorithm.
*
* @param[in] mode KBKDF mode to be used
* @param[in] hash_type hash type to use in hmac
* @param[in] hmac_callbacks input structure with HMAC callbacks
* @param[in] key_in input key buffer
* @param[in] key_in_len input key length in bytes
* @param[in] iv_in input IV buffer
* @param[in] iv_in_len input IV length in bytes
* @param[in] fixed_input fixed input buffer
* @param[in] fixed_input_len fixed input length in bytes
* @param[in, out] key_out start address of the KBKDF key
* @param[in] key_out_len required output key length
* @param[in] opts counter options
* @return eSTATUS_SUCCESS or
* eGENERIC_ERROR
*/
security_status_e kbkdf(kbkdf_mode_e mode, kbkdf_hash_type_e hash_type,
            kbkdf_hmac_callbacks_t hmac_callbacks,
            const uint8_t* key_in, const uint32_t key_in_len,
            const uint8_t* iv_in, const uint32_t iv_in_len,
            uint8_t* fixed_input, const uint32_t fixed_input_len,
            uint8_t* key_out, const uint32_t key_out_len,
            kbkdf_opts_t* opts);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*__KBKDF_H__*/