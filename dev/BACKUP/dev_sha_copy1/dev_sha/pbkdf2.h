#include <stdint.h>


void pbkdf2_hmac_sha1(const uint8_t *password, uint32_t pass_len,
                     const uint8_t *salt, uint32_t salt_len, uint32_t iters, 
                    uint8_t *out, uint32_t out_len);