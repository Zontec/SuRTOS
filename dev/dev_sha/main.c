#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/pkcs7.h>
#include <openssl/aes.h>
#include <assert.h>
#include "aes.h"
#include "sha1.h"
#include "sha256.h"
#include "sha512.h"
#include "pbkdf2.h"

#include "hmac_sha1.h"
#include "hmac_sha256.h"
#include "hmac_sha512.h"
#include "kbkdf.h"
#include "pbkdf2.h"


void print_arr(uint8_t *data, uint32_t data_len)
{
    for (int i = 0; i < data_len; i++)
        printf("%02X", data[i]);
    printf("\n");
}

#define DATA_SIZE 256

char sha_test[][DATA_SIZE] = 
{
    "f43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgjf43wigjoeirgj",
    "dfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwrdfgdsfgwr",
    "fewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwkfewfjwk",
    "qweflkwje",
    "feklwqfjewk",
    "djkfdfajsdjkdjkfdfajsdjkdjkfdfajsdjkdjkfdfajsdjkdjkfdfajsdjkdjkfdfajsdkdjkfdfajsdjk",
    "11212",
    "123",
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
     hmac_sha1_finish(&hmac1, hmac);
     return 0;
}

void test_hmac_sha1()
{
    printf("HMAC_SHA1 test\n");
    uint8_t *out[SHA1_HASH_SIZE];
    uint8_t *out1[SHA1_HASH_SIZE];
    int cnt = 0;
    for(int i = 0; i < sha_test_len; i++)
    {
        hmac_sha1_t ctx;
        hmac_sha1_init(&ctx, sha_test[i], strlen(sha_test[i]));
        for (int j = 0; j < strlen(sha_test[i]); j++)
            hmac_sha1_update(&ctx, &sha_test[i][j], 1);
        hmac_sha1_finish(&ctx, out);
        
        print_arr(out, SHA1_HASH_SIZE);
        uint32_t len;
        HMAC(EVP_sha1(), sha_test[i], strlen(sha_test[i]), sha_test[i], strlen(sha_test[i]), out1, &len);
        print_arr(out1, SHA1_HASH_SIZE);
        if(!memcmp(out, out1, SHA1_HASH_SIZE)){
            cnt++;
        } else {
            printf("ERROR!\n");
        }
        printf("===============================================\n");
    }
    printf("HMAC_sha1 tests finished\n");
    assert(cnt == sha_test_len);
}

void test_hmac_sha256()
{
    printf("HMAC_sha256 test\n");
    uint8_t *out[SHA256_HASH_SIZE];
    uint8_t *out1[SHA256_HASH_SIZE];
    int cnt = 0;
    for(int i = 0; i < sha_test_len; i++)
    {
        hmac_sha256_t ctx;
        hmac_sha256_init(&ctx, sha_test[i], strlen(sha_test[i]));
        for (int j = 0; j < strlen(sha_test[i]); j++)
            hmac_sha256_update(&ctx, &sha_test[i][j], 1);
        hmac_sha256_finish(&ctx, out);
        
        print_arr(out, SHA256_HASH_SIZE);
        uint32_t len;
        HMAC(EVP_sha256(), sha_test[i], strlen(sha_test[i]), sha_test[i], strlen(sha_test[i]), out1, &len);
        print_arr(out1, SHA256_HASH_SIZE);
        if(!memcmp(out, out1, SHA256_HASH_SIZE)){
            cnt++;
        } else {
            printf("ERROR!\n");
        }
        printf("===============================================\n");
    }
    printf("HMAC_sha256 tests finished\n");
    assert(cnt == sha_test_len);
}

void test_hmac_sha512()
{
    printf("HMAC_sha512 test\n");
    uint8_t *out[SHA512_HASH_SIZE];
    uint8_t *out1[SHA512_HASH_SIZE];
    int cnt = 0;
    for(int i = 0; i < sha_test_len; i++)
    {
        hmac_sha512_t ctx;
        hmac_sha512_init(&ctx, sha_test[i], strlen(sha_test[i]));
        for (int j = 0; j < strlen(sha_test[i]); j++)
            hmac_sha512_update(&ctx, &sha_test[i][j], 1);
        hmac_sha512_finish(&ctx, out);
        
        print_arr(out, SHA512_HASH_SIZE);
        uint32_t len;
        HMAC(EVP_sha512(), sha_test[i], strlen(sha_test[i]), sha_test[i], strlen(sha_test[i]), out1, &len);
        print_arr(out1, SHA512_HASH_SIZE);
        if(!memcmp(out, out1, SHA512_HASH_SIZE)){
            cnt++;
        } else {
            printf("ERROR!\n");
        }
        printf("===============================================\n");
    }
    printf("HMAC_sha512 tests finished\n");
    assert(cnt == sha_test_len);
}

