#include "pbkdf2.h"
#include "hmac_sha1.h"
void pbkdf2_hmac_sha1(const uint8_t *password, uint32_t pass_len,
                     const uint8_t *salt, uint32_t salt_len, uint32_t iters, 
                    uint8_t *out, uint32_t out_len)
{
    int j;
    unsigned int i;
    unsigned char md1[32];
    unsigned char work[32];
    unsigned char md_size = 20;
    uint32_t use_len;
    unsigned char *out_p = out;
    unsigned char counter[4];

    memset( counter, 0, 4 );
    counter[3] = 1;

    hmac_sha1_t hmac;
    hmac_sha1_init(&hmac, password, pass_len);
    while( out_len )
    {
        hmac_sha1_update(&hmac, salt, salt_len);
        hmac_sha1_update(&hmac, counter, 4);
        hmac_sha1_final(&hmac, work);
        
        hmac_sha1_init(&hmac, password, pass_len);
    
        memcpy( md1, work, md_size );

        for( i = 1; i < iters; i++ )
        {
        hmac_sha1_update(&hmac, md1, md_size);

              hmac_sha1_final(&hmac, md1);
        
        hmac_sha1_init(&hmac, password, pass_len);
            // U1 xor U2
            //
            for( j = 0; j < md_size; j++ )
                work[j] ^= md1[j];
        }

        use_len = ( out_len < md_size ) ? out_len : md_size;
        memcpy( out_p, work, use_len );

        out_len -= (uint32_t) use_len;
        out_p += use_len;

        for( i = 4; i > 0; i-- )
            if( ++counter[i - 1] != 0 )
                break;
    }

    /* Zeroise buffers to clear sensitive data from memory. */

}

