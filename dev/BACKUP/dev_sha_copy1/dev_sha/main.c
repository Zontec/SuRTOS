#include <stdio.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <assert.h>

#include "sha1.h"
#include "sha512.h"
#include "pbkdf2.h"

#include "hmac_sha1.h"
#include "hmac_sha256.h"
#include "kbkdf.h"

void print_arr(uint8_t *data, uint32_t data_len)
{
    for (int i = 0; i < data_len; i++)
        printf("%02X", data[i]);
    printf("\n");
}

#define DATA_SIZE 256

uint8_t sha_test[][DATA_SIZE] = 
{
    "f43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgj",
    "dfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwr",
    "fewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwk",
    "qweflkwje",
    "feklwqfjewk",
    "djkfdfajsdjkdjkfdfajsdjkdjkfdfajsdjkdjkfdfajsdjkdjkfdfajsdjkdjkfdfajsdkdjkfdfajsdjk",
    "11212",
    "",
    "flkds;lafddddddddddddddddddddddddddddddddddddddddddlfklskdlfksldkflsdfkld",
};
const uint8_t sha_test_len = sizeof(sha_test)/DATA_SIZE;

void test_sha1()
{
    printf("SHA1 test\n");
    uint8_t *out[SHA1_HASH_SIZE];
    uint8_t *out1[SHA1_HASH_SIZE];
    int cnt = 0;
    for(int i = 0; i < sha_test_len; i++)
    {
        sha1_t ctx;
        sha1_init(&ctx);
        for (int j = 0; j < DATA_SIZE; j++)
            sha1_update(&ctx, &sha_test[i][j], 1);
        sha1_finish(&ctx, out);
        
        print_arr(out, SHA1_HASH_SIZE);

        SHA1(sha_test[i], DATA_SIZE, out1);
        print_arr(out1, SHA1_HASH_SIZE);
        if(!memcmp(out, out1, SHA1_HASH_SIZE)){
            cnt++;
        } else {
            printf("ERROR!\n");
        }
        printf("===============================================\n");
    }
    printf("sha1 tests finished\n");
    assert(cnt == sha_test_len);
}

void test_sha256()
{
    printf("SHA256 test\n");
    uint8_t *out[SHA256_HASH_SIZE];
    uint8_t *out1[SHA256_HASH_SIZE];
    int cnt = 0;
    for(int i = 0; i < sha_test_len; i++)
    {
        sha256_t ctx;
        sha256_init(&ctx);
        for (int j = 0; j < DATA_SIZE; j++)
            sha256_update(&ctx, &sha_test[i][j], 1);
        sha256_finish(&ctx, out);
        
        print_arr(out, SHA256_HASH_SIZE);

        SHA256(sha_test[i], DATA_SIZE, out1);
        print_arr(out1, SHA256_HASH_SIZE);
        if(!memcmp(out, out1, SHA256_HASH_SIZE)){
            cnt++;
        } else {
            printf("ERROR!\n");
        }
        printf("===============================================\n");
    }
    printf("sha256 tests finished\n");
    assert(cnt == sha_test_len);
}


void test_sha512()
{
    printf("SHA512 test\n");
    uint8_t *out[SHA512_HASH_SIZE];
    uint8_t *out1[SHA512_HASH_SIZE];
    int cnt = 0;
    for(int i = 0; i < sha_test_len; i++)
    {
        sha512_t ctx;
        sha512_init(&ctx);
        for (int j = 0; j < DATA_SIZE; j++)
            sha512_update(&ctx, &sha_test[i][j], 1);
        sha512_finish(&ctx, out);
        
        print_arr(out, SHA512_HASH_SIZE);

        SHA512(sha_test[i], DATA_SIZE, out1);
        print_arr(out1, SHA512_HASH_SIZE);
        if(!memcmp(out, out1, SHA512_HASH_SIZE)){
            cnt++;
        } else {
            printf("ERROR!\n");
        }
        printf("===============================================\n");
    }
    printf("sha512 tests finished\n");
    assert(cnt == sha_test_len);
}
hmac_sha1_t hmac1;

uint32_t hmac1_init(uint8_t *key, uint32_t key_len)
{
     hmac_sha256_init(&hmac1, key, key_len);
     return 0;

}

uint32_t hmac1_update(uint8_t *data, uint32_t data_len)
{
     hmac_sha256_update(&hmac1, data, data_len);
     return 0;

}

uint32_t hmac1_final(uint8_t *hmac)
{
     hmac_sha256_final(&hmac1, hmac);
     return 0;
}


int main(){
    char *key = "123";
    char *data = "123";
    uint8_t out[256];
    uint8_t out1[256];
    uint32_t out_len = 32;

    uint32_t key_len = strlen(key);
    uint32_t data_len = strlen(data);




    kbkdf_hmac_callbacks_t callbacks;
    
    callbacks.hmac_init = hmac1_init;
    callbacks.hmac_update = hmac1_update;
    callbacks.hmac_final = hmac1_final;

    kbkdf(KBKDF_MODE_COUNTER, KBKDF_HASH_TYPE_SHA256, callbacks, 
        key, key_len, "", 0, data, data_len, out, 32 * 8, NULL);
    
    //pbkdf2_hmac_sha1(key, strlen(key), data, strlen(data), 1000, out, out_len);
    //PKCS5_PBKDF2_HMAC_SHA1(key, strlen(key), data, strlen(data), 1000, out_len, out1);

    print_arr(out, out_len);
    print_arr(out1, out_len);
    
    return 0;
}