void test_pbkdf2_hmac_sha1()
{
    printf("PBKDF2_HMAC_SHA1 test\n");
    uint8_t *out[32];
    uint8_t *out1[32];
    int cnt = 0;
    for(int i = 0; i < sha_test_len; i++)
    {
        hmac_sha512_t ctx;

        pbkdf2_hmac_sha1(sha_test[i], strlen(sha_test[i]), sha_test[i], strlen(sha_test[i]), 433, out, 32);

        print_arr(out, 32);
        PKCS5_PBKDF2_HMAC_SHA1(sha_test[i], strlen(sha_test[i]), sha_test[i], strlen(sha_test[i]), 433, 32, out1);
        print_arr(out1, 32);
        if(!memcmp(out, out1, 32)){
            cnt++;
        } else {
            printf("ERROR!\n");
        }
        printf("===============================================\n");
    }
    printf("PBKDF2_HMAC_SHA1 tests finished\n");
    assert(cnt == sha_test_len);
}

void test_pbkdf2_hmac_sha256()
{
    printf("PBKDF2_HMAC_SHA256 test\n");
    uint8_t *out[32];
    uint8_t *out1[32];
    int cnt = 0;
    for(int i = 0; i < sha_test_len; i++)
    {
        hmac_sha256_t ctx;

        pbkdf2_hmac_sha256(sha_test[i], strlen(sha_test[i]), sha_test[i], strlen(sha_test[i]), 433, out, 32);

        print_arr(out, 32);
        PKCS5_PBKDF2_HMAC(sha_test[i], strlen(sha_test[i]), sha_test[i], 
                        strlen(sha_test[i]), 433, EVP_sha256(), 32, out1);
        print_arr(out1, 32);
        if(!memcmp(out, out1, 32)){
            cnt++;
        } else {
            printf("ERROR!\n");
        }
        printf("===============================================\n");
    }
    printf("PBKDF2_HMAC_SHA256 tests finished\n");
    assert(cnt == sha_test_len);
}

void test_pbkdf2_hmac_sha512()
{
    printf("PBKDF2_HMAC_SHA512 test\n");
    uint8_t *out[32];
    uint8_t *out1[32];
    int cnt = 0;
    for(int i = 0; i < sha_test_len; i++)
    {
        hmac_sha512_t ctx;

        pbkdf2_hmac_sha512(sha_test[i], strlen(sha_test[i]), sha_test[i], strlen(sha_test[i]), 433, out, 32);

        print_arr(out, 32);
        PKCS5_PBKDF2_HMAC(sha_test[i], strlen(sha_test[i]), sha_test[i], 
                        strlen(sha_test[i]), 433, EVP_sha512(), 32, out1);
        print_arr(out1, 32);
        if(!memcmp(out, out1, 32)){
            cnt++;
        } else {
            printf("ERROR!\n");
        }
        printf("===============================================\n");
    }
    printf("PBKDF2_HMAC_SHA512 tests finished\n");
    assert(cnt == sha_test_len);
}

int main()
{
    /*
    test_sha1();
    test_sha256();
    test_sha512();

    test_hmac_sha1();
    test_hmac_sha256();
    test_hmac_sha512();

    test_pbkdf2_hmac_sha1();
    test_pbkdf2_hmac_sha256();
    test_pbkdf2_hmac_sha512();
*/
    uint8_t out[128];
    uint8_t out1[128];
    aes_key_t key;

    uint8_t in[16] = "1234567812345678";
    uint8_t iv[16] = "1234567812345678";
    

    aes_key_init(AES128, &key);
    aes_key_expand(AES_KEY_EXPANSION_NOT_REQUIRED, "1234567812345678", 16, &key);
    memset(out, 0, 128);
    aes_cfb_encrypt_block(&key, in,iv, out);
    print_arr(out, 32);

  return 0;
}   